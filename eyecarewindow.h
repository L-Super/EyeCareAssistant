#ifndef EYECAREWINDOW_H
#define EYECAREWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QTime>
#include <windows.h>

QT_BEGIN_NAMESPACE
namespace Ui { class EyeCareWindow; }
QT_END_NAMESPACE

class EyeCareWindow : public QMainWindow
{
    Q_OBJECT

public:
    EyeCareWindow(QWidget *parent = nullptr);
    ~EyeCareWindow();
    void NotifyText();
    void CreateTrayAction();
    void ShowTrayIcon();

private:
    void closeEvent(QCloseEvent *event);

public slots:
    // 被激活的操作
    void TrayIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    Ui::EyeCareWindow *ui;
    NOTIFYICONDATA nid;
    QSystemTrayIcon *myTrayIcon; // 托盘图标
    QTime currentTime;
    QTimer *timer;
    int num; // 记录触发气泡次数
    QMenu *trayMenu;
    QAction *trayShowAction;
    QAction *trayExitAction;
};
#endif // EYECAREWINDOW_H
