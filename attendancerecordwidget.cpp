#include "attendancerecordwidget.h"
#include "ui_attendancerecordwidget.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QDateTime>

AttendanceRecordWidget::AttendanceRecordWidget(int userId,QString name,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AttendanceRecordWidget)
{
    ui->setupUi(this);
    this->userId = userId;
    this->name = name;

    //创建布局
    QVBoxLayout *vLayout = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();

    //创建标题标签
    QLabel *title = new QLabel("出勤登记",this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");

    //创建表
    table = new QTableWidget(this);
    table->setColumnCount(7);
    table->setHorizontalHeaderLabels({"记录编号","ID","姓名","日期","上班时间","下班时间","下班打卡"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);

    //创建按钮
    QPushButton* attend = new QPushButton("签到",this);

    //创建弹簧
    QSpacerItem *hspacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

    //设置布局
    hLayout->addItem(hspacer);
    hLayout->addWidget(attend);
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
    connect(attend,&QPushButton::clicked,this,&AttendanceRecordWidget::checkIn);

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

void AttendanceRecordWidget::refreshTable()
{
    table->setRowCount(0);
    QSqlQuery query = db.executeSQL("SELECT id,staff_id,name,date,start_time,end_time,end_status FROM attendances WHERE staff_id = ?",{userId});
    int row = 0;
    while (query.next()) {
        table->insertRow(row);
        int colCount = table->columnCount();
        for(int col=0;col<colCount-1;col++) {
            QString val = query.value(col).toString();
            QTableWidgetItem *item = new QTableWidgetItem(val);
            table->setItem(row,col,item);
        }
        if(query.value("end_status").toString() == "0") {
            QPushButton* checkOut = new QPushButton("签退",this);
            checkOut->setProperty("row",row);
            table->setCellWidget(row,6,checkOut);
            connect(checkOut,&QPushButton::clicked,this,&AttendanceRecordWidget::checkOut);
        } else if (query.value("end_status").toString() == "1") {
            table->setItem(row,6,new QTableWidgetItem("已签退"));
        }
        row++;
    }
    return;
}

void AttendanceRecordWidget::checkIn()
{
    QString date = QDateTime::currentDateTime().date().toString("yyyy-MM-dd");
    QString time = QDateTime::currentDateTime().time().toString("hh:mm:ss");
    QSqlQuery query = db.executeSQL("SELECT date FROM attendances WHERE date = ? AND staff_id = ?",{date,userId});
    if(query.next()) {
        QMessageBox::warning(this,"提示","请勿重复签到！");
    } else {
        db.executeSQL("INSERT INTO attendances (staff_id,name,date,start_time,start_status) VALUES (?,?,?,?,1)",{userId,name,date,time});
        QMessageBox::information(this,"提示","签到成功！");
        refreshTable();
    }
    return;
}

void AttendanceRecordWidget::checkOut()
{
    QPushButton* checkOut = qobject_cast<QPushButton*>(sender());
    int row = checkOut->property("row").toInt();
    int id = table->item(row,0)->text().toInt();
    QString time = QDateTime::currentDateTime().time().toString("hh:mm:ss");
    db.executeSQL("UPDATE attendances SET end_status = 1,end_time = ? WHERE id = ?",{time,id});
    QMessageBox::information(this,"提示","签退成功！");
    refreshTable();
    return;
}

AttendanceRecordWidget::~AttendanceRecordWidget()
{
    delete ui;
}
