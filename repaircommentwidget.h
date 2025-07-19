#ifndef REPAIRCOMMENTWIDGET_H
#define REPAIRCOMMENTWIDGET_H

#include "database.h"
#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QLineEdit>

namespace Ui {
class RepairCommentWidget;
}

class RepairCommentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RepairCommentWidget(int userId,QWidget *parent = nullptr);
    ~RepairCommentWidget();

private slots:
    void commentClicked();

private:
    Ui::RepairCommentWidget *ui;
    int userId;
    Database db;
    QTableWidget* table;
    QComboBox* comboBox;
    QLineEdit* comment;
    void refreshTable();
};

#endif // REPAIRCOMMENTWIDGET_H
