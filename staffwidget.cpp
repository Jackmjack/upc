#include "staffwidget.h"
#include "ui_staffwidget.h"

StaffWidget::StaffWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StaffWidget)
{
    ui->setupUi(this);
}

StaffWidget::~StaffWidget()
{
    delete ui;
}
