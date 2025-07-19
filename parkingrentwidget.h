#ifndef PARKINGRENTWIDGET_H
#define PARKINGRENTWIDGET_H

#include "database.h"
#include <QWidget>
#include <QTableWidget>

namespace Ui {
class ParkingRentWidget;
}

class ParkingRentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ParkingRentWidget(QString name,QWidget *parent = nullptr);
    ~ParkingRentWidget();

signals:
    void singleItemSelectedChanged(bool hasSingleItem);

private slots:
    void onRentClicked();
    void onRejectClicked();
    void onItemSelectionChanged();

private:
    Ui::ParkingRentWidget *ui;
    QString name;
    Database db;
    QTableWidget* table;
    void refreshTable();
    QTableWidgetItem* getSingleSelectedItem();
};

#endif // PARKINGRENTWIDGET_H
