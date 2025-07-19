#include "faultreportwidget.h"
#include "ui_faultreportwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QDateTime>

FaultReportWidget::FaultReportWidget(int userId,QString name,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FaultReportWidget)
{
    ui->setupUi(this);
    this->userId = userId;
    this->name = name;

    //创建布局
    QVBoxLayout *vLayout = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();

    //创建标题标签
    QLabel *title = new QLabel("故障报修",this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");

    //创建标签
    QLabel* addressLabel = new QLabel("地址：",this);
    QLabel* faultLabel = new QLabel("故障描述：",this);

    //创建输入框
    address = new QLineEdit(this);
    fault = new QTextEdit(this);

    //设置输入框格式
    fault->setLineWrapMode(QTextEdit::WidgetWidth);
    fault->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    fault->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    fault->setPlaceholderText("请详细描述报修内容...");
    address->setPlaceholderText("请输入地址...");

    //创建按钮
    QPushButton* submit = new QPushButton("报修",this);

    //创建弹簧
    QSpacerItem *hspacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

    //设置布局
    hLayout->addItem(hspacer);
    hLayout->addWidget(submit);
    vLayout->addWidget(title);
    vLayout->addWidget(addressLabel);
    vLayout->addWidget(address);
    vLayout->addWidget(faultLabel);
    vLayout->addWidget(fault);
    vLayout->addLayout(hLayout);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setSpacing(0);
    setLayout(vLayout);

    //连接数据库
    if(!db.openDatabase("smartcommunity.db")) {
        QMessageBox::critical(this,"错误","数据库打开失败");
    }

    //连接按钮与槽函数
    connect(submit,&QPushButton::clicked,this,&FaultReportWidget::submitReport);

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

                  QLabel {
                      color: #000000;
                      font-size: 22px;
                      font-weight: bold;
                      font-family: "Microsoft YaHei",sans-serif;
                      margin-bottom: 0px;
                  }

                  QTextEdit {
                      border: 1px solid #CCCCCC;
                      border-radius: 4px;
                      padding: 0px 4px;
                      font-size: 20px;
                      background-color: white;
                  }
                  QTextEdit:focus {
                      border-color: #165DFF;
                      outline: none;
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

void FaultReportWidget::submitReport()
{
    QSqlQuery query = db.executeSQL("SELECT phone FROM users WHERE id = ?",{userId});
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString faultString = fault->toPlainText();
    QString addressString = address->text();
    if(time.isEmpty() || faultString.isEmpty() || addressString.isEmpty()) {
        QMessageBox::warning(this, "错误", "请输入完整内容！");
    }
    QString phone;
    if(query.next()) phone = query.value("phone").toString();
    else return;
    db.executeSQL("INSERT INTO repairs (reporter_id,reporter_name,report_time,fault,fault_address,reporter_phone,status) VALUES (?,?,?,?,?,?,0)",{userId,name,time,faultString,addressString,phone});
    QMessageBox::information(this,"提交成功","已提交报修订单！");
    address->clear();
    fault->clear();
    return;
}

FaultReportWidget::~FaultReportWidget()
{
    delete ui;
}
