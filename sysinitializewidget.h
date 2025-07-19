#ifndef SYSINITIALIZEWIDGET_H
#define SYSINITIALIZEWIDGET_H

#include "database.h"
#include <QWidget>

namespace Ui {
class SysInitializeWidget;
}

class SysInitializeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SysInitializeWidget(QWidget *parent = nullptr);
    ~SysInitializeWidget();

private slots:
    void onIniClicked();

private:
    Ui::SysInitializeWidget *ui;
    Database db;
};

#endif // SYSINITIALIZEWIDGET_H
