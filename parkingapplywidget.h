#ifndef PARKINGAPPLYWIDGET_H
#define PARKINGAPPLYWIDGET_H

#include "database.h"
#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>

namespace Ui {
class ParkingApplyWidget;
}

class ParkingApplyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ParkingApplyWidget(int userId,QWidget *parent = nullptr);
    ~ParkingApplyWidget();

signals:
    void singleItemSelectedChanged(bool hasSingleItem);

private slots:
    void onSubmitClicked();
    void onWithdrawClicked();
    void onItemSelectionChanged();

private:
    Ui::ParkingApplyWidget *ui;
    int userId;
    Database db;
    void refreshTable();
    void refreshComboBox();
    QTableWidget* table;
    QComboBox* carComboBox;
    QComboBox* parkingComboBox;
    QTableWidgetItem* getSingleSelectedItem();
};

#endif // PARKINGAPPLYWIDGET_H
