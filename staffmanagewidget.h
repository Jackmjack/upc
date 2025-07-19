#ifndef STAFFMANAGEWIDGET_H
#define STAFFMANAGEWIDGET_H

#include "database.h"
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>

namespace Ui {
class StaffManageWidget;
}

class StaffManageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StaffManageWidget(QWidget *parent = nullptr);
    ~StaffManageWidget();

signals:
    void singleItemSelectedChangedEdit(bool hasSingleItem);
    void singleItemSelectedChanged(bool hasSingleItem);
    void newAddStaffWidget();

private slots:
    void addStaff();
    void editStaff();
    void deleteStaff();
    void onItemSelectionChangedEdit();
    void onItemSelectionChanged();

private:
    Ui::StaffManageWidget *ui;
    QTableWidget* table;
    Database db;
    void refreshTable();
    QTableWidgetItem* getSingleSelectedItemEdit();
    QTableWidgetItem* getSingleSelectedItem();
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *refreshButton;
};

#endif // STAFFMANAGEWIDGET_H
