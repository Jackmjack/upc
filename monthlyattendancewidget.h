#ifndef MONTHLYATTENDANCEWIDGET_H
#define MONTHLYATTENDANCEWIDGET_H

#include "database.h"
#include <QWidget>
#include <QTableWidget>

namespace Ui {
class MonthlyAttendanceWidget;
}

class MonthlyAttendanceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MonthlyAttendanceWidget(int userId,QWidget *parent = nullptr);
    ~MonthlyAttendanceWidget();

private:
    Ui::MonthlyAttendanceWidget *ui;
    int userId;
    Database db;
    QTableWidget* table;
    void refreshTable();
};

#endif // MONTHLYATTENDANCEWIDGET_H
