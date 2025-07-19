#include "parkingrentwidget.h"
#include "ui_parkingrentwidget.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QSpacerItem>
#include <QDateTime>

ParkingRentWidget::ParkingRentWidget(QString name,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParkingRentWidget)
{
    ui->setupUi(this);
    this->name = name;

    //创建布局
    QVBoxLayout *vLayout = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();

    //创建标题标签
    QLabel *title = new QLabel("车位承租",this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");

    //创建表
    table = new QTableWidget(this);
    table->setColumnCount(8);
    table->setHorizontalHeaderLabels({"申请编号","申请人","申请车辆","申请车位","申请时间","申请状态","处理人","处理时间"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);

    //创建按钮
    QPushButton *rent = new QPushButton("出租",this);
    QPushButton *reject = new QPushButton("拒绝",this);
    QPushButton *refresh = new QPushButton("刷新",this);
    rent->setDisabled(true);
    reject->setDisabled(true);

    //创建弹簧
    QSpacerItem *hspacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

    //设置布局
    hLayout->addItem(hspacer);
    hLayout->addWidget(rent);
    hLayout->addWidget(reject);
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
    connect(rent,&QPushButton::clicked,this,&ParkingRentWidget::onRentClicked);
    connect(reject,&QPushButton::clicked,this,&ParkingRentWidget::onRejectClicked);
    connect(refresh,&QPushButton::clicked,this,&ParkingRentWidget::refreshTable);

    //编辑模式设置
    connect(table,&QTableWidget::itemSelectionChanged,this,&ParkingRentWidget::onItemSelectionChanged);
    connect(this,&ParkingRentWidget::singleItemSelectedChanged,rent,&QPushButton::setEnabled);
    connect(this,&ParkingRentWidget::singleItemSelectedChanged,reject,&QPushButton::setEnabled);

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

void ParkingRentWidget::refreshTable()
{
    table->setRowCount(0);
    QSqlQuery query = db.executeSQL("SELECT id,owner_id,car_id,parkingspot_id,apply_time,status,approver,approve_time FROM parkingapplication WHERE status = 0",{});
    int row = 0;
    while (query.next()) {
        QSqlQuery query1 = db.executeSQL("SELECT name FROM users WHERE id = ?",{query.value("owner_id").toInt()});
        QSqlQuery query2 = db.executeSQL("SELECT car_id FROM cars WHERE id = ?",{query.value("car_id").toInt()});
        QSqlQuery query3 = db.executeSQL("SELECT location FROM parkingspots WHERE id = ?",{query.value("parkingspot_id").toInt()});
        query1.next();
        query2.next();
        query3.next();
        table->insertRow(row);
        for (int col=0;col<table->columnCount();col++) {
            QString val = query.value(col).toString();
            if(col == 1) val = query1.value("name").toString();
            else if(col == 2) val = query2.value("car_id").toString();
            else if(col == 3) val = query3.value("location").toString();
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

void ParkingRentWidget::onRentClicked()
{
    QTableWidgetItem *item = this->getSingleSelectedItem();
    int row = item->row();
    QTableWidgetItem* idItem = table->item(row,0);
    QTableWidgetItem* spotItem = table->item(row,3);
    int id = idItem->text().toInt();
    QString location = spotItem->text();
    QMessageBox::StandardButton reply = QMessageBox::question(this,"确认租赁","确认将车位租给该业主吗？",QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if(reply == QMessageBox::No) {
        QMessageBox::information(this,"提示","已取消操作！");
        return;
    }
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
    QSqlQuery query = db.executeSQL("SELECT id FROM parkingspots WHERE location = ?",{location});
    query.next();
    int spot_id = query.value("id").toInt();
    query = db.executeSQL("SELECT owner_id FROM parkingapplication WHERE id = ?",{id});
    query.next();
    int owner_id = query.value("owner_id").toInt();
    db.executeSQL("UPDATE parkingapplication SET status = 1,approver = ?,approve_time = ? WHERE id = ?",{this->name,time,id});
    db.executeSQL("UPDATE parkingapplication SET status = 2,approver = ?,approve_time = ? WHERE parkingspot_id = ? AND id != ?",{this->name,time,spot_id,id});
    db.executeSQL("UPDATE parkingspots SET status = 2,owner_id = ? WHERE id = ?",{owner_id,spot_id});
    QMessageBox::information(this,"提示","车位租出成功！\n已自动拒绝同车位其他申请！");
    refreshTable();
    return;
}

void ParkingRentWidget::onRejectClicked()
{
    QTableWidgetItem *item = this->getSingleSelectedItem();
    int row = item->row();
    QTableWidgetItem* idItem = table->item(row,0);
    int id = idItem->text().toInt();
    QMessageBox::StandardButton reply = QMessageBox::question(this,"确认拒绝","确认拒绝该业主的申请吗？",QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if(reply == QMessageBox::No) {
        QMessageBox::information(this,"提示","已取消操作！");
        return;
    }
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
    db.executeSQL("UPDATE parkingapplication SET status = 2,approver = ?,approve_time = ? WHERE id = ?",{this->name,time,id});
    QMessageBox::information(this,"提示","申请拒绝成功！");
    refreshTable();
    return;
}

QTableWidgetItem* ParkingRentWidget::getSingleSelectedItem()
{
    QList<QTableWidgetItem*> selectedItems = table->selectedItems();
    if(selectedItems.size() == 1) {
        return selectedItems.first();
    }
    return nullptr;
}

void ParkingRentWidget::onItemSelectionChanged()
{
    bool hasSingle = (getSingleSelectedItem() != nullptr);
    emit singleItemSelectedChanged(hasSingle);
}

ParkingRentWidget::~ParkingRentWidget()
{
    delete ui;
}
