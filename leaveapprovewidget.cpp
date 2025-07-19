#include "leaveapprovewidget.h"
#include "ui_leaveapprovewidget.h"
#include <QMessageBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QCheckBox>
#include <QSpacerItem>
#include <QDateTime>

LeaveApproveWidget::LeaveApproveWidget(int userId,QString name,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LeaveApproveWidget)
{
    ui->setupUi(this);
    this->userId = userId;
    this->name = name;

    //创建布局
    QVBoxLayout *vLayout = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();

    //创建标题标签
    QLabel *title = new QLabel("请销假审批",this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");

    //创建表
    table = new QTableWidget(this);
    table->setColumnCount(8);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setHorizontalHeaderLabels({"假条编号","员工ID","姓名","开始时间","结束时间","请假原因","请假状态","勾选框",});
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);

    //创建按钮
    agree = new QPushButton("批准",this);
    disagree = new QPushButton("拒绝",this);
    select = new QPushButton("全选",this);
    ctrlz = new QPushButton("撤销",this);

    //创建弹簧
    QSpacerItem *hspacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

    //设置布局
    hLayout->addItem(hspacer);
    hLayout->addWidget(select);
    hLayout->addWidget(agree);
    hLayout->addWidget(disagree);
    hLayout->addWidget(ctrlz);
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
    connect(agree,&QPushButton::clicked,this,&LeaveApproveWidget::leaveAgree);
    connect(disagree,&QPushButton::clicked,this,&LeaveApproveWidget::leaveDisagree);
    connect(ctrlz,&QPushButton::clicked,this,&LeaveApproveWidget::leaveCtrlz);
    connect(select,&QPushButton::clicked,this,&LeaveApproveWidget::selectAll);

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

void LeaveApproveWidget::refreshTable()
{
    table->setRowCount(0);
    QSqlQuery query = db.executeSQL("SELECT id,staff_id,name,start_date,end_date,reason,status FROM leaves",{});
    int row = 0;
    while (query.next()) {
        table->insertRow(row);
        for (int col=0;col<table->columnCount();col++) {
            QString val = query.value(col).toString();
            if(col == 6) {
                if(val == "0"){
                    val = "审批中";
                    QCheckBox* checkBox = new QCheckBox();
                    table->setCellWidget(row,7,checkBox);
                }
                else if(val == "1") val = "已批准";
                else if(val == "2"){
                    val = "未通过";
                    QCheckBox* checkBox = new QCheckBox(this);
                    table->setCellWidget(row,7,checkBox);
                }
                else val = "未知";
            }
            QTableWidgetItem* item = new QTableWidgetItem(val);
            table->setItem(row,col,item);
        }
        row++;
    }
}

void LeaveApproveWidget::selectAll()
{
    bool select_all = (select->text() == "全选");
    int rowCount = table->rowCount();
    for(int row=0;row<rowCount;row++) {
        QCheckBox* checkBox = qobject_cast<QCheckBox*>(table->cellWidget(row,7));
        if(checkBox) {
            checkBox->setChecked(select_all);
        }
    }
    select->setText(select_all ? "取消全选" : "全选");
}

void LeaveApproveWidget::leaveAgree()
{
    int rowCount = table->rowCount();
    QSqlQuery query;
    for(int row=0;row<rowCount;row++) {
        QCheckBox* checkBox = qobject_cast<QCheckBox*>(table->cellWidget(row,7));
        if(checkBox && checkBox->isChecked()) {
            QTableWidgetItem* item = table->item(row,0);
            int id = item->text().toInt();
            QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            db.executeSQL("UPDATE leaves SET status = 1,approver = ?,approve_time = ? WHERE id = ?",{name,date,id});
            QMessageBox::information(this,"批准成功","已批准选中的请假申请！");
        }
    }
    refreshTable();
    select->setText("全选");
    return;
}

void LeaveApproveWidget::leaveDisagree()
{
    int rowCount = table->rowCount();
    QSqlQuery query;
    for(int row=0;row<rowCount;row++) {
        QCheckBox* checkBox = qobject_cast<QCheckBox*>(table->cellWidget(row,7));
        if(checkBox && checkBox->isChecked()) {
            QTableWidgetItem* item = table->item(row,0);
            int id = item->text().toInt();
            QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            db.executeSQL("UPDATE leaves SET status = 2,approver = ?,approve_time = ? WHERE id = ?",{name,date,id});
            QMessageBox::information(this,"拒绝成功","已拒绝选中的请假申请！");
        }
    }
    refreshTable();
    select->setText("全选");
    return;
}

void LeaveApproveWidget::leaveCtrlz()
{
    int rowCount = table->rowCount();
    QSqlQuery query;
    for(int row=0;row<rowCount;row++) {
        QCheckBox* checkBox = qobject_cast<QCheckBox*>(table->cellWidget(row,7));
        if(checkBox && checkBox->isChecked()) {
            QTableWidgetItem* item = table->item(row,0);
            int id = item->text().toInt();
            db.executeSQL("UPDATE leaves SET status = 0,approver = ?,approve_time = ? WHERE id = ?",{"","",id});
            QMessageBox::information(this,"撤销成功","已撤销选中的请假申请！");
        }
    }
    refreshTable();
    select->setText("全选");
    return;
}

LeaveApproveWidget::~LeaveApproveWidget()
{
    delete ui;
}
