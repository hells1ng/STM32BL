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

#include <QTimer>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    timer = new QTimer();

    createActions();
    createMenus();
    (void)statusBar();

    setWindowFilePath(QString());
    transactionCount = 0;

    refreshSerial();

    ui->waitResponseSpinBox->setRange(0, 10000);
    ui->waitResponseSpinBox->setValue(3000);

    setWindowTitle(tr("Stm32BL_FlashTool"));
    ui->serialPortComboBox->setFocus();


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
    connect(ui->writeAllButton, SIGNAL(clicked()),
            this, SLOT(transaction_writeall()));

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

    open_hex();
    create_addresses();

    ui->addressComboBox->addItems(ui->addressesLabel->text().split(','));
    ui->addressComboBox->setFocus();
    ui->requestLineEdit->setText("aa aa 01 7f 00 00 d4");
    udpflag = MasterThread::UDP_CMD_NO_CMD;

}
void MainWindow::create_addresses()
{
    //ui->addressesLabel заполнили в open_hex()
    foreach (const QString & num, ui->addressesLabel->text().split(','))
    {
            bool ok = false;
            int numInInt = num.toInt(&ok, 16);          // конвертируем из строкового представления
            if (numInInt != 0)
                addressesByteArray.append(reinterpret_cast<char>(( char)numInInt)); // добавляем число в итоговый массив, не включая 0-й (общий для всех)
    }
    //qDebug() << addressesByteArray.toHex();
}

MainWindow::~MainWindow()
{
    timer->stop();
    delete timer;
    delete ui;
}
void MainWindow::newFile()
{
    MainWindow *other = new MainWindow;
    other->show();
}
void MainWindow::on_cleanButton()
{
    ui->textEdit->clear();
    ui->failTextEdit->clear();
}
void MainWindow::on_reloadHexButton()
{
    ui->textEdit->clear();
    open_hex();
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
         //statusBar()->showMessage(fileName, 3000);
    }
}

void MainWindow::save()
{
    if (curFile.isEmpty())
        saveAs();
    else
        saveFile(curFile);
}

void MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return;

    saveFile(fileName);
}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        loadFile(action->data().toString());
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Me"),
            tr("<b>Thanks</b> everybody! "
               "Created by A.Vylegzhanin :)"));
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    //fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    //fileMenu->addAction(saveAct);
    //fileMenu->addAction(saveAsAct);
    separatorAct = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActs[i]);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    updateRecentFileActions();

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
//    helpMenu->addAction(aboutQtAct);
}
void MainWindow::open_hex()
{
    QString str1, str2;
    QFile configFile;
    configFile.setFileName("../STM32BL_FlashTool/config.txt");

    if (!configFile.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Cannot open config.txt"));
        return;
    }
    else
    {
        while(!configFile.atEnd())
        {
             //читаем 1 строку с адресом hex-файла
             str1 = configFile.readLine();
             qDebug() << str1;
             str1.remove(str1.right(1));//n
             //читаем 2 строку с адресами слейвов
             str2 = configFile.readLine();
             str1.remove(str2.right(1));//n
             qDebug() << str2;
             break;
        }
     fileName = str1;
     loadFile(fileName);
     setCurrentFile(fileName);

     ui->addressesLabel->setText(str2);
    }

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
            QJsonArray      addresses = item2["addresses"].toArray();
            qWarning() << "Addresses of Slaves = " << addresses;
    //        /* in case of array get array and convert into string*/
    //        qWarning() << tr("QJsonObject[appName] of value: ") << item["imp"];
    //        QJsonArray test = item["imp"].toArray();
    //        qWarning() << test[1].toString();
            fileName = subobj.toString();
            loadFile(fileName);
            setCurrentFile(fileName);

            ui->addressesLabel->setText(str2);

        }
        catch (...)
        {
            qDebug() << "Wrong settings.txt";
        }
    }

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

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);

}

void MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Recent Files"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << textEdit->toPlainText();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowFilePath(curFile);

    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setValue("recentFileList", files);

    //не работает сохранение последних открытых файлов
    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin)
            mainWin->updateRecentFileActions();
    }

//    updateRecentFileActions();
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

    separatorAct->setVisible(numRecentFiles > 0);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
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
void MainWindow::transaction_writeall()
{

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
    //ui->addressComboBox->setEnabled(enable);
    ui->serialPortComboBox->setEnabled(enable);
    ui->waitResponseSpinBox->setEnabled(enable);
    //ui->requestLineEdit->setEnabled(enable);
    ui->baudrateComboBox->setEnabled(enable);
    ui->refreshSerialButton->setEnabled(enable);
    ui->readButton->setEnabled(enable);
    ui->writeButton->setEnabled(enable);
    ui->writeAllButton->setEnabled(enable);
}

void MainWindow::on_enableNetworkCheckBox(bool enable)
{
    //пока оставим так))
//    ui->serialPortComboBox->setEnabled(!enable);
//    ui->baudrateComboBox->setEnabled(!enable);
//    ui->refreshSerialButton->setEnabled(!enable);
//    ui->serverAddressLineEdit->setEnabled(enable);
//    ui->serverPortLineEdit->setEnabled(enable);

    if (ui->enableNetworkCheckBox->isChecked())
        udpflag = MasterThread::UDP_CMD_USERPI;
    else
        udpflag = MasterThread::UDP_CMD_NO_CMD;

}
void MainWindow::enableButtons()
{
    setControlsEnabled(true);
}
