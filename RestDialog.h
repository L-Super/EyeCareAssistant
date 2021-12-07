#pragma once

#include <QDialog>

namespace Ui {
class RestDialog;
}

class RestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RestDialog(QWidget *parent = nullptr);
    ~RestDialog();
    void LoadBKImg();

private:
    Ui::RestDialog *ui;
};

