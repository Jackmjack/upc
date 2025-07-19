#ifndef OWNERINFOEDITWIDGET_H
#define OWNERINFOEDITWIDGET_H

#include "database.h"
#include <QWidget>
#include <QLineEdit>

namespace Ui {
class OwnerInfoEditWidget;
}

class OwnerInfoEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OwnerInfoEditWidget(QWidget *parent = nullptr);
    ~OwnerInfoEditWidget();

private slots:
    void enterClicked();

private:
    Ui::OwnerInfoEditWidget *ui;
    Database db;
    QLineEdit* usernameEdit;
    QLineEdit* fieldEdit;
    QLineEdit* valueEdit;
};

#endif // OWNERINFOEDITWIDGET_H
