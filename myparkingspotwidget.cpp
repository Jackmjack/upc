#include "myparkingspotwidget.h"
#include "ui_myparkingspotwidget.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QSpacerItem>

MyParkingspotWidget::MyParkingspotWidget(int userId,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyParkingspotWidget)
{
    ui->setupUi(this);
    this->userId = userId;

    //创建布局
    QVBoxLayout *vLayout = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();

    //创建标题标签
    QLabel *title = new QLabel("我的车位",this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");

    //创建按钮
    QPushButton* refresh = new QPushButton("刷新",this);

    //创建弹簧
    QSpacerItem *hspacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

    //创建表
    table = new QTableWidget(this);
    table->setColumnCount(8);
    table->setHorizontalHeaderLabels({"","车牌号","车型","车位","申请时间","申请状态","处理人","处理时间"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setColumnHidden(0,true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);

    //设置布局
    hLayout->addItem(hspacer);
    hLayout->addWidget(refresh);
    hLayout->setSpacing(4);
    vLayout->addWidget(title);
    vLayout->addWidget(table);
    vLayout->addLayout(hLayout);
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
    connect(refresh,&QPushButton::clicked,this,&MyParkingspotWidget::refreshTable);

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
                  QPushButton:disabled {
                      backgroud-color: gray;
                      color: lightgray;
                      border: 1px solid lightgray;
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

void MyParkingspotWidget::refreshTable()
{
    table->setRowCount(0);
    QSqlQuery query = db.executeSQL("SELECT id,car_id,car_id,parkingspot_id,apply_time,status,approver,approve_time FROM parkingapplication WHERE owner_id = ? AND status = 1",{userId});
    int row = 0;
    while (query.next()) {
        QSqlQuery query1 = db.executeSQL("SELECT car_id,car_type FROM cars WHERE id = ?",{query.value("car_id").toInt()});
        QSqlQuery query2 = db.executeSQL("SELECT location FROM parkingspots WHERE id = ?",{query.value("parkingspot_id").toInt()});
        query1.next();
        query2.next();
        table->insertRow(row);
        for (int col=0;col<table->columnCount();col++) {
            QString val = query.value(col).toString();
            if(col == 1) val = query1.value("car_id").toString();
            else if(col == 2) val = query1.value("car_type").toString();
            else if(col == 3) val = query2.value("location").toString();
            else if (col == 5) {
                if(val == "0") val = "审批中";
                else if(val == "1") val = "申请成功";
                else if(val == "2") val = "未通过";
                else val = "未知";
            }
            QTableWidgetItem* item = new QTableWidgetItem(val);
            table->setItem(row,col,item);
        }
        row++;
    }
}

MyParkingspotWidget::~MyParkingspotWidget()
{
    delete ui;
}
