#ifndef CARRECORDWIDGET_H
#define CARRECORDWIDGET_H

#include "database.h"
#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>

namespace Ui {
class CarRecordWidget;
}

class CarRecordWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CarRecordWidget(QWidget *parent = nullptr);
    ~CarRecordWidget();

signals:
    void singleItemSelectedChanged(bool hasSingleItem);

private slots:
    void onAddCarClicked();
    void onDeleteCarClicked();
    void onItemSelectionChanged();

private:
    Ui::CarRecordWidget *ui;
    Database db;
    QLineEdit* usernameEdit;
    QLineEdit* carNumEdit;
    QLineEdit* carTypeEdit;
    QPushButton* addCar;
    QPushButton* deleteCar;
    QPushButton* refresh;
    QTableWidget* table;
    void refreshTable();
    QTableWidgetItem* getSingleSelectedItem();
};

#endif // CARRECORDWIDGET_H
