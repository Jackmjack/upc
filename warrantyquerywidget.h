#ifndef WARRANTYQUERYWIDGET_H
#define WARRANTYQUERYWIDGET_H

#include "database.h"
#include <QWidget>
#include <QTableWidget>

namespace Ui {
class WarrantyQueryWidget;
}

class WarrantyQueryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WarrantyQueryWidget(QWidget *parent = nullptr);
    ~WarrantyQueryWidget();

private:
    Ui::WarrantyQueryWidget *ui;
    Database db;
    QTableWidget* table;
    void refreshTable();
};

#endif // WARRANTYQUERYWIDGET_H
