#ifndef ATTENDANCERECORDWIDGET_H
#define ATTENDANCERECORDWIDGET_H

#include "database.h"
#include <QWidget>
#include <QTableWidget>

namespace Ui {
class AttendanceRecordWidget;
}

class AttendanceRecordWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AttendanceRecordWidget(int userId,QString name,QWidget *parent = nullptr);
    ~AttendanceRecordWidget();

private slots:
    void checkIn();
    void checkOut();

private:
    Ui::AttendanceRecordWidget *ui;
    int userId;
    QString name;
    Database db;
    QTableWidget* table;
    void refreshTable();
};

#endif // ATTENDANCERECORDWIDGET_H
