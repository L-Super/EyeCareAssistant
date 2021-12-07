#include "RestDialog.h"
#include "ui_RestDialog.h"
#include <QPainter>

RestDialog::RestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RestDialog)
{
    ui->setupUi(this);
    setWindowTitle("Rest");
    setWindowOpacity(0.8);
    // 去掉标题栏
    setWindowFlags(Qt::FramelessWindowHint);
    showFullScreen();
    LoadBKImg();
}

RestDialog::~RestDialog()
{
    delete ui;
}

//TODO:透明背景图片为黑色
void RestDialog::LoadBKImg()
{
//    QImage img(":/image/image/littlecute.png");
//    img = img.scaled(this->size());
//    QPixmap p(QPixmap::fromImage(img));

    QPixmap pixmap = QPixmap(":/image/image/littlecute.png").scaled(this->size());

    QPalette palette;
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);
    setAutoFillBackground(true);
}
