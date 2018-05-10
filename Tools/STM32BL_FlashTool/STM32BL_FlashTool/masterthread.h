

#ifndef MASTERTHREAD_H
#define MASTERTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QtSerialPort/QSerialPort>
#include <QtNetwork>

//! [0]
class MasterThread : public QThread
{
    Q_OBJECT

public:
    MasterThread(QObject *parent = 0);
    ~MasterThread();

    void transaction(const QString &portName, int baudrate,
                     int waitTimeout,
                     int cmd,
                    // int addr,
                     QByteArray addr,
                     const QByteArray &request,
                     int udpflag);
    void run();
    enum {
         BTL_CMD_NO_CMD         = 0xff,
         BTL_CMD_RESTART        = 0x80,
         BTL_CMD_ERASE          = 0x32,
         BTL_CMD_FLASH          = 0x34,
         BTL_CMD_GOTOAPP        = 0x35,
         BTL_CMD_READDATA		= 0x36,
         BTL_CMD_FLASHCRC		= 0x37,
         BTL_CMD_WRITE_1REG		= 0x38, //now we write only rs485 addr reg
         BTL_CMD_REQUEST		= 0x00,
         BTL_BUFF_SIZE          = 1024,
         BTL_TIMEOUT            = 50,
         BTL_TIMEOUT_READDATA   = 500,
         BTL_START_TIMEOUT      = 2000,
         BTL_ADD_NETWORK_TIMEOUT= 100,
         RESPONSE_OK            = 0,
         RESPONSE_ERROR         = 1,
         UDP_CMD_NO_CMD         = 0,
         UDP_CMD_USERPI         = 1,
         UDP_CMD_RPION          = 2,
         UDP_CMD_RPIOFF         = 3
    };
    QString serverName;

signals:
    void response(int CMD, const QString &req, const QString &res, QByteArray respbytes);
    void response_fail(const QString &fail);
    void error(const QString &s);
    void timeout(const QString &s);
    void enableButtons();

private slots:
    void connect();
    void disconnect();

private:
    QSerialPort serial;
    QString portName;
    QByteArray request;
    int waitTimeout;
    int baud;
    int currentCmd;
    int currentAddress;
    QByteArray currentAddresses;
    QMutex mutex;
    QWaitCondition cond;
    bool quit;
    int udpFlag;
    void serial_response();
    QByteArray create_request(int cmd, QByteArray* arr);
    int send_request(QByteArray req, int CMD);
    int checkResponse(QString resp, int CMD, QByteArray resparray);
    void Flash();
    QString errorString;
    QTcpSocket socket;
    quint16 serverPort;

};
//! [0]

#endif // MASTERTHREAD_H
