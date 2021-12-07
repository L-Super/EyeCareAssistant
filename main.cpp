#include "eyecarewindow.h"
#include "RestDialog.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/image/image/stylesheet.css");
    if( qss.open(QFile::ReadOnly))
    {
        qApp->setStyleSheet(qss.readAll());
        qss.close();
    }
    else
        qDebug("Open failed");


    EyeCareWindow w;
    w.show();
    return a.exec();
}
