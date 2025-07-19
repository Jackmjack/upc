#ifndef PARKINGMANAGEWIDGET_H
#define PARKINGMANAGEWIDGET_H

#include "database.h"
#include <QWidget>
#include <QLineEdit>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>

namespace Ui {
class ParkingManageWidget;
}

class ParkingManageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ParkingManageWidget(QWidget *parent = nullptr);
    ~ParkingManageWidget();

signals:
    void singleItemSelectedChanged(bool hasSingleItem);

private slots:
    void addSpot();
    void editSpot();
    void deleteSpot();
    void onItemSelectionChanged();

private:
    Ui::ParkingManageWidget *ui;
    Database db;
    QTableWidget* table;
    void refreshTable();
    QTableWidgetItem* getSingleSelectedItem();
    QLineEdit *locationEdit;
    QLineEdit *priceEdit;
    QComboBox *comboBox;
    QPushButton *addBtn;
    QPushButton *editBtn;
    QPushButton *deleteBtn;
    QPushButton *refreshBtn;
};

#endif // PARKINGMANAGEWIDGET_H
