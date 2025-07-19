#include "leavewidget.h"
#include "ui_leavewidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>

LeaveWidget::LeaveWidget(int userId,QString name,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LeaveWidget)
{
    ui->setupUi(this);
    this->userId = userId;
    this->name = name;

    //创建布局
    QVBoxLayout *vLayout = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();

    //创建标题标签
    QLabel *title = new QLabel("请销假申请",this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");

    //创建标签
    QLabel* startLabel = new QLabel("开始时间：",this);
    QLabel* endLabel = new QLabel("结束时间：",this);
    QLabel* reasonLabel = new QLabel("请假原因：",this);

    //创建日期输入框
    startDate = new QDateEdit(this);
    endDate = new QDateEdit(this);
    startDate->setDate(QDate::currentDate());
    endDate->setDate(QDate::currentDate());

    //创建输入框
    reason = new QLineEdit(this);
    reason->setPlaceholderText("请输入请假原因...");

    //创建表
    table = new QTableWidget(this);
    table->setColumnCount(8);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setHorizontalHeaderLabels({"ID","姓名","开始时间","结束时间","请假原因","请假状态","审批人","审批时间"});
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);

    //创建按钮
    QPushButton* enter = new QPushButton("提交申请",this);

    //设置布局
    hLayout->addWidget(startLabel);
    hLayout->addWidget(startDate);
    hLayout->addWidget(endLabel);
    hLayout->addWidget(endDate);
    hLayout->addWidget(reasonLabel);
    hLayout->addWidget(reason);
    hLayout->addWidget(enter);
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
    connect(enter,&QPushButton::clicked,this,&LeaveWidget::submit);

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

                  QDateEdit {
                      border: 1px solid #DCDCDC;
                      border-radius: 4px;
                      padding: 0px 4px;
                      background-color: #FFFFFF;
                      color: #333333;
                      font-size: 18px;
                      appearance: none;
                  }
                  QDateEdit:hover {
                      border-color: #BCBCBC;
                  }
                  QDateEdit:focus {
                      border-color: #165DFF;
                      outline: none;
                  }
                  QDateEdit::drop-down {
                      subcontrol-origin: padding;
                      subcontrol-position: top right;
                      width: 25px;
                      border-left: 1px solid #DCDCDC;
                  }
                  QDateEdit::down-arrow {
                      font: 12px "Segoe UI";
                      text-align: center;
                      color: #999999;
                      content: "▼";
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

void LeaveWidget::refreshTable()
{
    table->setRowCount(0);
    QSqlQuery query = db.executeSQL("SELECT staff_id,name,start_date,end_date,reason,status,approver,approve_time FROM leaves WHERE staff_id = ?",{userId});
    int row = 0;
    while (query.next()) {
        table->insertRow(row);
        for (int col=0;col<table->columnCount();col++) {
            QString val = query.value(col).toString();
            if(col == 5) {
                if(val == "0") val = "审批中";
                else if(val == "1") val = "已批准";
                else if(val == "2") val = "未通过";
                else val = "未知";
            }
            QTableWidgetItem* item = new QTableWidgetItem(val);
            table->setItem(row,col,item);
        }
        row++;
    }
}

void LeaveWidget::submit()
{
    QString start = startDate->date().toString("yyyy-MM-dd");
    QString reasonString = reason->text();
    QString end = endDate->date().toString("yyyy-MM-dd");
    db.executeSQL("INSERT INTO leaves (staff_id,name,start_date,end_date,reason,status) VALUES (?,?,?,?,?,0)",{userId,name,start,end,reasonString});
    QMessageBox::information(this,"申请成功","已提交请假申请！");
    reason->clear();
    refreshTable();
    return;
}

LeaveWidget::~LeaveWidget()
{
    delete ui;
}
