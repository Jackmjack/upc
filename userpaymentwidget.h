#ifndef USERPAYMENTWIDGET_H
#define USERPAYMENTWIDGET_H

#include "database.h"
#include <QWidget>
#include <QTableWidget>
#include <QComboBox>

namespace Ui {
class UserPaymentWidget;
}

class UserPaymentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserPaymentWidget(int userId,QWidget *parent = nullptr);
    ~UserPaymentWidget();

private:
    Ui::UserPaymentWidget *ui;
    int userId;
    Database db;
    QComboBox* comboBox;
    QTableWidget* table;
    void refreshTable();
};

#endif // USERPAYMENTWIDGET_H
