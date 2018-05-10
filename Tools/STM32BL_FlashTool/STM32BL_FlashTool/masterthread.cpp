/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "masterthread.h"

//#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QTime>
#include <QtNetwork>

QT_USE_NAMESPACE

MasterThread::MasterThread(QObject *parent)
    : QThread(parent), waitTimeout(0), quit(false)
{
    errorString = "ERROR";
    serverName = "192.168.0.34";
    serverPort = 7777;
}

//! [0]
void MasterThread::connect()
{
    if (!serial.open(QIODevice::ReadWrite)) {
        emit error(tr("Can't open %1, error code %2")
                   .arg(portName).arg(serial.error()));
        return;
    }
}
void MasterThread::disconnect()
{
    if (serial.isOpen())
    {
        serial.close();
        emit timeout(tr("Disconnect from port:  %1")
                     .arg(serial.portName()));

    }
}

//! [0]
//! [0]
MasterThread::~MasterThread()
{
    mutex.lock();
    quit = true;
    cond.wakeOne();
    mutex.unlock();
    wait();
}
//! [0]

//! [1] //! [2]
void MasterThread::transaction(const QString &portName,
                               int baudrate,
                               int waitTimeout,
                               int cmd,
                               //int addr,
                               QByteArray addr,
                               const QByteArray &request,
                               int udpflag)
{
    //! [1]
    QMutexLocker locker(&mutex);
    this->portName = portName;
    this->waitTimeout = waitTimeout;
    this->request = request;
    this->baud = baudrate;
    this->currentCmd = cmd;
   // this->currentAddress = addr;
    this->currentAddresses = addr;
    this->udpFlag = udpflag;
    //! [3]
    if (!isRunning())
        start();
    else
        cond.wakeOne();
}
//! [2] //! [3]

void MasterThread::run()
{
    bool currentPortNameChanged = false;

    mutex.lock();
    //! [4] //! [5]
    QString currentPortName;
    if (udpFlag == UDP_CMD_NO_CMD)
    {
        if (currentPortName != portName) {
            currentPortName = portName;
            currentPortNameChanged = true;
        }
    }
    QByteArray currentRequest = request;
    mutex.unlock();
    //! [5] //! [6]

    while (!quit) {

        if (udpFlag == UDP_CMD_NO_CMD)
        {
            if (serial.baudRate() != baud && serial.isOpen())
                serial.setBaudRate(baud);
            //![6] //! [7]
            if (!serial.isOpen()) serial.open(QIODevice::ReadWrite);
            if (currentPortNameChanged) {
                serial.close();
                serial.setPortName(currentPortName);
                serial.setBaudRate(baud);

                if (!serial.open(QIODevice::ReadWrite)) {
                    emit error(tr("Can't open %1, error code %2")
                               .arg(portName).arg(serial.error()));
                    return;
                }
            }
            //serial.clear(QSerialPort::AllDirections);
        }
        else
        {
            const int Timeout = 1 * 1000;
            socket.connectToHost(serverName, serverPort);
            if (!socket.waitForConnected(Timeout)) {
                emit error(tr("Can't connect to %1, error code %2")
                           .arg(serverName).arg(socket.error()));
                return;
            }
        }

        if (currentCmd == BTL_CMD_FLASH)
        {
            Flash();
        }
        else if (currentCmd == BTL_CMD_RESTART)
        {
            currentAddress = currentAddresses[0];
            QByteArray cmd_reset = create_request(BTL_CMD_RESTART, 0);
            send_request(cmd_reset, BTL_CMD_RESTART);
        }
        else if (currentCmd == BTL_CMD_ERASE)
        {
            currentAddress = currentAddresses[0];
            QByteArray cmd_clear = create_request(BTL_CMD_ERASE, 0);//erase!
            send_request(cmd_clear, BTL_CMD_ERASE);
        }
        else if (currentCmd == BTL_CMD_NO_CMD)
        {
            QByteArray request(currentRequest);
            send_request(request, BTL_CMD_NO_CMD);
        }
        else if (currentCmd == BTL_CMD_GOTOAPP)
        {
            currentAddress = currentAddresses[0];
            QByteArray cmd_gotoapp = create_request(BTL_CMD_GOTOAPP, 0);//gotoapp!
            send_request(cmd_gotoapp, BTL_CMD_GOTOAPP);
        }
        else if (currentCmd == BTL_CMD_REQUEST)
        {
            currentAddress = currentAddresses[0];
            QByteArray cmd_request = create_request(BTL_CMD_REQUEST, &request);//request!
            send_request(cmd_request, BTL_CMD_REQUEST);
        }
        else if (currentCmd == BTL_CMD_READDATA)
        {
            currentAddress = currentAddresses[0];
            QByteArray cmd_readdata = create_request(BTL_CMD_READDATA, &request);//readdata!
            send_request(cmd_readdata, BTL_CMD_READDATA);
        }
        else if (currentCmd == BTL_CMD_WRITE_1REG)
        {
            currentAddress = currentAddresses[0];
            QByteArray cmd_writedata = create_request(BTL_CMD_WRITE_1REG, &request);//readdata!
            send_request(cmd_writedata, BTL_CMD_WRITE_1REG);
        }
        emit enableButtons();

        //! [9]  //! [13]
        mutex.lock();
        cond.wait(&mutex);
        if (udpFlag == UDP_CMD_NO_CMD)
        {
            if (currentPortName != portName) {
                currentPortName = portName;
                currentPortNameChanged = true;
            } else {
                currentPortNameChanged = false;
            }
        }
        currentRequest = request;
        mutex.unlock();
    }
    //! [13]
}


