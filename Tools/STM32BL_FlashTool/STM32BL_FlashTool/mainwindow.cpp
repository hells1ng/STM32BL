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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    createActions();
    createMenus();
    (void)statusBar();

    setWindowFilePath(QString());
    transactionCount = 0;

    refreshSerial();

    ui->waitResponseSpinBox->setRange(0, 10000);
    ui->waitResponseSpinBox->setValue(3000);

    setWindowTitle(tr("Stm32BL_FlashTool"));


    /*thread signals and mainwindow slots*/
    connect(&thread, SIGNAL(response(int, QString, QString, QByteArray)),
            this, SLOT(showResponse(int, QString, QString, QByteArray)));
    connect(&thread, SIGNAL(response_fail(QString)),
            this, SLOT(showFail(QString)));
    connect(&thread, SIGNAL(error(QString)),
            this, SLOT(processError(QString)));
    connect(&thread, SIGNAL(timeout(QString)),
            this, SLOT(processTimeout(QString)));
    connect(&thread, SIGNAL(enableButtons()),
            this, SLOT(enableButtons()));

    /*transaction buttons*/
    connect(ui->resetButton, SIGNAL(clicked()),
            this, SLOT(transaction_reset()));
    connect(ui->eraseButton, SIGNAL(clicked()),
            this, SLOT(transaction_erase()));
    connect(ui->flashButton, SIGNAL(clicked()),
            this, SLOT(transaction_flash()));
    connect(ui->flashAllButton, SIGNAL(clicked()),
            this, SLOT(transaction_flashall()));
    connect(ui->gotoappButton, SIGNAL(clicked()),
            this, SLOT(transaction_gotoapp()));
    connect(ui->sendButton, SIGNAL(clicked()),
            this, SLOT(transaction_send()));
    connect(ui->readButton, SIGNAL(clicked()),
            this, SLOT(transaction_read()));
    connect(ui->writeButton, SIGNAL(clicked()),
            this, SLOT(transaction_write()));

    /*serial buttons*/
    connect(ui->connectButton, SIGNAL(clicked()),
            &thread, SLOT(connect()));
    connect(ui->disconnectButton, SIGNAL(clicked()),
            &thread, SLOT(disconnect()));
    connect(ui->refreshSerialButton, SIGNAL(clicked()),
            this, SLOT(refreshSerial()));

    /*clean textEdits button*/
    connect(ui->cleanButton, SIGNAL(clicked()),
            this, SLOT(on_cleanButton()));

    connect(ui->reloadHexButton, SIGNAL(clicked()),
            this, SLOT(on_reloadHexButton()));

    connect(ui->enableNetworkCheckBox, SIGNAL(clicked(bool)),
            this, SLOT(on_enableNetworkCheckBox(bool)));

    read_settings();
    create_addresses();

    ui->addressComboBox->addItems(ui->addressesLabel->text().split(',', QString::SkipEmptyParts));
    ui->addressComboBox->setFocus();
    ui->requestLineEdit->setText("aa aa 01 7f 00 00 d4");
    udpflag = MasterThread::UDP_CMD_NO_CMD;

}

/*
 * Read json file with settings ( path to hex, adresses of slaves)
 */
void MainWindow::read_settings()
{
    QString val;
    QFile file;
    file.setFileName("../STM32BL_FlashTool/settings.txt");

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Cannot open settings.txt"));
        return;
    }
    else
    {
        val = file.readAll();
        file.close();
        qWarning() << val;

        try {

            QJsonDocument   d = QJsonDocument::fromJson(val.toUtf8());
            QJsonObject     o = d.object();

            QJsonValue      hexFile = o.value(QString("hex"));
            QJsonObject     item1 = hexFile.toObject();
            QJsonValue      subobj = item1["path"];
            qWarning() << "HexPath = " << subobj.toString();

            QJsonValue      slaves = o.value(QString("slaves"));
            QJsonObject     item2 = slaves.toObject();
                            addressesJsonArray = item2["addresses"].toArray();
            qWarning() << "Addresses of Slaves = " << addressesJsonArray;

            fileName = subobj.toString();
            loadFile(fileName);

        }
        catch (...)
        {
            qDebug() << "Wrong settings.txt";
        }
    }
}
/*
 * Create addressesByteArray from addressesJsonArray exclude 0 address
 */
