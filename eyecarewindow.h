#ifndef EYECAREWINDOW_H
#define EYECAREWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QTime>
#include <windows.h>
#include "RestDialog.h"

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
    void ShowRestDialog();

private:
    void closeEvent(QCloseEvent *event);
    void TimeProgressBar();

public slots:
    // 被激活的操作
    void TrayIconActivated(QSystemTrayIcon::ActivationReason reason);

private slots:
    void on_pushButton_clicked();

private:
    Ui::EyeCareWindow *ui;
    NOTIFYICONDATA nid;
    QSystemTrayIcon *myTrayIcon; // 托盘图标
    QTimer *timer;
    int num; // 记录触发气泡次数
    QMenu *trayMenu;
    QAction *trayShowAction;
    QAction *exitAction;
    QAction *cancelAction;
    QAction *aboutAction;
    QTimer *triggerProgressTimer;
    int timeProgress; // 进度条
    RestDialog* restDialog;
};
#endif // EYECAREWINDOW_H
