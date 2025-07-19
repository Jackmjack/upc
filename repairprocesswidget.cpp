#include "repairprocesswidget.h"
#include "ui_repairprocesswidget.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QCheckBox>

RepairProcessWidget::RepairProcessWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RepairProcessWidget)
{
    ui->setupUi(this);

    //创建布局
    QVBoxLayout *vLayout = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();

    //创建标题标签
    QLabel *title = new QLabel("维修过程",this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");

    //创建标签
    QLabel* staff = new QLabel("选择维修人员：",this);
    staff->setObjectName("staff");

    //创建表
    table = new QTableWidget(this);
    table->setColumnCount(10);
    table->setHorizontalHeaderLabels({"报修单号","报修人","报修时间","报修原因","地址","联系电话","报修状态","维修人","维修人联系电话","勾选框"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);

    //创建下拉框
    comboBox = new QComboBox(this);

    //创建按钮
    QPushButton* send = new QPushButton("派单",this);

    //创建弹簧
    QSpacerItem *hspacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

    //设置布局
    hLayout->addWidget(staff);
    hLayout->addWidget(comboBox);
    hLayout->addItem(hspacer);
    hLayout->addWidget(send);
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

    //刷新下拉框
    refreshComboBox();

    //连接按钮与槽函数
    connect(send,&QPushButton::clicked,this,&RepairProcessWidget::sendClicked);

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

                  QLabel#staff {
                      background-color: transparent;
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

void RepairProcessWidget::refreshTable()
{
    table->setRowCount(0);
    QSqlQuery query = db.executeSQL("SELECT id,reporter_name,report_time,fault,fault_address,reporter_phone,status,repairman_name,repairman_phone FROM repairs WHERE status = 0",{});
    int row = 0;
    while (query.next()) {
        table->insertRow(row);
        int colCount = table->columnCount();
        for(int col=0;col<colCount;col++) {
            QString val = query.value(col).toString();
            if(col == 6) {
                if(val == "0") val = "报修中";
                else if(val == "1") val = "已派单";
                else if(val == "2") val = "已结束";
                else val = "报修失败";
            }
            if(col == 9) {
                QCheckBox* checkBox = new QCheckBox();
                table->setCellWidget(row,col,checkBox);
                continue;
            }
            QTableWidgetItem *item = new QTableWidgetItem(val);
            table->setItem(row,col,item);
        }
        row++;
    }
    return;
}

void RepairProcessWidget::refreshComboBox()
{
    comboBox->clear();
    QSqlQuery query = db.executeSQL("SELECT id,name FROM users WHERE role = 1",{});
    while (query.next()) {
        int staff_id = query.value("id").toInt();
        QString staff_name = query.value("name").toString();
        comboBox->addItem(staff_name,staff_id);
    }
}

void RepairProcessWidget::sendClicked()
{
    int rowCount = table->rowCount();
    QSqlQuery query;
    for(int row=0;row<rowCount;row++) {
        QCheckBox* checkBox = qobject_cast<QCheckBox*>(table->cellWidget(row,9));
        if(checkBox && checkBox->isChecked()) {
            QTableWidgetItem* item = table->item(row,0);
            int id = item->text().toInt();
            int index = comboBox->currentIndex();
            QString repairman_name = comboBox->itemText(index);
            int repairman_id = comboBox->currentData().toInt();
            QString repairman_phone;
            query = db.executeSQL("SELECT phone FROM users WHERE id = ? ",{repairman_id});
            if(query.next()) repairman_phone = query.value("phone").toString();
            db.executeSQL("UPDATE repairs SET status = 1,repairman_id = ?,repairman_name = ?,repairman_phone = ? WHERE id = ?",{repairman_id,repairman_name,repairman_phone,id});
        }
    }
    QMessageBox::information(this,"派单成功","已指派工作人员！");
    refreshTable();
    return;
}

RepairProcessWidget::~RepairProcessWidget()
{
    delete ui;
}
