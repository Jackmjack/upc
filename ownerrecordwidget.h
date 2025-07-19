#ifndef OWNERRECORDWIDGET_H
#define OWNERRECORDWIDGET_H

#include "database.h"
#include <QWidget>
#include <QLineEdit>

namespace Ui {
class OwnerRecordWidget;
}

class OwnerRecordWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OwnerRecordWidget(QWidget *parent = nullptr);
    ~OwnerRecordWidget();

private slots:
    void onSubmitClicked();

private:
    Ui::OwnerRecordWidget *ui;
    QLineEdit* nameEdit;
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;
    QLineEdit* passwordEdit2;
    QLineEdit* phoneEdit;
    QLineEdit* addressEdit;
    Database db;
};

#endif // OWNERRECORDWIDGET_H
