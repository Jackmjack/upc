#ifndef ADDSTAFFWIDGET_H
#define ADDSTAFFWIDGET_H

#include "database.h"
#include <QWidget>
#include <QLineEdit>

namespace Ui {
class AddStaffWidget;
}

class AddStaffWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AddStaffWidget(QWidget *parent = nullptr);
    ~AddStaffWidget();

signals:
    void backToStaffWidget();

private slots:
    void onSubmitClicked();
    void onBackClicked();

private:
    Ui::AddStaffWidget *ui;
    QLineEdit* nameEdit;
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;
    QLineEdit* passwordEdit2;
    QLineEdit* departmentEdit;
    QLineEdit* positionEdit;
    QLineEdit* phoneEdit;
    QLineEdit* addressEdit;
    Database db;
};

#endif // ADDSTAFFWIDGET_H
