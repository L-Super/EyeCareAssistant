/*
 * 护眼助手
 * 用来提醒长时间用眼
 * 通过气泡弹窗，提醒
*/

// TODO:最小化按钮重写为隐藏到托盘

#include "eyecarewindow.h"
#include "ui_eyecarewindow.h"
#include <QCloseEvent>
#include <QDebug>

#include <windows.h>

#define WM_SHOWTASK (WM_USER+112) //注意n换成数字，n的大小应大于100

EyeCareWindow::EyeCareWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EyeCareWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("EyeCare");
    this->resize(300,125);
    // 禁止最大化
    setWindowFlags(this->windowFlags()&~Qt::WindowMaximizeButtonHint);
    setWindowIcon(QIcon(":/image/image/ico.png"));
//    // 背景图片

//    QPixmap pixmap = QPixmap(":/image/image/wallpaper.jpg").scaled(this->size());
//    QPalette palette (this->palette());
//    palette.setBrush(QPalette::Background, QBrush(pixmap));
//    this->setPalette(palette);



//    setWindowFlags(Qt::Tool);

//    currentTime = QTime::currentTime();
    // 气泡推送，每隔一段时间推送一次
    num = 0;
    timer = new QTimer(this);
    timer->start(500000);
    connect(timer,&QTimer::timeout,this,&EyeCareWindow::NotifyText);
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
  *  uTimeout
  * uCallbackMessage 	托盘图标响应消息
  *  uFlags 	标识，指出有效的成员并且指出托盘图标显示的方式:
      * NIF_MESSAGE 使 uCallbackMessage 有效，
      * NIF_ICON 使 hIcon 有效， NIF_TIP 使 szTip 有效，
      * NIF_INFO 使 szInfo 、 szInfoTitle 、 dwInfoFlags 、 uTimeout 有效。
  *  szTip 鼠标指向托盘图标时显示的提示消息，注意长度。
  * szInfo 气泡提示内容，注意长度。
*/

    qDebug()<<"enter notify func"<<num++;
    nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);

    nid.uCallbackMessage = WM_SHOWTASK;
    // 标识，指出有效的成员并且指出托盘图标显示的方式
    nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP|NIF_INFO;
    // 气泡提示内容
    wcscpy_s(nid.szInfo,L"内容");
    //szInfoTitle 气泡标题
    wcscpy_s(nid.szInfoTitle,L"推送标题");
    // dwInfoFlags 气泡图标，
    // NIIF_NONE 表示无图标，
    // NIIF_INFO 表示【提示】，
    // NIIF_WARNING 表示【警告】，
    // NIIF_ERROR 表示【错误】
    nid.dwInfoFlags = NIIF_INFO;
    // 气泡自动消失的时间
    nid.uTimeout = 10000;
    Shell_NotifyIcon(NIM_ADD,&nid);//在托盘区添加图标
}

void EyeCareWindow::CreateTrayAction()
{
    trayShowAction = new QAction("显示主界面");
    trayExitAction = new QAction("退出");
    trayShowAction->setIcon(QIcon(":/image/image/home.png"));
    trayExitAction->setIcon(QIcon(":/image/image/exit.png"));
   connect(trayShowAction,&QAction::triggered,this,[this](){
       this->show();
       myTrayIcon->deleteLater(); });
    connect(trayExitAction,&QAction::triggered,this,[this](){
        // 避免退出，图标延时消失
        delete myTrayIcon;
        exit(0);});
}

// 托盘显示应用
void EyeCareWindow::ShowTrayIcon()
{
    // 托盘
    QIcon systemIcon(":/image/image/ico.png");
    myTrayIcon = new QSystemTrayIcon(systemIcon,this);

    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    myTrayIcon->setToolTip(("护眼小助手"));
    // 在系统拖盘增加图标时显示提示信息。
    myTrayIcon->showMessage("提示","单击隐藏,双击显示主窗口");
//    QScopedPointer<QMenu> trayMenu(new QMenu);
//    QScopedPointer<QAction> trayShowAction(new QAction("显示主界面"));
    CreateTrayAction();
    trayMenu = new QMenu();
    trayMenu->addAction(trayShowAction);
    trayMenu->addSeparator();
    trayMenu->addAction(trayExitAction);

    // 在用户右击时，弹出菜单。
    myTrayIcon->setContextMenu(trayMenu);
    myTrayIcon->show();
    connect(myTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,
            SLOT(TrayIconActivated(QSystemTrayIcon::ActivationReason)));
}

// 点击关闭按钮，实现隐藏到托盘
void EyeCareWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    ShowTrayIcon();
    this->hide();
}

// 双击显示
void EyeCareWindow::TrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    // 双击
    case QSystemTrayIcon::DoubleClick:
        this->showNormal();
        myTrayIcon->deleteLater();
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

