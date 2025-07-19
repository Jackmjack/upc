#include "parametersetwidget.h"
#include "ui_parametersetwidget.h"
#include <QPushButton>
#include <QMessageBox>

ParameterSetWidget::ParameterSetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParameterSetWidget)
{
    ui->setupUi(this);

    //创建按钮
    QPushButton* diagnose = new QPushButton("诊断数据库",this);

    //连接按钮与槽函数
    connect(diagnose,&QPushButton::clicked,this,&ParameterSetWidget::onDiagnoseClicked);
}

void ParameterSetWidget::onDiagnoseClicked()
{
    QString report = "<b>=== 数据库诊断报告 ===</b><br>";

    // 数据库连接状态
    if (!mydb.isOpen() && !mydb.openDatabase("smartcommunity.db")) {
        report += "<font color='red'>✗✗ 数据库连接失败</font><br>";
        QMessageBox::critical(this, "数据库错误", "数据库打开失败！");
        return;
    }

    report += "<font color='green'>✓ 数据库已连接</font><br>";
    report += QString("驱动: <b>%1</b><br>").arg(db.driverName());
    report += QString("连接名: <b>%1</b><br>").arg(db.connectionName());
    report += QString("路径: <b>%1</b><br>").arg(db.databaseName());

    // 列出所有表
    QStringList tables = db.tables();
    report += "<br><b>数据库中的表 (" + QString::number(tables.size()) + "):</b><br>";
    for (const QString &tableName : tables) {
        report += " - " + tableName + "<br>";
    }
    QMessageBox::information(this, "数据库诊断", report);
}


ParameterSetWidget::~ParameterSetWidget()
{
    delete ui;
}
