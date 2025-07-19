#ifndef REPAIRPROCESSWIDGET_H
#define REPAIRPROCESSWIDGET_H

#include "database.h"
#include <QWidget>
#include <QTableWidget>
#include <QComboBox>

namespace Ui {
class RepairProcessWidget;
}

class RepairProcessWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RepairProcessWidget(QWidget *parent = nullptr);
    ~RepairProcessWidget();

private slots:
    void sendClicked();

private:
    Ui::RepairProcessWidget *ui;
    QTableWidget* table;
    QComboBox* comboBox;
    Database db;
    void refreshTable();
    void refreshComboBox();
};

#endif // REPAIRPROCESSWIDGET_H
