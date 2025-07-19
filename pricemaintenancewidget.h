#ifndef PRICEMAINTENANCEWIDGET_H
#define PRICEMAINTENANCEWIDGET_H

#include "database.h"
#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>

namespace Ui {
class PriceMaintenanceWidget;
}

class PriceMaintenanceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PriceMaintenanceWidget(QWidget *parent = nullptr);
    ~PriceMaintenanceWidget();

private slots:
    void addPrice();
    void updatePrice();
    void deletePrice();

private:
    Ui::PriceMaintenanceWidget *ui;
    QTableWidget* table;
    void refreshTable();
    Database db;
    QLineEdit *itemNameEdit;
    QLineEdit *unitEdit;
    QLineEdit *priceEdit;
};

#endif // PRICEMAINTENANCEWIDGET_H
