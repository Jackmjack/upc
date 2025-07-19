#ifndef OWNERINFOQUERYWIDGET_H
#define OWNERINFOQUERYWIDGET_H

#include "database.h"
#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>

namespace Ui {
class OwnerInfoQueryWidget;
}

class OwnerInfoQueryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OwnerInfoQueryWidget(QWidget *parent = nullptr);
    ~OwnerInfoQueryWidget();

private:
    Ui::OwnerInfoQueryWidget *ui;
    QTableWidget* table;
    Database db;
    QLineEdit* usernameEdit;
    void refreshTable();
};

#endif // OWNERINFOQUERYWIDGET_H