void MainWindow::create_addresses()
{
    QString qs;
    for (int i = 0; i < addressesJsonArray.size(); i++)
    {
        QString addr = addressesJsonArray[i].toString();
        qs = qs + addr + ",";
        if (addr.toInt() != 0)
            addressesByteArray.append(addr.toInt());
//        qDebug() << "addr = " << addressesJsonArray[i].toString().toInt();
    }

    ui->addressesLabel->setText(qs);
//    qDebug() << addressesByteArray.toHex();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cleanButton()
{
    ui->textEdit->clear();
    ui->failTextEdit->clear();
}
void MainWindow::on_reloadHexButton()
{
    ui->textEdit->clear();
    read_settings();
}
void MainWindow::refreshSerial(){
    ui->serialPortComboBox->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        ui->serialPortComboBox->addItem(info.portName());
}
void MainWindow::open()
{
    fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        loadFile(fileName);
    }
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Me"),
            tr("<b>Thanks</b> everybody! "
               "Created by A.Vylegzhanin :)"));
}

void MainWindow::createActions()
{
    //OPEN
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    //EXIT
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    //ABOUT
    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);

    separatorAct = fileMenu->addSeparator();

    fileMenu->addAction(exitAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}


void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Openning hex..."),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }
    else
    {
        if (!vector.isEmpty()) vector.clear();
        while(!file.atEnd())
        {
             //читаем строку
             QString str = file.readLine();
             //Берем строки с записью 00 (01,04,05 отсекаем)
             if (str.mid(7,2) == "00")
             {
                 //удаляем лишнее
                 str.remove(str.left(9));//:10C00000
                 str.remove(str.right(3));//ABCD/n
                 //разделяем строки на байты
                 for (int i = 2; i < str.size(); i = i + 3)
                 {
                     str.insert(i, "-");
                 }
                 //запись в вектор строк
                 vector.push_back(str);
             }
         }
    }

    for (int i = 0; i < vector.size(); i++)
    {
        ui->textEdit->append(vector[i]);
    }

    ui->label->setText(fileName);
    QApplication::restoreOverrideCursor();

    statusBar()->showMessage(tr("File loaded"), 2000);

}

