#ifndef LEAVEWIDGET_H
#define LEAVEWIDGET_H

#include "database.h"
#include <QWidget>
#include <QTableWidget>
#include <QMessageBox>
#include <QDateEdit>
#include <QLineEdit>

namespace Ui {
class LeaveWidget;
}

class LeaveWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LeaveWidget(int userId,QString name,QWidget *parent = nullptr);
    ~LeaveWidget();

private slots:
    void submit();

private:
    Ui::LeaveWidget *ui;
    int userId;
    QString name;
    QTableWidget* table;
    void refreshTable();
    Database db;
    QDateEdit* startDate;
    QDateEdit* endDate;
    QLineEdit* reason;
};

#endif // LEAVEWIDGET_H
