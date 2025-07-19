#ifndef PARAMETERSETWIDGET_H
#define PARAMETERSETWIDGET_H

#include "database.h"
#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class ParameterSetWidget;
}

class ParameterSetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ParameterSetWidget(QWidget *parent = nullptr);
    ~ParameterSetWidget();

private slots:
    void onDiagnoseClicked();

private:
    Ui::ParameterSetWidget *ui;
    QSqlDatabase db;
    Database mydb;
};

#endif // PARAMETERSETWIDGET_H