void MainWindow::transaction_reset() //при нажатии на Reset
{
    setControlsEnabled(false);
    if (udpflag == MasterThread::UDP_CMD_NO_CMD)
        ui->statusLabel->setText(tr("Status: Reset STM32, connected to port %1.")
                             .arg(ui->serialPortComboBox->currentText()));
    else
        ui->statusLabel->setText(tr("Status: Reset STM32, connected to %1.")
                             .arg(thread.serverName));

    thread.transaction(ui->serialPortComboBox->currentText(),
                       ui->baudrateComboBox->currentText().toInt(),
                       ui->waitResponseSpinBox->value(),
                       MasterThread::BTL_CMD_RESTART,
                       //ui->addressComboBox->currentText().toInt(),
                       get_oneAddress(),
                       0,
                       udpflag);
}
void MainWindow::transaction_read()
{
    setControlsEnabled(false);
    if (udpflag == MasterThread::UDP_CMD_NO_CMD)
        ui->statusLabel->setText(tr("Status: Reading data from STM32, connected to port %1.")
                             .arg(ui->serialPortComboBox->currentText()));
    else
        ui->statusLabel->setText(tr("Status: Reading data from STM32, connected to %1.")
                             .arg(thread.serverName));

    QByteArray arr;
    QString arrstring = ui->pageEdit->text()+ "-" + ui->wordsEdit->text();

    foreach (const QString & num, arrstring.split('-')) {
            bool ok = false;
            signed int numInInt = num.toInt(&ok, 10);          // конвертируем из строкового представления
            if (numInInt > 255) numInInt = 255;
            if (numInInt < 0) numInInt = 0;
            arr.append(reinterpret_cast<char>(( char)numInInt));       // добавляем число в итоговый массив
        }
    //qDebug()<<arr.toHex();

    thread.transaction(ui->serialPortComboBox->currentText(),
                       ui->baudrateComboBox->currentText().toInt(),
                       ui->waitResponseSpinBox->value(),
                       MasterThread::BTL_CMD_READDATA,
                       //ui->addressComboBox->currentText().toInt(),
                       get_oneAddress(),
                       arr,
                       udpflag);
}
void MainWindow::transaction_write()
{
    setControlsEnabled(false);
     if (udpflag == MasterThread::UDP_CMD_NO_CMD)
         ui->statusLabel->setText(tr("Status: Writing data to STM32, connected to port %1.")
                              .arg(ui->serialPortComboBox->currentText()));
     else
         ui->statusLabel->setText(tr("Status: Writing data to STM32, connected to %1.")
                              .arg(thread.serverName));

     QByteArray arr;
     QString arrstring = ui->pageEdit->text()+ "-" + ui->RS485ADRdecEdit->text();

     foreach (const QString & num, arrstring.split('-')) {
             bool ok = false;
             signed int numInInt = num.toInt(&ok, 10);          // конвертируем из строкового представления
             if (numInInt > 255) numInInt = 255;
             if (numInInt < 0) numInInt = 0;
             arr.append(reinterpret_cast<char>(( char)numInInt));       // добавляем число в итоговый массив
         }
     //qDebug()<<arr.toHex();

     thread.transaction(ui->serialPortComboBox->currentText(),
                        ui->baudrateComboBox->currentText().toInt(),
                        ui->waitResponseSpinBox->value(),
                        MasterThread::BTL_CMD_WRITE_1REG,
                        //ui->addressComboBox->currentText().toInt(),
                        get_oneAddress(),
                        arr,
                        udpflag);

}

void MainWindow::transaction_erase() //при нажатии на Erase
{
    setControlsEnabled(false);
    if (udpflag == MasterThread::UDP_CMD_NO_CMD)
        ui->statusLabel->setText(tr("Status: Erase STM32, connected to port %1.")
                             .arg(ui->serialPortComboBox->currentText()));
    else
        ui->statusLabel->setText(tr("Status: Erase STM32, connected to %1.")
                             .arg(thread.serverName));

    thread.transaction(ui->serialPortComboBox->currentText(),
                       ui->baudrateComboBox->currentText().toInt(),
                       ui->waitResponseSpinBox->value(),
                       MasterThread::BTL_CMD_ERASE,
                       //ui->addressComboBox->currentText().toInt(),
                       get_oneAddress(),
                       0,
                       udpflag);
}
void MainWindow::transaction_flash() //при нажатии на Flash
{
    if (fileName.isEmpty())
    {
        QMessageBox::warning(this, tr("Open File!"),
                                       tr("Open Hex File, foool!"));
                                       //.arg("fileName")
                                       //.arg("file.errorString())"));
        return;
    }
    setControlsEnabled(false);

    if (udpflag == MasterThread::UDP_CMD_NO_CMD)
        ui->statusLabel->setText(tr("Status: Flash STM32, connected to port %1.")
                             .arg(ui->serialPortComboBox->currentText()));
    else
        ui->statusLabel->setText(tr("Status: Flash STM32, connected to %1.")
                             .arg(thread.serverName));

    /* get outArray */
    get_hexReqFromFile();

    //outArray - массив hex-прошивки

    thread.transaction(ui->serialPortComboBox->currentText(),
                       ui->baudrateComboBox->currentText().toInt(),
                       ui->waitResponseSpinBox->value(),
                       MasterThread::BTL_CMD_FLASH,
                       //ui->addressComboBox->currentText().toInt(),
                       get_oneAddress(),
                       outArray,
                       udpflag);

    //qDebug() << QString(outArray.toHex());
    outArray.clear();
}

