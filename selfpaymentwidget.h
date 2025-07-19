#ifndef SELFPAYMENTWIDGET_H
#define SELFPAYMENTWIDGET_H

#include "database.h"
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>

namespace Ui {
class SelfPaymentWidget;
}

class SelfPaymentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SelfPaymentWidget(int userId,QString name,QWidget *parent = nullptr);
    ~SelfPaymentWidget();

private slots:
    void calculatePay();
    void confirmPayment();

private:
    Ui::SelfPaymentWidget *ui;
    Database db;
    int userId;
    QString name;
    QLineEdit* quantityEdit;
    QLineEdit* amountEdit;
    QComboBox* itemComboBox;
    void loadPaymentItems();
};

#endif // SELFPAYMENTWIDGET_H