//! [4]
QByteArray MasterThread::create_request(int cmd, QByteArray* arr)
{
    QByteArray req;
    unsigned char crc = 0xBB;
    req[0] = 0xBB;
    req[1] = 0xBB;
    req[2] = currentAddress;
    req[3] = cmd;

    if (cmd == BTL_CMD_FLASH)
    {
        req.append(*arr);
    }
    else if (cmd == BTL_CMD_READDATA)
    {
        req.append(*arr);
    }
    else if (cmd == BTL_CMD_WRITE_1REG)
    {
        req.append(*arr);
    }
    else if (cmd == BTL_CMD_REQUEST)
    {
        req.append(*arr);
    }
    else
    {
        req[4] = 0x00;
        req[5] = 0x00;
    }
    for (int j = 1; j < req.size(); j++)
    {
        //crc = crc + req[j];
        crc ^= req[j];
    }

    req[req.size()] = crc;
    return req;
}

int MasterThread::send_request(QByteArray req, int CMD)
{
    //QThread::msleep(100);
    //ЗДЕСЬ УЖАС с повторением одинакового кода в if - serial в else - socket ))
    if (udpFlag == UDP_CMD_NO_CMD)
    {
        serial.clear(QSerialPort::Input);
        serial.write(req);

        qDebug() << req.toHex();

        int currentTimeout = waitTimeout;
        if (CMD == BTL_CMD_RESTART)
            currentTimeout = BTL_START_TIMEOUT;

        if (serial.waitForBytesWritten(currentTimeout)) {
            QByteArray addr;
            addr[0] = currentAddress;
            QString failstr;
            switch (CMD)
            {
                case BTL_CMD_RESTART:
                    failstr = "RESET";
                    break;
                case BTL_CMD_ERASE:
                    failstr = "ERASE";
                    break;
                case BTL_CMD_FLASH:
                    failstr = "FLASH";
                    break;
                case BTL_CMD_GOTOAPP:
                    failstr = "GOTOAPP";
                    break;
                case BTL_CMD_READDATA:
                    failstr = "READDATA";
                    break;
                case BTL_CMD_WRITE_1REG:
                    failstr = "WRITEDATA";
                    break;
                case BTL_CMD_FLASHCRC:
                    failstr = "FLASHCRC";
                    break;
                case BTL_CMD_REQUEST:
                    failstr = "REQUEST";
                    break;
                case BTL_CMD_NO_CMD:
                    failstr = "?";
                    addr[0] = 0x00;
                    break;
                default:
                     failstr = "?";
                    break;
            }

            //! [8] //! [10]
            // read response
            if (serial.waitForReadyRead(currentTimeout))//Если получен ответ в течение зад.времени
            {
                QByteArray responseData = serial.readAll();
                if (CMD == BTL_CMD_READDATA)
                {
                    while (serial.waitForReadyRead(BTL_TIMEOUT_READDATA))
                        responseData += serial.readAll();
                }
                else
                {
                    while (serial.waitForReadyRead(BTL_TIMEOUT))
                        responseData += serial.readAll();
                }

                /*Response String */
                QString responseString(responseData);
                //QString responseString(responseData.toHex());
                QString requestString(req.toHex());
                //! [12]
                if (checkResponse(responseString, CMD, responseData) == RESPONSE_ERROR)//если ответ не верен
                {
                    emit this->response_fail(QString (addr.toHex()) + ":" + failstr + ":Response FAIL");
                    emit this->response(CMD, requestString, responseString, responseData);
                    return RESPONSE_ERROR;//если неверный ответ
                }
                else//а если верен
                    emit this->response(CMD, requestString, responseString, responseData);
                //! [10] //! [11] //! [12]

            }
            else    //а если ничего не пришло
            {
                emit this->response_fail(QString (addr.toHex()) + ":" + failstr + ":Timeout FAIL");
                emit timeout(tr("Wait read response timeout %1")
                             .arg(QTime::currentTime().toString()));
                return RESPONSE_ERROR;//если нет ответа в течение таймаута
            }
            //! [9] //! [11]
        } else {
            emit timeout(tr("Wait write request timeout %1")
                         .arg(QTime::currentTime().toString()));
        }
        return RESPONSE_OK;
    }
    else
    {
        socket.write(req);

        qDebug() << req.toHex();

        int currentTimeout = waitTimeout;
        if (CMD == BTL_CMD_RESTART)
            currentTimeout = BTL_START_TIMEOUT + BTL_ADD_NETWORK_TIMEOUT;

        if (socket.waitForBytesWritten(currentTimeout)) {
            QByteArray addr;
            addr[0] = currentAddress;
            QString failstr;
            switch (CMD)
            {
                case BTL_CMD_RESTART:
                    failstr = "RESET";
                    break;
                case BTL_CMD_ERASE:
                    failstr = "ERASE";
                    break;
                case BTL_CMD_FLASH:
                    failstr = "FLASH";
                    break;
                case BTL_CMD_GOTOAPP:
                    failstr = "GOTOAPP";
                    break;
                case BTL_CMD_READDATA:
                    failstr = "READDATA";
                    break;
                case BTL_CMD_WRITE_1REG:
                    failstr = "WRITEDATA";
                    break;
                case BTL_CMD_FLASHCRC:
                    failstr = "FLASHCRC";
                    break;
                case BTL_CMD_REQUEST:
                    failstr = "REQUEST";
                    break;
                case BTL_CMD_NO_CMD:
                    failstr = "?";
                    addr[0] = 0x00;
                    break;
                default:
                     failstr = "?";
                    break;
            }

            //! [8] //! [10]
            // read response
            if (socket.waitForReadyRead(currentTimeout))//Если получен ответ в течение зад.времени
            {
                QByteArray responseData = socket.readAll();
                if (CMD == BTL_CMD_READDATA)
                {
                    while (socket.waitForReadyRead(BTL_TIMEOUT_READDATA + BTL_ADD_NETWORK_TIMEOUT))
                        responseData += socket.readAll();
                }
                else
                {
                    while (socket.waitForReadyRead(BTL_TIMEOUT + BTL_ADD_NETWORK_TIMEOUT))
                        responseData += socket.readAll();
                }

                /*Response String */
                QString responseString(responseData);
                //QString responseString(responseData.toHex());
                QString requestString(req.toHex());
                //! [12]
                if (checkResponse(responseString, CMD, responseData) == RESPONSE_ERROR)//если ответ не верен
                {
                    emit this->response_fail(QString (addr.toHex()) + ":" + failstr + ":Response FAIL");
                    emit this->response(CMD, requestString, responseString, responseData);
                    return RESPONSE_ERROR;//если неверный ответ
                }
                else//а если верен
                    emit this->response(CMD, requestString, responseString, responseData);
                //! [10] //! [11] //! [12]

            }
            else    //а если ничего не пришло
            {
                emit this->response_fail(QString (addr.toHex()) + ":" + failstr + ":Timeout FAIL");
                emit timeout(tr("Wait read response timeout %1")
                             .arg(QTime::currentTime().toString()));
                return RESPONSE_ERROR;//если нет ответа в течение таймаута
            }
            //! [9] //! [11]
        } else {
            emit timeout(tr("Wait write request timeout %1")
                         .arg(QTime::currentTime().toString()));
        }
        return RESPONSE_OK;
    }


}