void MainWindow::transaction_flashall() //при нажатии на Flash_All
{
    if (fileName.isEmpty())
    {
        QMessageBox::warning(this, tr("Open File!"),
                                       tr("Open Hex File, foool!"));
                                       //.arg("fileName")
                                       //.arg("file.errorString())"));
        return;
    }
    setControlsEnabled(false);

    if (udpflag == MasterThread::UDP_CMD_NO_CMD)
        ui->statusLabel->setText(tr("Status: Flash STM32 devices, connected to port %1.")
                             .arg(ui->serialPortComboBox->currentText()));
    else
        ui->statusLabel->setText(tr("Status: Flash STM32 devices, connected to %1.")
                             .arg(thread.serverName));

    /* get outArray */
    get_hexReqFromFile();

    //outArray - массив hex-прошивки

    thread.transaction(ui->serialPortComboBox->currentText(),
                       ui->baudrateComboBox->currentText().toInt(),
                       ui->waitResponseSpinBox->value(),
                       MasterThread::BTL_CMD_FLASH,
                       //ui->addressComboBox->currentText().toInt(),
                       //get_oneAddress(),
                       addressesByteArray,
                       outArray,
                       udpflag);

    //qDebug() << QString(outArray.toHex());
    outArray.clear();
}

void MainWindow::transaction_send() //при нажатии на Send
{
    setControlsEnabled(false);

    if (udpflag == MasterThread::UDP_CMD_NO_CMD)
        ui->statusLabel->setText(tr("Status: Running, connected to port %1.")
                             .arg(ui->serialPortComboBox->currentText()));
    else
        ui->statusLabel->setText(tr("Status: Running, connected to %1.")
                             .arg(thread.serverName));

    QByteArray arr;
    foreach (const QString & num, ui->requestLineEdit->text().split(' '))
    {
            bool ok = false;
            int numInInt = num.toInt(&ok, 16);          // конвертируем из строкового представления
            arr.append(reinterpret_cast<char>(( char)numInInt)); // добавляем число в итоговый массив
    }
    //qDebug()<<arr.toHex();

    thread.transaction(ui->serialPortComboBox->currentText(),
                       ui->baudrateComboBox->currentText().toInt(),
                       ui->waitResponseSpinBox->value(),
                       MasterThread::BTL_CMD_NO_CMD,
                       //ui->addressComboBox->currentText().toInt(),
                       get_oneAddress(),
                       //ui->requestLineEdit->text().toLocal8Bit());
                       arr,
                       udpflag);
}


void MainWindow::transaction_gotoapp() //при нажатии на GoToApp
{
    setControlsEnabled(false);

    if (udpflag == MasterThread::UDP_CMD_NO_CMD)
        ui->statusLabel->setText(tr("Status: GoToApp STM32, connected to port %1.")
                             .arg(ui->serialPortComboBox->currentText()));
    else
        ui->statusLabel->setText(tr("Status: GoToApp STM32, connected to %1.")
                             .arg(thread.serverName));

    thread.transaction(ui->serialPortComboBox->currentText(),
                       ui->baudrateComboBox->currentText().toInt(),
                       ui->waitResponseSpinBox->value(),
                       MasterThread::BTL_CMD_GOTOAPP,
                       //ui->addressComboBox->currentText().toInt(),
                       get_oneAddress(),
                       0,
                       udpflag);
}

void MainWindow::get_hexReqFromFile()
{
    for (int i = 0; i < vector.size(); i++)
    {
        foreach (const QString & num, vector[i].split('-'))
        {
                bool ok = false;
                int numInInt = num.toInt(&ok, 16);          // конвертируем из строкового представления
                //Q_ASSERT(ok);                               // ковертация прошла удачно
                //Q_ASSERT(numInInt >= 0 && numInInt <= 255); // итоговое число поместится в unsigned char
                outArray.append(reinterpret_cast<char>(( char)numInInt));       // добавляем число в итоговый массив
        }
    }
}

