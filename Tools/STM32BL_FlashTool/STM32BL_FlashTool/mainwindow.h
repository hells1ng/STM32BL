/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QDialog>
#include <QMainWindow>
#include <QtWidgets>
#include <QtSerialPort/QSerialPortInfo>
#include <QJsonObject>

#include "masterthread.h"

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QTextEdit;
class QLabel;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QComboBox;

namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void open();            // File->Open button
    void about();           // About button
    void refreshSerial();   // Refresh button

    void transaction_reset();
    void transaction_erase();
    void transaction_flash();
    void transaction_flashall();
    void transaction_gotoapp();
    void transaction_send();
    void transaction_read();
    void transaction_write();

    void showResponse(int CMD, const QString &req, const QString &s, QByteArray respBytes);
    void showFail(const QString &s);
    void processError(const QString &s);
    void processTimeout(const QString &s);
    void on_cleanButton();
    void on_reloadHexButton();
    void on_enableNetworkCheckBox(bool enable);

    void enableButtons();

private:
    Ui::MainWindow *ui;
    void createActions();
    void createMenus();
    void loadFile(const QString &fileName);

    void read_settings();
    void create_addresses();
    QByteArray get_oneAddress();
    void get_hexReqFromFile();
    void setControlsEnabled(bool enable);

    QMenu *fileMenu;
    QMenu *helpMenu;
    QAction *openAct;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *separatorAct;

    QTextStream out; //input hex and output bin
    QVector <QString> vector;
    QByteArray outArray;

    QString fileName;
    int transactionCount;
    int udpflag;

    QByteArray      addressesByteArray;

    QJsonArray      addressesJsonArray;

    MasterThread thread;
};

#endif


