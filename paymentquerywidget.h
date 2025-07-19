#ifndef PAYMENTQUERYWIDGET_H
#define PAYMENTQUERYWIDGET_H

#include "database.h"
#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>

namespace Ui {
class PaymentQueryWidget;
}

class PaymentQueryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PaymentQueryWidget(QWidget *parent = nullptr);
    ~PaymentQueryWidget();

private:
    Ui::PaymentQueryWidget *ui;
    Database db;
    QTableWidget* table;
    void refreshTable();
    QLineEdit* ownerIdEdit;
    QComboBox* comboBox;
};

#endif // PAYMENTQUERYWIDGET_H
