#include "pricemaintenancewidget.h"
#include "ui_pricemaintenancewidget.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QDateTime>

PriceMaintenanceWidget::PriceMaintenanceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PriceMaintenanceWidget)
{
    ui->setupUi(this);

    //创建标题标签
    QLabel *title = new QLabel("价格维护", this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");

    //创建输入框
    QLabel *itemNameLabel = new QLabel("项目名称：", this);
    QLabel *priceLabel = new QLabel("收费金额：", this);
    QLabel *unitLabel = new QLabel("计费单位：", this);
    itemNameEdit = new QLineEdit(this);
    unitEdit = new QLineEdit(this);
    priceEdit = new QLineEdit(this);
    priceEdit->setValidator(new QDoubleValidator(0, 999999, 2, this));

    //创建按钮
    QPushButton* addBtn = new QPushButton("添加", this);
    QPushButton* updateBtn = new QPushButton("更新", this);
    QPushButton* deleteBtn = new QPushButton("删除", this);

    //创建表
    table = new QTableWidget(this);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"ID", "项目名称", "收费金额", "计费单位", "更新时间"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);


    //创建弹簧
    QSpacerItem *hspacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

    //创建布局
    QHBoxLayout *inputLayout = new QHBoxLayout;
    QHBoxLayout *btnLayout = new QHBoxLayout;
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    //设置布局
    inputLayout->addWidget(itemNameLabel);
    inputLayout->addWidget(itemNameEdit);
    inputLayout->addWidget(priceLabel);
    inputLayout->addWidget(priceEdit);
    inputLayout->addWidget(unitLabel);
    inputLayout->addWidget(unitEdit);
    btnLayout->addItem(hspacer);
    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(updateBtn);
    btnLayout->addWidget(deleteBtn);
    btnLayout->setSpacing(4);
    mainLayout->addWidget(title);
    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(table);
    mainLayout->addLayout(btnLayout);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //连接按钮与槽函数
    connect(addBtn, &QPushButton::clicked, this, &PriceMaintenanceWidget::addPrice);
    connect(updateBtn, &QPushButton::clicked, this, &PriceMaintenanceWidget::updatePrice);
    connect(deleteBtn, &QPushButton::clicked, this, &PriceMaintenanceWidget::deletePrice);
    connect(table, &QTableWidget::itemSelectionChanged, [this]() {
        QList<QTableWidgetItem*> selected = table->selectedItems();
        if (!selected.isEmpty()) {
            int row = selected.first()->row();
            itemNameEdit->setText(table->item(row,1)->text());
            priceEdit->setText(table->item(row,2)->text());
            unitEdit->setText(table->item(row,3)->text());
        } else {
            itemNameEdit->clear();
            priceEdit->clear();
            unitEdit->clear();
        }});

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

void PriceMaintenanceWidget::refreshTable()
{
    table->setRowCount(0);
    QSqlQuery query = db.executeSQL("SELECT id,item_name,price,unit,update_time FROM paymentitems",{});
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

void PriceMaintenanceWidget::addPrice()
{
    QString itemName = itemNameEdit->text().trimmed();
    QString unit = unitEdit->text().trimmed();
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    bool ok;
    double price = priceEdit->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "输入错误", "请输入有效的金额！");
        return;
    }
    if (itemName.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请填写项目名称！");
        return;
    }
    if (price <= 0) {
        QMessageBox::warning(this, "输入错误", "金额需大于0！");
        return;
    }
    db.executeSQL("INSERT INTO paymentitems (item_name,unit,price,update_time) VALUES (?,?,?,?)",{itemName,unit,price,time});
    QMessageBox::information(this, "成功", "价格信息添加成功！");
    refreshTable();
}

void PriceMaintenanceWidget::updatePrice()
{
    if (table->selectedItems().isEmpty()) {
        QMessageBox::warning(this, "选择错误", "请选中要更新的行！");
        return;
    }
    int row = table->currentRow();
    int id = table->item(row, 0)->text().toInt();
    QString newItemName = itemNameEdit->text().trimmed();
    QString newUnit = unitEdit->text().trimmed();
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    bool ok;
    double newPrice = priceEdit->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "输入错误", "请输入有效的金额！");
        return;
    }
    if (newItemName.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请填写项目名称！");
        return;
    }
    if (newPrice <= 0) {
        QMessageBox::warning(this, "输入错误", "金额需大于0！");
        return;
    }
    db.executeSQL("UPDATE paymentitems SET item_name = ?,unit = ?,price = ?,update_time = ? WHERE id = ?",{newItemName,newUnit,newPrice,time,id});
    QMessageBox::information(this, "成功", "价格信息更新成功！");
    refreshTable();
}

void PriceMaintenanceWidget::deletePrice()
{
    if (table->selectedItems().isEmpty()) {
        QMessageBox::warning(this, "选择错误", "请选中要删除的行！");
        return;
    }
    int row = table->currentRow();
    int id = table->item(row, 0)->text().toInt();
    if (QMessageBox::question(this, "确认删除", "确定要删除选中的价格记录吗？") != QMessageBox::Yes) {
        QMessageBox::information(this,"提示","已取消操作！");
        return;
    }
    db.executeSQL("DELETE FROM paymentitems WHERE id = ?",{id});
    QMessageBox::information(this, "成功", "价格信息删除成功！");
    refreshTable();
}

PriceMaintenanceWidget::~PriceMaintenanceWidget()
{
    delete ui;
}
