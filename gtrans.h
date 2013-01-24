// gtrans.h

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

#ifndef GTRANS_H
#define GTRANS_H

#include <QMap>
#include <QString>
#include <QWidget>

class QComboBox;
class QTextEdit;
class QPushButton;
class QNetworkReply;
class QNetworkAccessManager;

class GTrans : public QWidget {
    Q_OBJECT;

public:
    GTrans();

public slots:
    void doTrans();
    void finishedSlot(QNetworkReply* reply);

private:
    QTextEdit *inputTxt;
    QTextEdit *outputTxt;
    QComboBox *toLang;
    QComboBox *fromLang;

    QPushButton *trans_b;
    QMap<QString, QString> languages;

    void fillInLanguages();
    QNetworkAccessManager* nam;
};

#endif