int MasterThread::checkResponse(QString resp, int CMD, QByteArray resparray)
{
    switch (CMD) {
    case (BTL_CMD_REQUEST):
    {
        if (resparray[0] != (char)0xAA)
            return RESPONSE_ERROR;
        else if (resparray[1] != (char)currentAddress)
            return RESPONSE_ERROR;
        else
        {
            char crc = 0x00;

            for (int j = 0; j < resparray.size()-1; j++)
            {
                //crc = crc + resparray[j];
                crc ^= resparray[j];
            }
            if (resparray[resparray.size()-1] != (char)crc)
                return RESPONSE_ERROR;
            else
                return RESPONSE_OK;
        }
        break;
    }
    case (BTL_CMD_NO_CMD):
        return RESPONSE_OK;
    case (BTL_CMD_READDATA):
        return RESPONSE_OK;
    default:
    {
        if (!resp.contains("OK"))
        //if (!resp.contains("4f4b"))
        {
            //qDebug() << QString("RESPONSE_ERROR");
            return RESPONSE_ERROR;

        }
        else
        {
            //qDebug() << QString("RESPONSE_OK");
            return RESPONSE_OK;
        }

        break;
    }
    }
}


void MasterThread::Flash()
{
    QByteArray currentRequest;
    for (int j  = 0; j < currentAddresses.size(); j++)
/**/{
        currentAddress = currentAddresses[j];
        currentRequest = request;

        QThread::msleep(1000);

        QByteArray cmd_reset = create_request(BTL_CMD_RESTART, 0);

        if (send_request(cmd_reset, BTL_CMD_RESTART) == RESPONSE_ERROR)
        {
            //error
        }
        else
        {
            QByteArray cmd_clear = create_request(BTL_CMD_ERASE, 0);//erase!

            if (send_request(cmd_clear, BTL_CMD_ERASE) == RESPONSE_ERROR)
            {
                //error
            }
            else
            {
                int bufsize = BTL_BUFF_SIZE;
                bool needClean = false;

                while (!currentRequest.isEmpty())
                {
                    if (currentRequest.size() < bufsize) bufsize = currentRequest.size();

                    QByteArray requestData(currentRequest, bufsize);// байты для записи постранично

                    QByteArray cmd_flash = create_request(BTL_CMD_FLASH, &requestData);

                    if (send_request(cmd_flash, BTL_CMD_FLASH) == RESPONSE_ERROR)
                        needClean = true;

                    requestData.clear();
                    currentRequest.remove(0, bufsize);

                    if (needClean) break;
                }
                if (needClean)//если запись не удалась - очищаем
                {
                    QByteArray cmd_clear = create_request(BTL_CMD_ERASE, 0);//erase!
                    if (send_request(cmd_clear, BTL_CMD_ERASE) == RESPONSE_OK)
                    {
                        emit this->response_fail(QString("Erased!"));
                    }
                }
                else//если запись удалась
                {
                    QByteArray cmd_flashcrc = create_request(BTL_CMD_FLASHCRC, 0);//flash crc
                    if (send_request(cmd_flashcrc, BTL_CMD_FLASHCRC) == RESPONSE_ERROR)//если не удалось записать CRC
                    {
                        QByteArray cmd_clear = create_request(BTL_CMD_ERASE, 0);
                        if (send_request(cmd_clear, BTL_CMD_ERASE) == RESPONSE_OK)//очищаем!
                        {
                            emit this->response_fail(QString("Erased!"));
                        }
                    }
                    else
                    {   //если удалось записать CRC - reset and gotoapp!
                        QByteArray cmd_gotoapp = create_request(BTL_CMD_GOTOAPP, 0);//reset and gotoapp!
                        if (send_request(cmd_gotoapp, BTL_CMD_GOTOAPP) == RESPONSE_ERROR)
                        {
                            emit this->response_fail(QString("Fail goToApp"));
                        }
                    }
                }
            }
        }
/**/}
}
void MasterThread::serial_response()
{

}

