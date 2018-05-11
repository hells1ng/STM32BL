#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Stm32BL_FlashTool");
    MainWindow *mainWin = new MainWindow;
    mainWin->show();
    return app.exec();
}
