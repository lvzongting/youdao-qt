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
#include <QClipboard>
#include <unistd.h>


GTrans::GTrans() {

    //lzt QTextcode modify for Utf8
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings( QTextCodec::codecForLocale());

    //Layout
    QLayout *mainLayout = new QVBoxLayout;

    // The input section
//    QLayout *top = new QHBoxLayout;
//    QBoxLayout *tl = new QVBoxLayout;
//    QLabel *inLabel = new QLabel(tr("Input:"));
//    mainLayout->addWidget(inLabel);                  //lzt add
//   fromLang = new QComboBox;
//    tl->addWidget(inLabel);
//    tl->addSpacing(10);
//    tl->addWidget(fromLang);
//    tl->addStretch();
//    top->addItem(tl);
//    inputTxt = new QTextEdit;

//    top->addWidget(inputTxt);
//    mainLayout->addWidget(inputTxt);                //lzt add

    // The output section
//    QLayout *bottom = new QHBoxLayout;
//    QBoxLayout *bl = new QVBoxLayout;
//    QLabel *outLabel = new QLabel(tr("Output:"));
//    toLang = new QComboBox;
//    bl->addWidget(outLabel);
//    bl->addSpacing(10);
//    bl->addWidget(toLang);
//    bl->addStretch();
//    bottom->addItem(bl);
//    mainLayout->addWidget(outLabel);                //lzt add
    outputTxt = new QTextEdit;
    outputTxt->setReadOnly(true);
//    bottom->addWidget(outputTxt);
    mainLayout->addWidget(outputTxt);               //lzt add

//    mainLayout->addItem(top);
//    mainLayout->addItem(bottom);

    // Translate button
//    trans_b = new QPushButton(tr("Translate"));
//    mainLayout->addWidget(trans_b);

    setLayout(mainLayout);
    setWindowTitle(tr("Translate"));


//    trans_b->setDefault(true);
//    connect(trans_b, SIGNAL(clicked()), this, SLOT(doTrans()));

//lzt clip
    QClipboard *clipboard = QApplication::clipboard();
    connect(clipboard, SIGNAL(selectionChanged()), this, SLOT(doTrans()));
//

    // Setup foxus and tab order.
//    inputTxt->setTabChangesFocus(true);
//    inputTxt->setFocus(Qt::ActiveWindowFocusReason);
//    outputTxt->setFocusProxy(trans_b);
}

void GTrans::doTrans() {
    nam = new QNetworkAccessManager(this);

    // Figure out the URL for translate.google.com
//  QStringList words = inputTxt->toPlainText().split(QRegExp("\\s+"));

//lzt clipboard
    QClipboard *clipboard = QApplication::clipboard();
    QStringList words = clipboard->text(QClipboard::Selection).split(QRegExp("\\s+"));
//    inputTxt->setPlainText(words.join(" "));
//

    //QString gtUrl = tr("https://translate.google.com/translate_t?langpair=%1|%2&text=").arg(languages[fromLang->currentText()]).arg(languages[toLang->currentText()]) + words.join("+");
    QString gtUrl = tr("http://dict.youdao.com/search?q=") + words.join(" ");
//   outputTxt->setPlainText(gtUrl);
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
//    inputTxt->selectAll();
//    inputTxt->setFocus();
    usleep(500000);
    outputTxt->selectAll();
    outputTxt->setFocus();

}

void GTrans::finishedSlot(QNetworkReply* reply) {

    // The request finished, so check for an error
    if (reply->error() == QNetworkReply::NoError) {

//      outputTxt->setPlainText(tr(reply->readAll()));
//      outputTxt->setHtml(tr(reply->readAll()));
//lzt2
        // Find the translated string - this is a bit of a hack, but works and is easier than parsing the HTML...
//      QRegExp rx(tr("input type=hidden name=gtrans value=\"(.*)\"><div id=sug_exp>"));
        QRegExp rx(tr("<div id=\"phrsListTab\" class=\"trans-wrapper clearfix\">(.*)<div id=\"ads\" class=\"ads\">"));
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

    } else {
        outputTxt->setPlainText(tr("A network error occured."));
    }

    // Schedule old network stuff for deletion

    reply->deleteLater();
    nam->deleteLater();
}
