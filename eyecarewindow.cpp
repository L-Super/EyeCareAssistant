/*
 * 护眼助手
 * 用来提醒长时间用眼
 * 通过气泡弹窗，提醒
*/

// TODO:
// *最小化按钮重写为隐藏到托盘

#include "eyecarewindow.h"
#include "ui_eyecarewindow.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QPainter>
#include <QBrush>
#include <QDebug>

#include <windows.h>

#define WM_SHOWTASK (WM_USER+112) //注意n换成数字，n的大小应大于100

EyeCareWindow::EyeCareWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EyeCareWindow),myTrayIcon(new QSystemTrayIcon(this))
{
    ui->setupUi(this);
    this->setWindowTitle("EyeCare");
    this->resize(300,125);
    // 禁止最大化
    setWindowFlags(this->windowFlags()&~Qt::WindowMaximizeButtonHint);

    // 设置窗体透明
//    this->setWindowOpacity(0.8);

    timeProgress = 0;
    // 气泡推送，每隔一段时间推送一次

    timer = new QTimer(this);
    triggerProgressTimer = new QTimer(this);
    countdown = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&EyeCareWindow::NotifyText);
    connect(triggerProgressTimer,&QTimer::timeout,this,&EyeCareWindow::TimeProgressBar);
}

EyeCareWindow::~EyeCareWindow()
{
    Shell_NotifyIcon(NIM_DELETE,&nid);
    delete ui;
}

// 调用windows api 实现气泡弹窗
void EyeCareWindow::NotifyText()
{
/*
 * https://blog.csdn.net/jhfyuf/article/details/83889749
 * https://www.cnblogs.com/sssworld/p/5379986.html
 * dwMessage 常用的选项如下：
    NIM_ADD
     添加托盘图标。
    NIM_MODIFY
     修改图标，也可弹出气泡图标。
    NIM_DELETE
     删除图标，应在整个程序结束时也调用一次。
  *
  *  cbSize: 	NOTIFYICONDATA 占用的空间大小
  *  uID: 	创建的图标标识，与 hWnd 组合定位托盘图标。
  *  hIcon: 	图标句柄，一般使用 LoadIcon(hInst, MAKEINTRESOURCE(IDI_WIN32TEST)) 赋予。
  *  uTimeout 气泡自动消失的时间
  *  uCallbackMessage 	托盘图标响应消息
  *  uFlags 	标识，指出有效的成员并且指出托盘图标显示的方式:
      * NIF_MESSAGE 使 uCallbackMessage 有效，
      * NIF_ICON 使 hIcon 有效， NIF_TIP 使 szTip 有效，
      * NIF_INFO 使 szInfo 、 szInfoTitle 、 dwInfoFlags 、 uTimeout 有效。
  * szTip 鼠标指向托盘图标时显示的提示消息，注意长度。
  * szInfo 气泡提示内容，注意长度。
  * dwInfoFlags 气泡图标
        NIIF_NONE 表示无图标，
        NIIF_INFO 表示【提示】，
        NIIF_WARNING 表示【警告】，
        NIIF_ERROR 表示【错误】
*/

    qDebug()<<"enter notify func";
    nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);

    nid.uCallbackMessage = WM_SHOWTASK;
    // 标识，指出有效的成员并且指出托盘图标显示的方式
    nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP|NIF_INFO;
    // 气泡提示内容
    wcscpy_s(nid.szInfo,L"你已经看电脑很长时间啦\n放松一下眼睛叭");
    //szInfoTitle 气泡标题
    wcscpy_s(nid.szInfoTitle,L"Ding~");
    // dwInfoFlags 气泡图标
    nid.dwInfoFlags = NIIF_INFO;
    // 气泡自动消失的时间
    nid.uTimeout = 10000;
    Shell_NotifyIcon(NIM_ADD,&nid);//在托盘区添加图标
    // 触发进度条计时器
    triggerProgressTimer->start(1000 * 60);
}

