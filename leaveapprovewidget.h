#ifndef LEAVEAPPROVEWIDGET_H
#define LEAVEAPPROVEWIDGET_H

#include "database.h"
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>

namespace Ui {
class LeaveApproveWidget;
}

class LeaveApproveWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LeaveApproveWidget(int userId,QString name,QWidget *parent = nullptr);
    ~LeaveApproveWidget();

private slots:
    void leaveAgree();
    void leaveDisagree();
    void leaveCtrlz();
    void selectAll();

private:
    Ui::LeaveApproveWidget *ui;
    Database db;
    QTableWidget* table;
    int userId;
    QString name;
    QPushButton* select;
    QPushButton* agree;
    QPushButton* disagree;
    QPushButton* ctrlz;
    void refreshTable();
};

#endif // LEAVEAPPROVEWIDGET_H
