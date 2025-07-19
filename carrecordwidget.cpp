#include "carrecordwidget.h"
#include "ui_carrecordwidget.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QFormLayout>
#include <QGroupBox>

CarRecordWidget::CarRecordWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CarRecordWidget)
{
    ui->setupUi(this);

    //创建布局
    QFormLayout* formLayout = new QFormLayout();
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QHBoxLayout* hLayout = new QHBoxLayout();

    //创建标题标签
    QLabel *title = new QLabel("车辆登记", this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");

    //按钮
    addCar = new QPushButton("添加车辆", this);
    deleteCar = new QPushButton("删除车辆", this);
    refresh = new QPushButton("刷新", this);
    deleteCar->setDisabled(true);

    //创建输入框
    usernameEdit = new QLineEdit(this);
    carNumEdit = new QLineEdit(this);
    carTypeEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("请输入车主用户名");
    carNumEdit->setPlaceholderText("请输入车牌号");
    carTypeEdit->setPlaceholderText("请输入车型");

    //创建弹簧
    QSpacerItem* spacer = new QSpacerItem(10,10,QSizePolicy::Fixed,QSizePolicy::Fixed);
    QSpacerItem* hspacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

    //创建表
    table = new QTableWidget(this);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"车辆ID","车主ID","车主姓名","车牌号","车型"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);

    // 创建表单布局
    formLayout->setLabelAlignment(Qt::AlignRight);
    formLayout->setHorizontalSpacing(10);
    formLayout->setVerticalSpacing(15);
    formLayout->addItem(spacer);
    formLayout->addRow("用户名：", usernameEdit);
    formLayout->addRow("车牌号：", carNumEdit);
    formLayout->addRow("车型：", carTypeEdit);
    formLayout->addRow(hLayout);
    formLayout->setContentsMargins(10,0,10,10);

    //创建GroupBox
    QGroupBox* groupBox = new QGroupBox("车辆信息", this);
    groupBox->setLayout(formLayout);

    //设置布局
    hLayout->addItem(hspacer);
    hLayout->addWidget(addCar);
    hLayout->addWidget(deleteCar);
    hLayout->addWidget(refresh);
    hLayout->setSpacing(4);
    mainLayout->addWidget(title);
    mainLayout->addWidget(groupBox);
    mainLayout->addWidget(table);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //连接按钮与槽函数
    connect(addCar,&QPushButton::clicked,this,&CarRecordWidget::onAddCarClicked);
    connect(deleteCar,&QPushButton::clicked,this,&CarRecordWidget::onDeleteCarClicked);
    connect(refresh,&QPushButton::clicked,this,&CarRecordWidget::refreshTable);

    //编辑模式设置
    connect(table,&QTableWidget::itemSelectionChanged,this,&CarRecordWidget::onItemSelectionChanged);
    connect(this,&CarRecordWidget::singleItemSelectedChanged,deleteCar,&QPushButton::setEnabled);

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

void CarRecordWidget::refreshTable()
{
    table->setRowCount(0);
    QSqlQuery query = db.executeSQL("SELECT id,owner_id,owner_name,car_id,car_type FROM cars",{});
    int row = 0;
    while (query.next()) {
        table->insertRow(row);
        for (int col=0;col<table->columnCount();col++) {
            QString val = query.value(col).toString();
            QTableWidgetItem* item = new QTableWidgetItem(val);
            table->setItem(row,col,item);
        }
        row++;
    }
}

void CarRecordWidget::onAddCarClicked()
{
    QString username = usernameEdit->text().trimmed();
    QString carNum = carNumEdit->text().trimmed();
    QString carType = carTypeEdit->text().trimmed();
    QString name = "";
    int userId = -1;
    if (username.isEmpty() || carNum.isEmpty() || carType.isEmpty()) {
        QMessageBox::warning(this, "错误", "请输入完整信息！");
        return;
    }
    QSqlQuery query = db.executeSQL("SELECT id,name FROM users WHERE username = ?",{username});
    if(query.next()) {
        name = query.value("name").toString();
        userId = query.value("id").toInt();
    } else {
        QMessageBox::warning(this,"错误","未找到该用户！");
        return;
    }
    db.executeSQL("INSERT INTO cars (owner_id,owner_name,car_id,car_type) VALUES (?,?,?,?)",{userId,name,carNum,carType});
    QMessageBox::information(this,"提示","添加成功！");
    refreshTable();
    usernameEdit->clear();
    carNumEdit->clear();
    carTypeEdit->clear();
    return;
}

void CarRecordWidget::onDeleteCarClicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this,"确认删除","确认删除该车辆信息吗？此操作不可恢复！",QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if(reply == QMessageBox::No) {
        QMessageBox::information(this,"提示","已取消操作！");
        return;
    }
    QTableWidgetItem *item = this->getSingleSelectedItem();
    int row = item->row();
    QTableWidgetItem* idItem = table->item(row,0);
    int id = idItem->text().toInt();
    QSqlQuery query;
    query.prepare("DELETE FROM cars WHERE id = ?");
    query.addBindValue(id);
    if(query.exec()) {
        QMessageBox::information(this,"提示","删除成功！");
    } else {
        QMessageBox::warning(this,"错误","删除失败！");
    }
    refreshTable();
    return;
}

QTableWidgetItem* CarRecordWidget::getSingleSelectedItem()
{
    QList<QTableWidgetItem*> selectedItems = table->selectedItems();
    if(selectedItems.size() == 1) {
        return selectedItems.first();
    }
    return nullptr;
}

void CarRecordWidget::onItemSelectionChanged()
{
    bool hasSingle = (getSingleSelectedItem() != nullptr);
    emit singleItemSelectedChanged(hasSingle);
}

CarRecordWidget::~CarRecordWidget()
{
    delete ui;
}
