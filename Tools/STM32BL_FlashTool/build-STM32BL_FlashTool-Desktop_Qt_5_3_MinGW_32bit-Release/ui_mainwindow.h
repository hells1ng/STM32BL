/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionExit;
    QAction *actionThanks;
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QComboBox *addressComboBox;
    QPushButton *resetButton;
    QPushButton *eraseButton;
    QPushButton *flashButton;
    QPushButton *gotoappButton;
    QLabel *label;
    QTextEdit *textEdit;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *serialPortLabel;
    QComboBox *serialPortComboBox;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QSpinBox *waitResponseSpinBox;
    QPushButton *refreshSerialButton;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_4;
    QComboBox *baudrateComboBox;
    QPushButton *sendButton;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *requestLabel;
    QLineEdit *requestLineEdit;
    QLabel *statusLabel;
    QLabel *trafficLabel;
    QFrame *line;
    QFrame *line_2;
    QPushButton *cleanButton;
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QLabel *addressesLabel2;
    QPushButton *flashAllButton;
    QLabel *addressesLabel;
    QTextEdit *failTextEdit;
    QPushButton *reloadHexButton;
    QPushButton *readButton;
    QPushButton *writeButton;
    QLabel *serialPortLabel_2;
    QLineEdit *pageEdit;
    QLabel *serialPortLabel_3;
    QLineEdit *wordsEdit;
    QLabel *serialPortLabel_4;
    QPushButton *writeAllButton;
    QLabel *serialPortLabel_7;
    QLineEdit *RS485ADRhexEdit;
    QFrame *line_8;
    QCheckBox *enableNetworkCheckBox;
    QFrame *line_10;
    QFrame *line_11;
    QLabel *serialPortLabel_5;
    QLineEdit *RS485ADRdecEdit;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(885, 528);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionThanks = new QAction(MainWindow);
        actionThanks->setObjectName(QStringLiteral("actionThanks"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 82, 157));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout->addWidget(label_2);

        addressComboBox = new QComboBox(verticalLayoutWidget);
        addressComboBox->setObjectName(QStringLiteral("addressComboBox"));
        addressComboBox->setEnabled(true);
        addressComboBox->setSizeIncrement(QSize(0, 0));
        addressComboBox->setBaseSize(QSize(0, 0));

        verticalLayout->addWidget(addressComboBox);

        resetButton = new QPushButton(verticalLayoutWidget);
        resetButton->setObjectName(QStringLiteral("resetButton"));

        verticalLayout->addWidget(resetButton);

        eraseButton = new QPushButton(verticalLayoutWidget);
        eraseButton->setObjectName(QStringLiteral("eraseButton"));

        verticalLayout->addWidget(eraseButton);

        flashButton = new QPushButton(verticalLayoutWidget);
        flashButton->setObjectName(QStringLiteral("flashButton"));

        verticalLayout->addWidget(flashButton);

        gotoappButton = new QPushButton(verticalLayoutWidget);
        gotoappButton->setObjectName(QStringLiteral("gotoappButton"));

        verticalLayout->addWidget(gotoappButton);

        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 410, 681, 16));
        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(490, 0, 391, 281));
        textEdit->setMouseTracking(true);
        verticalLayoutWidget_2 = new QWidget(centralWidget);
        verticalLayoutWidget_2->setObjectName(QStringLiteral("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(280, 0, 81, 47));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        serialPortLabel = new QLabel(verticalLayoutWidget_2);
        serialPortLabel->setObjectName(QStringLiteral("serialPortLabel"));

        verticalLayout_2->addWidget(serialPortLabel);

        serialPortComboBox = new QComboBox(verticalLayoutWidget_2);
        serialPortComboBox->setObjectName(QStringLiteral("serialPortComboBox"));

        verticalLayout_2->addWidget(serialPortComboBox);

        horizontalLayoutWidget_2 = new QWidget(centralWidget);
        horizontalLayoutWidget_2->setObjectName(QStringLiteral("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(280, 140, 151, 31));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(horizontalLayoutWidget_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_2->addWidget(label_3);

        waitResponseSpinBox = new QSpinBox(horizontalLayoutWidget_2);
        waitResponseSpinBox->setObjectName(QStringLiteral("waitResponseSpinBox"));

        horizontalLayout_2->addWidget(waitResponseSpinBox);

        refreshSerialButton = new QPushButton(centralWidget);
        refreshSerialButton->setObjectName(QStringLiteral("refreshSerialButton"));
        refreshSerialButton->setGeometry(QRect(360, 0, 71, 23));
        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(280, 50, 151, 25));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(horizontalLayoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout->addWidget(label_4);

        baudrateComboBox = new QComboBox(horizontalLayoutWidget);
        baudrateComboBox->setObjectName(QStringLiteral("baudrateComboBox"));

        horizontalLayout->addWidget(baudrateComboBox);

        sendButton = new QPushButton(centralWidget);
        sendButton->setObjectName(QStringLiteral("sendButton"));
        sendButton->setGeometry(QRect(340, 220, 75, 23));
        verticalLayoutWidget_3 = new QWidget(centralWidget);
        verticalLayoutWidget_3->setObjectName(QStringLiteral("verticalLayoutWidget_3"));
        verticalLayoutWidget_3->setGeometry(QRect(280, 170, 191, 43));
        verticalLayout_3 = new QVBoxLayout(verticalLayoutWidget_3);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        requestLabel = new QLabel(verticalLayoutWidget_3);
        requestLabel->setObjectName(QStringLiteral("requestLabel"));

        verticalLayout_3->addWidget(requestLabel);

        requestLineEdit = new QLineEdit(verticalLayoutWidget_3);
        requestLineEdit->setObjectName(QStringLiteral("requestLineEdit"));

        verticalLayout_3->addWidget(requestLineEdit);

        statusLabel = new QLabel(centralWidget);
        statusLabel->setObjectName(QStringLiteral("statusLabel"));
        statusLabel->setGeometry(QRect(10, 230, 411, 16));
        trafficLabel = new QLabel(centralWidget);
        trafficLabel->setObjectName(QStringLiteral("trafficLabel"));
        trafficLabel->setGeometry(QRect(10, 290, 451, 111));
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(260, 0, 20, 161));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(0, 150, 271, 16));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        cleanButton = new QPushButton(centralWidget);
        cleanButton->setObjectName(QStringLiteral("cleanButton"));
        cleanButton->setGeometry(QRect(494, 290, 381, 23));
        connectButton = new QPushButton(centralWidget);
        connectButton->setObjectName(QStringLiteral("connectButton"));
        connectButton->setEnabled(false);
        connectButton->setGeometry(QRect(440, 0, 51, 23));
        connectButton->setAutoFillBackground(false);
        disconnectButton = new QPushButton(centralWidget);
        disconnectButton->setObjectName(QStringLiteral("disconnectButton"));
        disconnectButton->setGeometry(QRect(360, 20, 71, 23));
        addressesLabel2 = new QLabel(centralWidget);
        addressesLabel2->setObjectName(QStringLiteral("addressesLabel2"));
        addressesLabel2->setGeometry(QRect(0, 161, 109, 13));
        flashAllButton = new QPushButton(centralWidget);
        flashAllButton->setObjectName(QStringLiteral("flashAllButton"));
        flashAllButton->setGeometry(QRect(0, 200, 82, 23));
        addressesLabel = new QLabel(centralWidget);
        addressesLabel->setObjectName(QStringLiteral("addressesLabel"));
        addressesLabel->setGeometry(QRect(0, 180, 261, 21));
        failTextEdit = new QTextEdit(centralWidget);
        failTextEdit->setObjectName(QStringLiteral("failTextEdit"));
        failTextEdit->setGeometry(QRect(490, 320, 391, 81));
        reloadHexButton = new QPushButton(centralWidget);
        reloadHexButton->setObjectName(QStringLiteral("reloadHexButton"));
        reloadHexButton->setGeometry(QRect(0, 430, 61, 23));
        readButton = new QPushButton(centralWidget);
        readButton->setObjectName(QStringLiteral("readButton"));
        readButton->setGeometry(QRect(90, 130, 79, 23));
        writeButton = new QPushButton(centralWidget);
        writeButton->setObjectName(QStringLiteral("writeButton"));
        writeButton->setEnabled(true);
        writeButton->setGeometry(QRect(170, 130, 79, 23));
        serialPortLabel_2 = new QLabel(centralWidget);
        serialPortLabel_2->setObjectName(QStringLiteral("serialPortLabel_2"));
        serialPortLabel_2->setGeometry(QRect(110, 10, 81, 19));
        pageEdit = new QLineEdit(centralWidget);
        pageEdit->setObjectName(QStringLiteral("pageEdit"));
        pageEdit->setEnabled(false);
        pageEdit->setGeometry(QRect(210, 10, 31, 20));
        serialPortLabel_3 = new QLabel(centralWidget);
        serialPortLabel_3->setObjectName(QStringLiteral("serialPortLabel_3"));
        serialPortLabel_3->setGeometry(QRect(110, 30, 91, 19));
        wordsEdit = new QLineEdit(centralWidget);
        wordsEdit->setObjectName(QStringLiteral("wordsEdit"));
        wordsEdit->setEnabled(false);
        wordsEdit->setGeometry(QRect(210, 30, 31, 20));
        serialPortLabel_4 = new QLabel(centralWidget);
        serialPortLabel_4->setObjectName(QStringLiteral("serialPortLabel_4"));
        serialPortLabel_4->setGeometry(QRect(180, 50, 31, 19));
        writeAllButton = new QPushButton(centralWidget);
        writeAllButton->setObjectName(QStringLiteral("writeAllButton"));
        writeAllButton->setEnabled(false);
        writeAllButton->setGeometry(QRect(90, 200, 79, 23));
        serialPortLabel_7 = new QLabel(centralWidget);
        serialPortLabel_7->setObjectName(QStringLiteral("serialPortLabel_7"));
        serialPortLabel_7->setGeometry(QRect(100, 70, 61, 19));
        RS485ADRhexEdit = new QLineEdit(centralWidget);
        RS485ADRhexEdit->setObjectName(QStringLiteral("RS485ADRhexEdit"));
        RS485ADRhexEdit->setEnabled(false);
        RS485ADRhexEdit->setGeometry(QRect(170, 70, 61, 20));
        line_8 = new QFrame(centralWidget);
        line_8->setObjectName(QStringLiteral("line_8"));
        line_8->setGeometry(QRect(70, 0, 20, 161));
        line_8->setFrameShape(QFrame::VLine);
        line_8->setFrameShadow(QFrame::Sunken);
        enableNetworkCheckBox = new QCheckBox(centralWidget);
        enableNetworkCheckBox->setObjectName(QStringLiteral("enableNetworkCheckBox"));
        enableNetworkCheckBox->setGeometry(QRect(280, 100, 91, 17));
        enableNetworkCheckBox->setChecked(false);
        line_10 = new QFrame(centralWidget);
        line_10->setObjectName(QStringLiteral("line_10"));
        line_10->setGeometry(QRect(280, 80, 211, 16));
        line_10->setFrameShape(QFrame::HLine);
        line_10->setFrameShadow(QFrame::Sunken);
        line_11 = new QFrame(centralWidget);
        line_11->setObjectName(QStringLiteral("line_11"));
        line_11->setGeometry(QRect(280, 130, 211, 16));
        line_11->setFrameShape(QFrame::HLine);
        line_11->setFrameShadow(QFrame::Sunken);
        serialPortLabel_5 = new QLabel(centralWidget);
        serialPortLabel_5->setObjectName(QStringLiteral("serialPortLabel_5"));
        serialPortLabel_5->setGeometry(QRect(180, 90, 31, 19));
        RS485ADRdecEdit = new QLineEdit(centralWidget);
        RS485ADRdecEdit->setObjectName(QStringLiteral("RS485ADRdecEdit"));
        RS485ADRdecEdit->setGeometry(QRect(170, 110, 61, 20));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 885, 20));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        baudrateComboBox->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        actionThanks->setText(QApplication::translate("MainWindow", "Thanks..", 0));
        label_2->setText(QApplication::translate("MainWindow", "Slave's address", 0));
        resetButton->setText(QApplication::translate("MainWindow", "Reset in Boot", 0));
        eraseButton->setText(QApplication::translate("MainWindow", "Erase!", 0));
        flashButton->setText(QApplication::translate("MainWindow", "Flash!", 0));
        gotoappButton->setText(QApplication::translate("MainWindow", "GoToApp", 0));
        label->setText(QApplication::translate("MainWindow", "Hex file source..", 0));
        serialPortLabel->setText(QApplication::translate("MainWindow", "Com Port:", 0));
        label_3->setText(QApplication::translate("MainWindow", "Response(ms)", 0));
        refreshSerialButton->setText(QApplication::translate("MainWindow", "Refresh", 0));
        label_4->setText(QApplication::translate("MainWindow", "Baudrate", 0));
        baudrateComboBox->clear();
        baudrateComboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "9600", 0)
         << QApplication::translate("MainWindow", "19200", 0)
         << QApplication::translate("MainWindow", "57600", 0)
         << QApplication::translate("MainWindow", "115200", 0)
        );
        baudrateComboBox->setCurrentText(QApplication::translate("MainWindow", "57600", 0));
        sendButton->setText(QApplication::translate("MainWindow", "Send", 0));
        requestLabel->setText(QApplication::translate("MainWindow", "Enter request in Hex (01 4f 3a ..):", 0));
        statusLabel->setText(QApplication::translate("MainWindow", "Status:", 0));
        trafficLabel->setText(QApplication::translate("MainWindow", "Traffic...", 0));
        cleanButton->setText(QApplication::translate("MainWindow", "Clean", 0));
        connectButton->setText(QApplication::translate("MainWindow", "Conn", 0));
        disconnectButton->setText(QApplication::translate("MainWindow", "DisConnect", 0));
        addressesLabel2->setText(QApplication::translate("MainWindow", "Slave's addresses:", 0));
        flashAllButton->setText(QApplication::translate("MainWindow", "Flash All!", 0));
        addressesLabel->setText(QApplication::translate("MainWindow", "-", 0));
        reloadHexButton->setText(QApplication::translate("MainWindow", "ReloadHex", 0));
        readButton->setText(QApplication::translate("MainWindow", "Read ADDR", 0));
        writeButton->setText(QApplication::translate("MainWindow", "Write ADDR", 0));
        serialPortLabel_2->setText(QApplication::translate("MainWindow", "PageSettings:", 0));
        pageEdit->setInputMask(QString());
        pageEdit->setText(QApplication::translate("MainWindow", "16", 0));
        serialPortLabel_3->setText(QApplication::translate("MainWindow", "Words(32bitReg):", 0));
        wordsEdit->setText(QApplication::translate("MainWindow", "255", 0));
        serialPortLabel_4->setText(QApplication::translate("MainWindow", "HEX", 0));
        writeAllButton->setText(QApplication::translate("MainWindow", "Write All!", 0));
        serialPortLabel_7->setText(QApplication::translate("MainWindow", "RS485_ADR", 0));
        RS485ADRhexEdit->setText(QApplication::translate("MainWindow", "0", 0));
        enableNetworkCheckBox->setText(QApplication::translate("MainWindow", "Enable TCP/IP", 0));
        serialPortLabel_5->setText(QApplication::translate("MainWindow", "DEC", 0));
        RS485ADRdecEdit->setText(QApplication::translate("MainWindow", "0", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
