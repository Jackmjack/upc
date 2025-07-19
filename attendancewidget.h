#ifndef ATTENDANCEWIDGET_H
#define ATTENDANCEWIDGET_H

#include "database.h"
#include <QWidget>
#include <QTableWidget>

namespace Ui {
class AttendanceWidget;
}

class AttendanceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AttendanceWidget(QWidget *parent = nullptr);
    ~AttendanceWidget();

private:
    Ui::AttendanceWidget *ui;
    Database db;
    QTableWidget* table;
    void refreshTable();
};

#endif // ATTENDANCEWIDGET_H