void EyeCareWindow::CreateTrayAction()
{
    if (trayShowAction == nullptr ||
            exitAction == nullptr ||
            aboutAction == nullptr ||
            cancelAction == nullptr ||
            trayMenu == nullptr)
    {
        trayShowAction = new QAction("显示主界面");
        exitAction = new QAction("退出");
        aboutAction = new QAction("关于");
        cancelAction = new QAction("取消");
        trayMenu = new QMenu();
    }

    trayShowAction->setIcon(QIcon(":/image/image/home.png"));
    exitAction->setIcon(QIcon(":/image/image/exit.png"));
    aboutAction->setIcon(QIcon(":/image/image/about.png"));
    cancelAction->setIcon(QIcon(":/image/image/cancel.png"));

    trayMenu->addAction(trayShowAction);
    trayMenu->addAction(cancelAction);
    trayMenu->addAction(aboutAction);
    trayMenu->addSeparator();
    trayMenu->addAction(exitAction);

    connect(trayShowAction, &QAction::triggered, this, [this](){
        this->show();
        myTrayIcon->hide();
    });
    connect(exitAction, &QAction::triggered, this, [this](){
        // 避免退出，图标延时消失
        delete myTrayIcon;
        exit(0);
    });

    connect(aboutAction,&QAction::triggered,[this]{
        // 控制着当最后一个可视的窗口退出时候，程序是否退出，默认是true
        // 不加的话，点击后主程序也退出了
        QApplication::setQuitOnLastWindowClosed(false);
        QMessageBox::about(this,"关于我","一个爱瞎搞的楚莫识\nv1.0.2");
    });
    connect(cancelAction,&QAction::triggered,this,[this]{
        timer->stop();
        QApplication::setQuitOnLastWindowClosed(false);
        QMessageBox::information(this,"取消","已取消定时",QMessageBox::Ok);
    });
}

// 托盘显示应用
void EyeCareWindow::ShowTrayIcon()
{
    // 托盘
    QIcon systemIcon(":/image/image/ico.png");

    try {
        if(myTrayIcon == nullptr)
        {
            myTrayIcon = new QSystemTrayIcon(systemIcon,this);
        }
    } catch (const char* & e) {
        qDebug()<<"myTrayIcon new fail";
    }

    myTrayIcon->setIcon(systemIcon);

    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    myTrayIcon->setToolTip(("护眼小助手"));
    // 在系统拖盘增加图标时显示提示信息。
    myTrayIcon->showMessage("提示","单击隐藏,双击显示主窗口");

    CreateTrayAction();

    // 在用户右击时，弹出菜单。
    myTrayIcon->setContextMenu(trayMenu);
    myTrayIcon->show();
    connect(myTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,
            SLOT(TrayIconActivated(QSystemTrayIcon::ActivationReason)));
}

// 到时弹出休息框，提示休息眼睛
void EyeCareWindow::ShowRestDialog()
{
    restDialog = new RestDialog();
    restDialog->show();

    // 弹窗持续20s
    countdown->start(20000);
    connect(countdown, &QTimer::timeout,this,[&]()
    {
        QApplication::setQuitOnLastWindowClosed(false);
        restDialog->hide();
        restDialog->deleteLater();
        countdown->stop();
    });
}

// 点击关闭按钮，实现隐藏到托盘
void EyeCareWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    ShowTrayIcon();
    this->hide();
}

// 进度条倒计时显示
void EyeCareWindow::TimeProgressBar()
{
    // 计算百分比的公式为：(value() - minimum()) / (maximum() - minimum())。
    auto settingTime = ui->spinBox->text();
    auto times = settingTime.toInt();
    float step = 100 / times;
    timeProgress += step;

    ui->progressBar->setValue(timeProgress);

    if(timeProgress == 100)
    {
        qDebug("time reach 100");
        triggerProgressTimer->stop();
        timeProgress = 0;
        // 窗口最小化
        setWindowState(Qt::WindowMinimized);
        // 弹出休息弹窗
        ShowRestDialog();
    }
}

// 双击显示
void EyeCareWindow::TrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    // 双击
    case QSystemTrayIcon::DoubleClick:
        this->showNormal();
        myTrayIcon->hide();
        break;
// 单击 两者不能同时存在，因双击由两个单击构成
//    case QSystemTrayIcon::Trigger:
// 此showMessage只有在托盘里才有效，不在则crash
//        myTrayIcon->showMessage(("Title"),
//                                ("welcome"),
//                                QSystemTrayIcon::Information,
//                                1000);
//        break;
    default:
        break;
    }
}

// 开始计时
void EyeCareWindow::on_pushButton_clicked()
{
    auto settingTime = ui->spinBox->text();
    auto flag = QMessageBox::information(this,tr("提示对话框"),tr("设置成功!"), QMessageBox::Ok);
    if(flag == QMessageBox::Ok) qDebug()<<tr("clicked ok!");
    // 1000ms = 1s 1000*60 = 1 min
    setTime = settingTime.toInt() * 1000 * 60;
    timer->start(setTime);

//    triggerProgressTimer->start(1000 * 60);
    triggerProgressTimer->start(100); //测试用

}
