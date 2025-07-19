#include "caridentificationwidget.h"
#include "ui_caridentificationwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QSpacerItem>
#include <QDateTime>

CarIdentificationWidget::CarIdentificationWidget(int userId,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CarIdentificationWidget)
{
    ui->setupUi(this);
    this->userId = userId;

    //创建布局
    QFormLayout* formLayout = new QFormLayout();
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QHBoxLayout* hLayout = new QHBoxLayout();

    //创建标题标签
    QLabel *title = new QLabel("车辆进入识别", this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");

    //创建输入框
    carNumEdit = new QLineEdit(this);
    carNumEdit->setPlaceholderText("请输入车牌号（例如：粤A12345）");

    //按钮
    QPushButton* in = new QPushButton("进场", this);
    QPushButton* out = new QPushButton("出场", this);

    //创建弹簧
    QSpacerItem* spacer = new QSpacerItem(10,100,QSizePolicy::Fixed,QSizePolicy::Fixed);
    QSpacerItem* hspacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

    // 创建表单布局
    formLayout->setLabelAlignment(Qt::AlignRight);
    formLayout->setHorizontalSpacing(10);
    formLayout->setVerticalSpacing(15);
    formLayout->addItem(spacer);
    formLayout->addRow("车牌号：", carNumEdit);
    formLayout->addRow(hLayout);
    formLayout->setContentsMargins(10,0,10,10);

    //创建GroupBox
    QGroupBox* groupBox = new QGroupBox("车辆信息", this);
    QSizePolicy policy = groupBox->sizePolicy();
    policy.setVerticalPolicy(QSizePolicy::Expanding);
    groupBox->setSizePolicy(policy);
    groupBox->setLayout(formLayout);

    //设置布局
    hLayout->addItem(hspacer);
    hLayout->addWidget(in);
    hLayout->addWidget(out);
    hLayout->setSpacing(4);
    mainLayout->addWidget(title);
    mainLayout->addWidget(groupBox);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //连接按钮与槽函数
    connect(in,&QPushButton::clicked,this,&CarIdentificationWidget::onInClicked);
    connect(out,&QPushButton::clicked,this,&CarIdentificationWidget::onOutClicked);

    //连接数据库
    if(!db.openDatabase("smartcommunity.db")) {
        QMessageBox::critical(this,"错误","数据库打开失败");
    }

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

void CarIdentificationWidget::onInClicked()
{
    QString carNum = carNumEdit->text().trimmed();
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    int id = -1;
    int status = -1;
    QSqlQuery query = db.executeSQL("SELECT id FROM parking WHERE car_number = ? AND isinside = 0",{carNum});
    if(query.next()) {
        QMessageBox::warning(this,"错误","该车辆已经入场！");
        return;
    }
    query = db.executeSQL("SELECT id FROM cars WHERE car_id = ?",{carNum});
    if(query.next()) {
        id = query.value("id").toInt();
        status = 0;
        QMessageBox::information(this,"欢迎","亲爱的业主，欢迎进入停车场！");
    } else {
        status = 1;
        QMessageBox::information(this,"提示","检测到非本小区车辆，进入将产生收费(4元/小时)！");
    }
    db.executeSQL("INSERT INTO parking (driver_id,car_id,car_number,in_time,status) VALUES (?,?,?,?,?)",{userId,id,carNum,time,status});
    QMessageBox::information(this,"欢迎","已放行！");
    carNumEdit->clear();
    return;
}

void CarIdentificationWidget::onOutClicked()
{
    QString carNum = carNumEdit->text().trimmed();
    QString outTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QSqlQuery query = db.executeSQL("SELECT id,in_time,status FROM parking WHERE car_number = ? AND isinside = 0",{carNum});
    if(query.next()) {
        QString inTime = query.value("in_time").toString();
        int status = query.value("status").toInt();
        int id = query.value("id").toInt();
        if(status == 0) {
            db.executeSQL("UPDATE parking SET out_time = ?,isinside = 1 WHERE id = ?",{outTime,id});
            QMessageBox::information(this,"放行","祝您一路平安！");
            carNumEdit->clear();
            return;
        } else {
            QString format = "yyyy-MM-dd hh:mm:ss";
            QDateTime time1 = QDateTime::fromString(inTime,format);
            QDateTime time2 = QDateTime::fromString(outTime,format);
            qint64 seconds = time1.secsTo(time2);
            int hours = int(seconds/3600);
            int cost = hours*4;
            QString fee = QString::number(cost);
            db.executeSQL("UPDATE parking SET out_time = ?,isinside = 1 WHERE id = ?",{outTime,id});
            QMessageBox::information(this,"停车费","已产生"+fee+"元停车费，请及时支付！");
            carNumEdit->clear();
            return;
        }
    } else {
        QMessageBox::warning(this,"错误","未查询到入场记录！请联系工作人员！");
        carNumEdit->clear();
        return;
    }
}

CarIdentificationWidget::~CarIdentificationWidget()
{
    delete ui;
}
