#ifndef MYPARKINGSPOTWIDGET_H
#define MYPARKINGSPOTWIDGET_H

#include "database.h"
#include <QWidget>
#include <QTableWidget>

namespace Ui {
class MyParkingspotWidget;
}

class MyParkingspotWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyParkingspotWidget(int userId,QWidget *parent = nullptr);
    ~MyParkingspotWidget();

private:
    Ui::MyParkingspotWidget *ui;
    int userId;
    Database db;
    QTableWidget* table;
    void refreshTable();
};

#endif // MYPARKINGSPOTWIDGET_H