QByteArray MainWindow::get_oneAddress()
{
    QByteArray ret;
    ret[0] = ui->addressComboBox->currentText().toInt();
    return ret;
}


void MainWindow::showResponse(int CMD, const QString &req, const QString &s, QByteArray respBytes) //после response()
{
    if (CMD == MasterThread::BTL_CMD_READDATA)
    {
        for (int i = 0; i < respBytes.size(); i = i + 4)
        {
            QByteArray word = QByteArray(respBytes, i + 4);
            word.remove(0, i);
            switch (i) {
            case 0:
                ui->RS485ADRhexEdit->setText(QString(word.toHex()));
                break;
            default:
                break;
            }
        }
        ui->trafficLabel->setText(tr("Traffic, transaction #%1:"
                                 "\n\r-request: %2"
                                 "\n\r-response: %3")
                              .arg(++transactionCount).arg(req).arg(QString(respBytes.toHex())));

        ui->textEdit->append(tr("Traffic, transaction #%1:"
                                 "\n\r-request: %2"
                                 "\n\r-response: %3")
                              .arg(transactionCount).arg(req).arg(QString(respBytes.toHex())));
    }
    else if (CMD == MasterThread::BTL_CMD_NO_CMD)
    {
        ui->trafficLabel->setText(tr("Traffic, transaction #%1:"
                                 "\n\r-request: %2"
                                 "\n\r-response: %3")
                              .arg(++transactionCount).arg(req).arg(QString(respBytes.toHex())));

        ui->textEdit->append(tr("Traffic, transaction #%1:"
                                 "\n\r-request: %2"
                                 "\n\r-response: %3")
                              .arg(transactionCount).arg(req).arg(QString(respBytes.toHex())));
    }
    else
    {
        ui->trafficLabel->setText(tr("Traffic, transaction #%1:"
                                     "\n\r-request: %2"
                                     "\n\r-response: %3")
                                  .arg(++transactionCount).arg(req).arg(s));
        ui->textEdit->append(tr("Traffic, transaction #%1:"
                                 "\n\r-request: %2"
                                 "\n\r-response: %3")
                              .arg(transactionCount).arg(req).arg(QString(s)));
    }


    //qDebug() << qs;
}
void MainWindow::showFail(const QString &s) //после response()
{
    ui->failTextEdit->append(s);

}
void MainWindow::processError(const QString &s)
{
    setControlsEnabled(true);
    ui->statusLabel->setText(tr("Status: Not running, %1.").arg(s));
    ui->trafficLabel->setText(tr("No traffic."));
}

void MainWindow::processTimeout(const QString &s)
{
    //setControlsEnabled(true);
    ui->statusLabel->setText(tr("Status: Running, %1.").arg(s));
    ui->trafficLabel->setText(tr("No traffic."));
}

void MainWindow::setControlsEnabled(bool enable)
{
    ui->sendButton->setEnabled(enable);
    ui->resetButton->setEnabled(enable);
    ui->eraseButton->setEnabled(enable);
    ui->flashButton->setEnabled(enable);
    ui->flashAllButton->setEnabled(enable);
    ui->gotoappButton->setEnabled(enable);
    ui->serialPortComboBox->setEnabled(enable);
    ui->waitResponseSpinBox->setEnabled(enable);
    ui->baudrateComboBox->setEnabled(enable);
    ui->refreshSerialButton->setEnabled(enable);
    ui->readButton->setEnabled(enable);
    ui->writeButton->setEnabled(enable);
}

void MainWindow::on_enableNetworkCheckBox(bool enable)
{
    if (ui->enableNetworkCheckBox->isChecked())
        udpflag = MasterThread::UDP_CMD_USERPI;
    else
        udpflag = MasterThread::UDP_CMD_NO_CMD;

}
void MainWindow::enableButtons()
{
    setControlsEnabled(true);
}
