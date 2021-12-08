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

    // 这里设置图标后，关于对话框的图标也会变化
    a.setWindowIcon(QIcon(":/image/image/ico.png"));

    EyeCareWindow w;
    w.show();
//    RestDialog r;
//    r.show();
    return a.exec();
}
