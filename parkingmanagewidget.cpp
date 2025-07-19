#include "parkingmanagewidget.h"
#include "ui_parkingmanagewidget.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QFormLayout>
#include <QGroupBox>

ParkingManageWidget::ParkingManageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParkingManageWidget)
{
    ui->setupUi(this);

    //创建布局
    QFormLayout* formLayout = new QFormLayout();
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QHBoxLayout* hLayout = new QHBoxLayout();

    //创建标题标签
    QLabel *title = new QLabel("车位管理", this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");

    //按钮
    addBtn = new QPushButton("新增车位", this);
    editBtn = new QPushButton("修改车位", this);
    deleteBtn = new QPushButton("删除车位", this);
    refreshBtn = new QPushButton("刷新", this);
    editBtn->setDisabled(true);
    deleteBtn->setDisabled(true);

    //创建输入框
    locationEdit = new QLineEdit(this);
    priceEdit = new QLineEdit(this);

    //创建下拉框
    comboBox = new QComboBox(this);
    comboBox->addItem("未租赁", 0);
    comboBox->addItem("租赁中", 1);

    //创建弹簧
    QSpacerItem* spacer = new QSpacerItem(10,10,QSizePolicy::Fixed,QSizePolicy::Fixed);
    QSpacerItem* hspacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

    //创建表
    table = new QTableWidget(this);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"车位ID","车位位置","车位状态","车位价格(元)"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);

    // 创建表单布局
    formLayout->setLabelAlignment(Qt::AlignRight);
    formLayout->setHorizontalSpacing(10);
    formLayout->setVerticalSpacing(15);
    formLayout->addItem(spacer);
    formLayout->addRow("车位位置：", locationEdit);
    formLayout->addRow("车位价格：", priceEdit);
    formLayout->addRow("车位状态：", comboBox);
    formLayout->addRow(hLayout);
    formLayout->setContentsMargins(10,0,10,10);

    //创建GroupBox
    QGroupBox* groupBox = new QGroupBox("车位信息", this);
    groupBox->setLayout(formLayout);

    //设置布局
    hLayout->addItem(hspacer);
    hLayout->addWidget(addBtn);
    hLayout->addWidget(editBtn);
    hLayout->addWidget(deleteBtn);
    hLayout->addWidget(refreshBtn);
    hLayout->setSpacing(4);
    mainLayout->addWidget(title);
    mainLayout->addWidget(groupBox);
    mainLayout->addWidget(table);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //连接按钮与槽函数
    connect(addBtn,&QPushButton::clicked,this,&ParkingManageWidget::addSpot);
    connect(editBtn,&QPushButton::clicked,this,&ParkingManageWidget::editSpot);
    connect(deleteBtn,&QPushButton::clicked,this,&ParkingManageWidget::deleteSpot);
    connect(refreshBtn,&QPushButton::clicked,this,&ParkingManageWidget::refreshTable);

    //编辑模式设置
    connect(table,&QTableWidget::itemSelectionChanged,this,&ParkingManageWidget::onItemSelectionChanged);
    connect(this,&ParkingManageWidget::singleItemSelectedChanged,editBtn,&QPushButton::setEnabled);
    connect(this,&ParkingManageWidget::singleItemSelectedChanged,deleteBtn,&QPushButton::setEnabled);

    //连接数据库
    if(!db.openDatabase("smartcommunity.db")) {
        QMessageBox::critical(this,"错误","数据库打开失败");
    }

    //刷新表
    refreshTable();

    //设置样式表
    setStyleSheet(R"(

                  QWidget {
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

                  QLabel {
                      color: #000000;
                      font-size: 18px;
                      font-weight: bold;
                      font-family: "Microsoft YaHei",sans-serif;
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
                  QPushButton:disabled {
                      backgroud-color: gray;
                      color: lightgray;
                      border: 1px solid lightgray;
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
                  }

                  QGroupBox {
                      border: 1px solid #000000;
                      border-radius: 5px;
                      margin-top: 0px;
                  }
                  )");
}

void ParkingManageWidget::refreshTable()
{
    table->setRowCount(0);
    QSqlQuery query = db.executeSQL("SELECT id,location,status,price FROM parkingspots",{});
    int row = 0;
    while (query.next()) {
        table->insertRow(row);
        for (int col=0;col<table->columnCount();col++) {
            QString val = query.value(col).toString();
            if(col == 2) {
                if(val == "1") val = "租赁中";
                else if(val == "0") val = "未租赁";
                else if(val == "2") val = "已租出";
                else val = "未知";
            }
            QTableWidgetItem* item = new QTableWidgetItem(val);
            table->setItem(row,col,item);
        }
        row++;
    }
}

void ParkingManageWidget::addSpot()
{
    QString location = locationEdit->text().trimmed();
    QString price = priceEdit->text().trimmed();
    int status = comboBox->currentData().toInt();

    if (location.isEmpty() || price.isEmpty()) {
        QMessageBox::warning(this, "错误", "车位位置和价格不能为空！");
        return;
    }
    db.executeSQL("INSERT INTO parkingspots (location,price,status) VALUES (?,?,?)",{location,price.toInt(),status});
        QMessageBox::information(this, "成功", "车位新增成功！");
        refreshTable();
        locationEdit->clear();
        priceEdit->clear();
        return;
}

void ParkingManageWidget::editSpot()
{
    QString location = locationEdit->text().trimmed();
    QString price = priceEdit->text().trimmed();
    int status = comboBox->currentData().toInt();
    if (location.isEmpty() || price.isEmpty()) {
        QMessageBox::warning(this, "错误", "车位位置和价格不能为空！");
        return;
    }
    QTableWidgetItem *item = this->getSingleSelectedItem();
    int row = item->row();
    QTableWidgetItem* idItem = table->item(row,0);
    int id = idItem->text().toInt();
    db.executeSQL("UPDATE parkingspots SET location = ?,price = ?,status = ? WHERE id = ?",{location,price.toInt(),status,id});
    QMessageBox::information(this,"提示","修改成功！");
    refreshTable();
    locationEdit->clear();
    priceEdit->clear();
    return;
}

void ParkingManageWidget::deleteSpot()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this,"确认删除","确认删除该车位信息吗？此操作不可恢复！",QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if(reply == QMessageBox::No) {
        QMessageBox::information(this,"提示","已取消操作！");
        return;
    }
    QTableWidgetItem *item = this->getSingleSelectedItem();
    int row = item->row();
    QTableWidgetItem* idItem = table->item(row,0);
    int id = idItem->text().toInt();
    QSqlQuery query;
    query.prepare("DELETE FROM parkingspots WHERE id = ?");
    query.addBindValue(id);
    if(query.exec()) {
        QMessageBox::information(this,"提示","删除成功！");
    } else {
        QMessageBox::warning(this,"错误","删除失败！");
    }
    refreshTable();
    return;
}

QTableWidgetItem* ParkingManageWidget::getSingleSelectedItem()
{
    QList<QTableWidgetItem*> selectedItems = table->selectedItems();
    if(selectedItems.size() == 1) {
        int row = selectedItems[0]->row();
        QTableWidgetItem* item = table->item(row,2);
        if(item->text() == "已租出") return nullptr;
        else return selectedItems.first();
    }
    return nullptr;
}

void ParkingManageWidget::onItemSelectionChanged()
{
    bool hasSingle = (getSingleSelectedItem() != nullptr);
    emit singleItemSelectedChanged(hasSingle);
}

ParkingManageWidget::~ParkingManageWidget()
{
    delete ui;
}
