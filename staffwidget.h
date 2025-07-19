#ifndef STAFFWIDGET_H
#define STAFFWIDGET_H

#include <QWidget>

namespace Ui {
class StaffWidget;
}

class StaffWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StaffWidget(QWidget *parent = nullptr);
    ~StaffWidget();

private:
    Ui::StaffWidget *ui;
};

#endif // STAFFWIDGET_H
