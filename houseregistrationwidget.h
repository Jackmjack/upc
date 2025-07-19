#ifndef HOUSEREGISTRATIONWIDGET_H
#define HOUSEREGISTRATIONWIDGET_H

#include "database.h"
#include <QWidget>
#include <QLineEdit>

namespace Ui {
class HouseRegistrationWidget;
}

class HouseRegistrationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HouseRegistrationWidget(QWidget *parent = nullptr);
    ~HouseRegistrationWidget();

private slots:
    void onSubmitClicked();

private:
    Ui::HouseRegistrationWidget *ui;
    QLineEdit* nameEdit;
    QLineEdit* houseNumberEdit;
    QLineEdit* buildingNumberEdit;
    QLineEdit* areaEdit;
    Database db;
};

#endif // HOUSEREGISTRATIONWIDGET_H
