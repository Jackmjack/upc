#include "paymentquerywidget.h"
#include "ui_paymentquerywidget.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>

PaymentQueryWidget::PaymentQueryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaymentQueryWidget)
{
    ui->setupUi(this);

    //创建布局
    QVBoxLayout *vLayout = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();

    //创建标题标签
    QLabel *title = new QLabel("缴费查询",this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");

    //创建标签
    QLabel *ownerIdLabel = new QLabel("业主ID：",this);
    QLabel *methodLabel = new QLabel("缴费方式：",this);

    //创建输入框
    ownerIdEdit = new QLineEdit(this);
    ownerIdEdit->setPlaceholderText("输入要查询的业主ID...");

    //创建下拉框
    comboBox = new QComboBox(this);
    comboBox->addItem("全部",-1);
    comboBox->addItem("线下缴费",0);
    comboBox->addItem("自助缴费",1);

    //创建表
    table = new QTableWidget(this);
    table->setColumnCount(8);
    table->setHorizontalHeaderLabels({"","缴费人","缴费项目","缴费数量","总金额","缴费时间","缴费方式","处理人"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setColumnHidden(0,true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);

    //创建按钮
    QPushButton *seek = new QPushButton("查询",this);

    //设置布局
    hLayout->addWidget(ownerIdLabel);
    hLayout->addWidget(ownerIdEdit);
    hLayout->addWidget(methodLabel);
    hLayout->addWidget(comboBox);
    hLayout->addWidget(seek);
    hLayout->setSpacing(4);
    vLayout->addWidget(title);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(table);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setSpacing(0);
    setLayout(vLayout);

    //连接数据库
    if(!db.openDatabase("smartcommunity.db")) {
        QMessageBox::critical(this,"错误","数据库打开失败");
    }

    //刷新表
    refreshTable();

    //连接按钮与槽函数
    connect(seek,&QPushButton::clicked,this,&PaymentQueryWidget::refreshTable);

    //设置样式表
    setStyleSheet(R"(QWidget {
                      background-color: #f0f5ff;
                      font-family: "Microsoft YaHei", sans-serif;
                  }

                  QLabel#title {
                      color: #000000;
                      font-size: 48px;
                      font-weight: bold;
                      font-family: "华文琥珀",sans-serif;
                      margin-bottom: 0px;
                  }

                  QTableWidget {
                      background-color: #f0f5ff;
                      gridline-color: #e0e0e0;
                  }
                  QHeaderView::section {
                      background-color: #4a90e2;
                      color: white;
                      font-weight: bold;
                      margin-top:0px;
                      padding: 6px 8px;
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
                      background-color: #E8F3FF;
                  }

                  QLineEdit {
                      border: 1px solid #CCCCCC;
                      border-radius: 4px;
                      padding: 0px 4px;
                      font-size: 20px;
                      background-color: white;
                  }
                  QLineEdit:focus {
                      border-color: #165DFF;
                      outline: none;
                  }

                  QComboBox {
                      border: 1px solid #DCDCDC;
                      border-radius: 4px;
                      padding: 2px 30px 2px 10px;
                      background-color: #FFFFFF;
                      color: #333333;
                      font-size: 18px;
                  }
                  QComboBox:hover {
                      border-color: #BCBCBC;
                  }
                  QComboBox:focus {
                      border-color: #165DFF;
                      outline: none;
                  }
                  QComboBox QAbstractItemView {
                      border: 1px solid #DCDCDC;
                      border-radius: 4px;
                      background-color: #FFFFFF;
                      color: #333333;
                      font-size: 18px;
                      selection-background-color: #f0f5ff;
                      selection-color: #165DFF;
                  })");
}

void PaymentQueryWidget::refreshTable()
{
    table->setRowCount(0);
    QString ownerId = ownerIdEdit->text().trimmed();
    int method = comboBox->currentData().toInt();
    QSqlQuery query;
    if(method == -1) {
        if(ownerId.isEmpty()) query = db.executeSQL("SELECT id,pay_name,item_id,quantity,amount,pay_time,pay_method,operator_id FROM paymentrecords",{});
        else query = db.executeSQL("SELECT id,pay_name,item_id,quantity,amount,pay_time,pay_method,operator_id FROM paymentrecords WHERE owner_id = ?",{ownerId.toInt()});
    } else {
        if(ownerId.isEmpty()) query = db.executeSQL("SELECT id,pay_name,item_id,quantity,amount,pay_time,pay_method,operator_id FROM paymentrecords WHERE pay_method = ?",{method});
        else query = db.executeSQL("SELECT id,pay_name,item_id,quantity,amount,pay_time,pay_method,operator_id FROM paymentrecords WHERE owner_id = ? AND pay_method = ?",{ownerId.toInt(),method});
    }
    int row = 0;
    while (query.next()) {
        table->insertRow(row);
        for (int col=0;col<table->columnCount();col++) {
            QString val = query.value(col).toString();
            if(col == 2) {
                QSqlQuery query1 = db.executeSQL("SELECT item_name FROM paymentitems WHERE id = ?",{val.toInt()});
                if(query1.next()) val = query1.value("item_name").toString();
                else QMessageBox::warning(this,"错误","未查询到缴费项目！");
            }
            if(col == 6) {
                if(val == "0") val = "线下缴费";
                else if(val == "1") val = "自助缴费";
                else val = "未知";
            }
            if(col == 7) {
                if(!val.isEmpty()) {
                   QSqlQuery query1 = db.executeSQL("SELECT name FROM users WHERE id =?",{val.toInt()});
                  if(query1.next()) val = query1.value("name").toString();
                }
            }
            QTableWidgetItem* item = new QTableWidgetItem(val);
            table->setItem(row,col,item);
        }
        row++;
    }
}

PaymentQueryWidget::~PaymentQueryWidget()
{
    delete ui;
}
