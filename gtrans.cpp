// gtrans.cpp

// Copyright (c) 2010, Jeremiah LaRocco jlarocco@jlarocco.com

// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.

// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include "gtrans.h"

#include <QtGui>
#include <QMap>

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

void GTrans::fillInLanguages() {
    if (languages.size()==0) {
        languages[tr("French")] = tr("fr");
        languages[tr("Spanish")] = tr("es");
        languages[tr("Afrikaans")] = tr("af");
        languages[tr("Albanian")] = tr("sq");
                         
        languages[tr("Arabic")] = tr("ar");
        languages[tr("Belarusian")] = tr("be");
        languages[tr("Bulgarian")] = tr("bg");
        languages[tr("Catalan")] = tr("ca");
        languages[tr("Chinese")] = tr("zh-cn");
                         
        languages[tr("Croatian")] = tr("hr");
        languages[tr("Czech")] = tr("cs");
        languages[tr("Danish")] = tr("da");
        languages[tr("Dutch")] = tr("nl");
        languages[tr("English")] = tr("en");
        languages[tr("Estonian")] = tr("et");
                         
        languages[tr("Filipino")] = tr("tl");
        languages[tr("Finnish")] = tr("fi");
        languages[tr("French")] = tr("fr");
        languages[tr("Galician")] = tr("gl");
        languages[tr("German")] = tr("de");
        languages[tr("Greek")] = tr("el");
                         
        languages[tr("Haitian creole alpha")] = tr("ht");
        languages[tr("Hebrew")] = tr("iw");
        languages[tr("Hindi")] = tr("hi");
        languages[tr("Hungarian")] = tr("hu");
        languages[tr("Icelandic")] = tr("is");
                         
        languages[tr("Indonesian")] = tr("id");
        languages[tr("Irish")] = tr("ga");
        languages[tr("Italian")] = tr("it");
        languages[tr("Japanese")] = tr("ja");
        languages[tr("Korean")] = tr("ko");
                         
        languages[tr("Latvian")] = tr("lv");
        languages[tr("Lithuanian")] = tr("lt");
        languages[tr("Macedonian")] = tr("mk");
        languages[tr("Malay")] = tr("ms");
        languages[tr("Maltese")] = tr("mt");
                         
        languages[tr("Norwegian")] = tr("no");
        languages[tr("Persian")] = tr("fa");
        languages[tr("Polish")] = tr("pl");
        languages[tr("Portuguese")] = tr("pt");
        languages[tr("Romanian")] = tr("ro");
                         
        languages[tr("Russian")] = tr("ru");
        languages[tr("Serbian")] = tr("sr");
        languages[tr("Slovak")] = tr("sk");
        languages[tr("Slovenian")] = tr("sl");
        languages[tr("Spanish")] = tr("es");
                         
        languages[tr("Swahili")] = tr("sw");
        languages[tr("Swedish")] = tr("sv");
        languages[tr("Thai")] = tr("th");
        languages[tr("Turkish")] = tr("tr");
        languages[tr("Ukrainian")] = tr("uk");
                         
        languages[tr("Vietnamese")] = tr("vi");
        languages[tr("Welsh")] = tr("cy");
        languages[tr("Yiddish")]= tr("yi");
    }
    fromLang->addItems(languages.keys());
    toLang->addItems(languages.keys());

    fromLang->setCurrentIndex(fromLang->findText(tr("English")));
    toLang->setCurrentIndex(toLang->findText(tr("French")));
}

