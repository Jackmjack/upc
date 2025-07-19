#include "parkingapplywidget.h"
#include "ui_parkingapplywidget.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QDateTime>

ParkingApplyWidget::ParkingApplyWidget(int userId,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParkingApplyWidget)
{
    ui->setupUi(this);
    this->userId = userId;

    //创建布局
    QVBoxLayout *vLayout = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();

    //创建标题标签
    QLabel *title = new QLabel("车位申请",this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");

    //创建标签
    QLabel* carLabel = new QLabel("选择车辆：",this);
    QLabel* parkingLabel = new QLabel("选择车位：",this);
    QSizePolicy policy = carLabel->sizePolicy();
    policy.setHorizontalPolicy(QSizePolicy::Fixed);
    carLabel->setSizePolicy(policy);
    policy = parkingLabel->sizePolicy();
    policy.setHorizontalPolicy(QSizePolicy::Fixed);
    parkingLabel->setSizePolicy(policy);

    //创建按钮
    QPushButton* submit = new QPushButton("提交申请",this);
    QPushButton* withdraw = new QPushButton("撤销申请",this);
    QPushButton* refresh = new QPushButton("刷新",this);
    withdraw->setDisabled(true);
    policy = submit->sizePolicy();
    policy.setHorizontalPolicy(QSizePolicy::Fixed);
    submit->setSizePolicy(policy);
    policy = withdraw->sizePolicy();
    policy.setHorizontalPolicy(QSizePolicy::Fixed);
    withdraw->setSizePolicy(policy);
    policy = refresh->sizePolicy();
    policy.setHorizontalPolicy(QSizePolicy::Fixed);
    refresh->setSizePolicy(policy);

    //创建下拉框
    carComboBox = new QComboBox(this);
    parkingComboBox = new QComboBox(this);

    //设置下拉框内容
    refreshComboBox();

    //创建表
    table = new QTableWidget(this);
    table->setColumnCount(8);
    table->setHorizontalHeaderLabels({"","车牌号","车型","申请车位","申请时间","申请状态","处理人","处理时间"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setColumnHidden(0,true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);

    //设置布局
    hLayout->addWidget(carLabel);
    hLayout->addWidget(carComboBox);
    hLayout->addWidget(parkingLabel);
    hLayout->addWidget(parkingComboBox);
    hLayout->addWidget(submit);
    hLayout->addWidget(withdraw);
    hLayout->addWidget(refresh);
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
    connect(submit,&QPushButton::clicked,this,&ParkingApplyWidget::onSubmitClicked);
    connect(withdraw,&QPushButton::clicked,this,&ParkingApplyWidget::onWithdrawClicked);
    connect(refresh,&QPushButton::clicked,this,&ParkingApplyWidget::refreshTable);

    //编辑模式设置
    connect(table,&QTableWidget::itemSelectionChanged,this,&ParkingApplyWidget::onItemSelectionChanged);
    connect(this,&ParkingApplyWidget::singleItemSelectedChanged,withdraw,&QPushButton::setEnabled);

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

void ParkingApplyWidget::refreshComboBox()
{
    carComboBox->clear();
    parkingComboBox->clear();
    QSqlQuery query = db.executeSQL("SELECT car_id,id FROM cars WHERE owner_id = ?",{userId});
    while (query.next()) {
        int id = query.value("id").toInt();
        QString carNum = query.value("car_id").toString();
        carComboBox->addItem(carNum,id);
    }
    query = db.executeSQL("SELECT location,id FROM parkingspots WHERE status = 1",{});
    while (query.next()) {
        int id = query.value("id").toInt();
        QString location = query.value("location").toString();
        parkingComboBox->addItem(location,id);
    }
}

void ParkingApplyWidget::refreshTable()
{
    table->setRowCount(0);
    QSqlQuery query = db.executeSQL("SELECT id,car_id,car_id,parkingspot_id,apply_time,status,approver,approve_time FROM parkingapplication WHERE owner_id = ?",{userId});
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

void ParkingApplyWidget::onSubmitClicked()
{
    int car_id = carComboBox->currentData().toInt();
    int parkingspot_id = parkingComboBox->currentData().toInt();
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
    QSqlQuery query = db.executeSQL("SELECT id FROM parkingapplication WHERE car_id = ? AND parkingspot_id = ?",{car_id,parkingspot_id});
    if(query.next()) {
        QMessageBox::warning(this,"错误","请勿重复申请！");
        return;
    }
    db.executeSQL("INSERT INTO parkingapplication (owner_id,car_id,parkingspot_id,apply_time,status) VALUES (?,?,?,?,0)",{userId,car_id,parkingspot_id,time});
    QMessageBox::information(this,"提示","申请提交成功");
    refreshTable();
    return;
}

void ParkingApplyWidget::onWithdrawClicked()
{
    QTableWidgetItem *item = this->getSingleSelectedItem();
    int row = item->row();
    QTableWidgetItem* idItem = table->item(row,0);
    int id = idItem->text().toInt();
    QSqlQuery query = db.executeSQL("SELECT status FROM parkingapplication WHERE id = ?",{id});
    if(query.next()) {
        if(query.value("status").toString() != "0") {
            QMessageBox::warning(this,"错误","只能撤销审批中的申请！");
            return;
        }

    } else {
        QMessageBox::warning(this,"错误","未找到申请记录！");
        return;
    }
    QMessageBox::StandardButton reply = QMessageBox::question(this,"确认撤销","确认撤销车位申请吗？",QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if(reply == QMessageBox::No) {
        QMessageBox::information(this,"提示","已取消操作！");
        return;
    }
    query.prepare("DELETE FROM parkingapplication WHERE id = ?");
    query.addBindValue(id);
    if(query.exec()) {
        QMessageBox::information(this,"提示","撤销成功！");
    } else {
        QMessageBox::warning(this,"错误","撤销失败！");
    }
    refreshTable();
    return;
}

QTableWidgetItem* ParkingApplyWidget::getSingleSelectedItem()
{
    QList<QTableWidgetItem*> selectedItems = table->selectedItems();
    if(selectedItems.size() == 1) {
        return selectedItems.first();
    }
    return nullptr;
}

void ParkingApplyWidget::onItemSelectionChanged()
{
    bool hasSingle = (getSingleSelectedItem() != nullptr);
    emit singleItemSelectedChanged(hasSingle);
}

ParkingApplyWidget::~ParkingApplyWidget()
{
    delete ui;
}
