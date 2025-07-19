#include "ownerinfoquerywidget.h"
#include "ui_ownerinfoquerywidget.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>

OwnerInfoQueryWidget::OwnerInfoQueryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OwnerInfoQueryWidget)
{
    ui->setupUi(this);

    //创建布局
    QVBoxLayout *vLayout = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();

    //创建标题标签
    QLabel *title = new QLabel("业主信息查询",this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");

    //创建标签
    QLabel *usernameLabel = new QLabel("查询：",this);

    //创建输入框
    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("输入要查询的业主用户名...");

    //创建表
    table = new QTableWidget(this);
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({"ID","用户名","姓名","电话","地址","详细地址"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);

    //创建按钮
    QPushButton *seek = new QPushButton("查询",this);

    //设置布局
    hLayout->addWidget(usernameLabel);
    hLayout->addWidget(usernameEdit);
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
    connect(seek,&QPushButton::clicked,this,&OwnerInfoQueryWidget::refreshTable);

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

void OwnerInfoQueryWidget::refreshTable()
{
    table->setRowCount(0);
    QString username = usernameEdit->text().trimmed();
    QSqlQuery query;
    if(username.isEmpty()) query = db.executeSQL("SELECT id,username,name,phone,address FROM users WHERE role = 2",{});
    else query = db.executeSQL("SELECT id,username,name,phone,address FROM users WHERE role = 2 AND username = ?",{username});
    int row = 0;
    while (query.next()) {
        table->insertRow(row);
        for (int col=0;col<table->columnCount()-1;col++) {
            QString val = query.value(col).toString();
            QTableWidgetItem* item = new QTableWidgetItem(val);
            table->setItem(row,col,item);
        }
        QSqlQuery query1 = db.executeSQL("SELECT area,building_number,house_number FROM properties WHERE owner_name = ?",{query.value("name").toString()});
        if(query1.next()) {
            QString area = query1.value("area").toString();
            QString buildingNumber = query1.value("building_number").toString();
            QString houseNumber = query1.value("house_number").toString();
            table->setItem(row,5,new QTableWidgetItem(area+buildingNumber+houseNumber));
        } else {
            table->setItem(row,5,new QTableWidgetItem("暂无详细地址"));
        }
        row++;
    }
}

OwnerInfoQueryWidget::~OwnerInfoQueryWidget()
{
    delete ui;
}
