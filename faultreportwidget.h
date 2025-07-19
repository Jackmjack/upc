#ifndef FAULTREPORTWIDGET_H
#define FAULTREPORTWIDGET_H

#include "database.h"
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>

namespace Ui {
class FaultReportWidget;
}

class FaultReportWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FaultReportWidget(int userId,QString name,QWidget *parent = nullptr);
    ~FaultReportWidget();

private slots:
    void submitReport();

private:
    Ui::FaultReportWidget *ui;
    int userId;
    QString name;
    Database db;
    QLineEdit* address;
    QTextEdit* fault;
};

#endif // FAULTREPORTWIDGET_H
