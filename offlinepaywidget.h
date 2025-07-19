#ifndef OFFLINEPAYWIDGET_H
#define OFFLINEPAYWIDGET_H

#include "database.h"
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>

namespace Ui {
class OfflinePayWidget;
}

class OfflinePayWidget : public QWidget
{
    Q_OBJECT

private slots:
    void calculatePay();
    void confirmPayment();

public:
    explicit OfflinePayWidget(int userId,QWidget *parent = nullptr);
    ~OfflinePayWidget();

private:
    Ui::OfflinePayWidget *ui;
    Database db;
    int userId;
    QLineEdit* ownerNameEdit;
    QLineEdit* ownerIdEdit;
    QLineEdit* quantityEdit;
    QLineEdit* amountEdit;
    QComboBox* itemComboBox;
    void loadPaymentItems();
};

#endif // OFFLINEPAYWIDGET_H
