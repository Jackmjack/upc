#ifndef REPAIRQUERYWIDGET_H
#define REPAIRQUERYWIDGET_H

#include "database.h"
#include <QWidget>
#include <QTableWidget>

namespace Ui {
class RepairQueryWidget;
}

class RepairQueryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RepairQueryWidget(int userId,QWidget *parent = nullptr);
    ~RepairQueryWidget();

private:
    Ui::RepairQueryWidget *ui;
    int userId;
    Database db;
    QTableWidget* table;
    void refreshTable();
};

#endif // REPAIRQUERYWIDGET_H
