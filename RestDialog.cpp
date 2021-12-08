#include "RestDialog.h"
#include "ui_RestDialog.h"
#include <QPainter>
#include <QDebug>
#include <QIcon>

RestDialog::RestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RestDialog)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/image/image/ico.png"));
    setWindowTitle("Rest");
    setWindowOpacity(0.8);
// FramelessWindowHint + WA_StyledBackground 可实现窗口屏幕置顶
    // 去掉标题栏
    setWindowFlags(Qt::FramelessWindowHint);
    //窗口置顶
    setWindowFlag(Qt::WindowStaysOnTopHint);
//    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_StyledBackground);
    showFullScreen();
//    LoadBKImg();
}

RestDialog::~RestDialog()
{
    delete ui;
}

// 使得透明图片背景正常，非黑色
void RestDialog::paintEvent(QPaintEvent *event)
{
    QPixmap pixmap = QPixmap(":/image/image/littlecute.png").scaled(this->size());
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.drawPixmap(0,0,pixmap);

    // 添加文本
    QFont font;
    font.setPixelSize(30); //设置字体像素大小为12
    QString text = "休息一下噢"; //设置显示的文本
    QString escText("ESC 强制关闭");
    QFontMetrics metrics(font);
    //获取显示文本的宽度
    qDebug()<<"w:"<<metrics.horizontalAdvance(text); //一个字符大小为12*24,所以宽度为12*7
    //获取显示文本的高度
    qDebug()<<"h:"<<metrics.height(); //高度为24

    painter.setFont(font);
//    QLinearGradient ( qreal x1, qreal y1, qreal x2, qreal y2 )
//    //其中x1,y1表示渐变起始坐标, x2,y2表示渐变终点坐标
//    //如果只有x相等,则表示垂直线性渐变,如果只有y相等,则表示平行线性渐变,否则就是斜角线性渐变
    painter.setRenderHint(QPainter::Antialiasing,true);
    QLinearGradient linear(width()/8, height()/10*9,
                           width()/8+metrics.horizontalAdvance(text), height()/10*9+metrics.height());
    linear.setColorAt(0, QColor(103,178,111));
    linear.setColorAt(1, QColor(76,162,205));
//    painter.setBrush(linear);
//    painter.setPen(Qt::transparent);

    QPainterPath path;
    path.addText(width()/8, height()/2, font,text);
    path.addText(width()/10*9, height()/10*9, font, escText);
    painter.fillPath(path,linear);
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
//    setAutoFillBackground(true);
}
