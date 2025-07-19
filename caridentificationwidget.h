#ifndef CARIDENTIFICATIONWIDGET_H
#define CARIDENTIFICATIONWIDGET_H

#include "database.h"
#include <QWidget>
#include <QLineEdit>

namespace Ui {
class CarIdentificationWidget;
}

class CarIdentificationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CarIdentificationWidget(int userId,QWidget *parent = nullptr);
    ~CarIdentificationWidget();

private slots:
    void onInClicked();
    void onOutClicked();

private:
    Ui::CarIdentificationWidget *ui;
    int userId;
    Database db;
    QLineEdit* carNumEdit;
};

#endif // CARIDENTIFICATIONWIDGET_H
