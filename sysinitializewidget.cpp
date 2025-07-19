#include "sysinitializewidget.h"
#include "ui_sysinitializewidget.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QMessageBox>

SysInitializeWidget::SysInitializeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysInitializeWidget)
{
    ui->setupUi(this);

    //创建标题标签
    QLabel *title = new QLabel("系统初始化",this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title1");
    QSizePolicy policy = title->sizePolicy();
    policy.setVerticalPolicy(QSizePolicy::Fixed);
    title->setSizePolicy(policy);

    //创建按钮
    QPushButton* initial = new QPushButton("初始化系统",this);

    //创建布局
    QVBoxLayout* vLayout = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();

    //创建弹簧
    QSpacerItem *hspacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

    //设置布局
    hLayout->addItem(hspacer);
    hLayout->addWidget(initial);
    hLayout->addItem(hspacer);
    vLayout->addWidget(title);
    vLayout->addLayout(hLayout);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setSpacing(0);
    setLayout(vLayout);

    //关联按钮与槽函数
    connect(initial,&QPushButton::clicked,this,&SysInitializeWidget::onIniClicked);

    //设置样式表
    setStyleSheet(R"(QLabel#title1 {
                      color: #000000;
                      font-size: 48px;
                      font-weight: bold;
                      font-family: "华文琥珀",sans-serif;
                      margin-bottom: 0px;
                      backgroud-color: transparent;
                  }
                  QPushButton {
                      background-color: transparent;
                      color: #165DFF;
                      border: 1px solid #165DFF;
                      border-radius: 4px;
                      padding: 2px 18px;
                      font-size: 18px;
                  }
                  QPushButton:hover {
                      color: red;
                      border: 1px solid red;
                  })");
}

void SysInitializeWidget::onIniClicked()
{
    if(!db.openDatabase("smartcommunity.db")) {
        QMessageBox::critical(this,"错误","数据库打开失败");
    }
    int ret = QMessageBox::question(this,"确认初始化","是否清空所有数据？此操作不可逆！",QMessageBox::Yes | QMessageBox::No,QMessageBox::No);
    if(ret != QMessageBox::Yes) {
       return;
    }
    QSqlQuery tableQuery("SELECT name FROM sqlite_master WHERE type='table'");
    while (tableQuery.next()) {
        QString tableName = tableQuery.value(0).toString();

        // 清空表数据
        QSqlQuery deleteQuery(QString("DELETE FROM %1").arg(tableName));

        // 重置自增计数器（通过删除sqlite_sequence表中对应记录）
        QSqlQuery sequenceQuery(QString("DELETE FROM sqlite_sequence WHERE name = '%1'").arg(tableName));
    }
    db.executeSQL("INSERT INTO users (username,password,role,name,department,position,status,phone,address) VALUES ('manager','1234',0,'管理员','xx部门','xx岗位',0,'1xxxxxxxxxx','地址')",{});
    QMessageBox::information(this, "完成", "数据已清空，自增主键已重置！");
}

SysInitializeWidget::~SysInitializeWidget()
{
    delete ui;
}