GTrans::GTrans() {
    QLayout *mainLayout = new QVBoxLayout;

    // The input section
    QLayout *top = new QHBoxLayout;
    QBoxLayout *tl = new QVBoxLayout;
    QLabel *inLabel = new QLabel(tr("Input:"));
    fromLang = new QComboBox;
    tl->addWidget(inLabel);
    tl->addSpacing(10);
    tl->addWidget(fromLang);
    tl->addStretch();
    top->addItem(tl);
    inputTxt = new QTextEdit;

    top->addWidget(inputTxt);

    // The output section
    QLayout *bottom = new QHBoxLayout;
    QBoxLayout *bl = new QVBoxLayout;
    QLabel *outLabel = new QLabel(tr("Output:"));
    toLang = new QComboBox;
    bl->addWidget(outLabel);
    bl->addSpacing(10);
    bl->addWidget(toLang);
    bl->addStretch();
    bottom->addItem(bl);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());   //lzt QTextcode modify
    outputTxt = new QTextEdit;
    outputTxt->setReadOnly(true);
    bottom->addWidget(outputTxt);

    mainLayout->addItem(top);
    mainLayout->addItem(bottom);

    // Translate button
    trans_b = new QPushButton(tr("Translate"));
    mainLayout->addWidget(trans_b);

    fillInLanguages();

    setLayout(mainLayout);
    setWindowTitle(tr("Translate"));
    
    trans_b->setDefault(true);
    connect(trans_b, SIGNAL(clicked()), this, SLOT(doTrans()));

    // Setup foxus and tab order.
    inputTxt->setTabChangesFocus(true);
    inputTxt->setFocus(Qt::ActiveWindowFocusReason);
    setTabOrder(inputTxt, toLang);
    setTabOrder(toLang, trans_b);
    setTabOrder(trans_b, fromLang);
    setTabOrder(fromLang, inputTxt);
    outputTxt->setFocusProxy(trans_b);
}

void GTrans::doTrans() {
    nam = new QNetworkAccessManager(this);

    // Figure out the URL for translate.google.com
    QStringList words = inputTxt->toPlainText().split(QRegExp("\\s+"));
    //QString gtUrl = tr("https://translate.google.com/translate_t?langpair=%1|%2&text=").arg(languages[fromLang->currentText()]).arg(languages[toLang->currentText()]) + words.join("+");
    QString gtUrl = tr("http://dict.youdao.com/search?q=") + words.join("+");
//  outputTxt->setPlainText(gtUrl);
//lzt1
    QUrl url(gtUrl);

    QNetworkRequest qnr(url);

    // Google checks for certain user agents, so spoof one
    qnr.setRawHeader("User-Agent", "");

    // Make the request.
    nam->get(qnr);

    // Tell it to get back to us...
    connect(nam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(finishedSlot(QNetworkReply*)));

    // Go back to the text input
    inputTxt->selectAll();
    inputTxt->setFocus();
}

void GTrans::finishedSlot(QNetworkReply* reply) {

    // The request finished, so check for an error
    if (reply->error() == QNetworkReply::NoError) {

//      outputTxt->setPlainText(tr(reply->readAll()));
        outputTxt->setHtml(tr(reply->readAll()));
//lzt2
        // Find the translated string - this is a bit of a hack, but works and is easier than parsing the HTML...
/*      QRegExp rx(tr("input type=hidden name=gtrans value=\"(.*)\"><div id=sug_exp>"));
        QRegExp rx(tr("<div id=\"phrsListTab\" class=\"trans-wrapper clearfix\">(.*)</div>"));
        QString rep(reply->readAll());
        rep.replace("\n", " ");

        // Check that the translated text was found
        if (rx.indexIn(rep) != -1) {
            // The text in rx.cap(1) is html character encoded so that "&" => "&amp;"
            // That f's up QTextEdit's "setHtml", so fix it
            QString tmp = rx.cap(1);
            tmp.replace(tr("&amp;"), tr("&"));
            
            // Display the results
            outputTxt->setHtml(tmp);
        } else {
            outputTxt->setPlainText(tr("Could not find translated string in result..."));
//lzt3
        }
*/
    } else {
        outputTxt->setPlainText(tr("A network error occured."));
    }

    // Schedule old network stuff for deletion

    reply->deleteLater();
    nam->deleteLater();
}
