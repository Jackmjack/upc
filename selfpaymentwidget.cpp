#include "selfpaymentwidget.h"
#include "ui_selfpaymentwidget.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QDateTime>

SelfPaymentWidget::SelfPaymentWidget(int userId,QString name,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelfPaymentWidget)
{
    ui->setupUi(this);
    this->userId = userId;
    this->name = name;

    //创建布局
    QFormLayout* formLayout = new QFormLayout();
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QHBoxLayout* hLayout = new QHBoxLayout();

    //创建标题标签
    QLabel *title = new QLabel("自助缴费", this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");

    //按钮
    QPushButton* confirmBtn = new QPushButton("确认缴费", this);

    //创建下拉框
    itemComboBox = new QComboBox(this);

    //创建输入框
    quantityEdit = new QLineEdit("1", this);
    amountEdit = new QLineEdit(this);
    amountEdit->setReadOnly(true);
    amountEdit->setStyleSheet("background-color: #e0e0e0;");
    amountEdit->setPlaceholderText("请先选择缴费数量");

    //创建弹簧
    QSpacerItem* spacer = new QSpacerItem(10,50,QSizePolicy::Fixed,QSizePolicy::Fixed);
    QSpacerItem* hspacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

    // 创建表单布局
    formLayout->setLabelAlignment(Qt::AlignRight);
    formLayout->setHorizontalSpacing(10);
    formLayout->setVerticalSpacing(15);
    formLayout->addItem(spacer);
    formLayout->addRow("缴费项目：", itemComboBox);
    formLayout->addRow("数量：",quantityEdit);
    formLayout->addRow("金额：",amountEdit);
    formLayout->addRow(hLayout);
    formLayout->setContentsMargins(10,0,10,10);

    //创建GroupBox
    QGroupBox* groupBox = new QGroupBox("缴费信息", this);
    QSizePolicy policy = groupBox->sizePolicy();
    policy.setVerticalPolicy(QSizePolicy::Expanding);
    groupBox->setSizePolicy(policy);
    groupBox->setLayout(formLayout);

    //设置布局
    hLayout->addItem(hspacer);
    hLayout->addWidget(confirmBtn);
    hLayout->setSpacing(4);
    mainLayout->addWidget(title);
    mainLayout->addWidget(groupBox);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //连接按钮与槽函数
    connect(itemComboBox, &QComboBox::currentTextChanged, this, &SelfPaymentWidget::calculatePay);
    connect(quantityEdit, &QLineEdit::textChanged, this, &SelfPaymentWidget::calculatePay);
    connect(confirmBtn, &QPushButton::clicked, this, &SelfPaymentWidget::confirmPayment);

    //连接数据库
    if(!db.openDatabase("smartcommunity.db")) {
        QMessageBox::critical(this,"错误","数据库打开失败");
    }

    //刷新下拉框
    loadPaymentItems();

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

void SelfPaymentWidget::loadPaymentItems()
{
    itemComboBox->clear();
    QSqlQuery query = db.executeSQL("SELECT id,item_name FROM paymentitems",{});
    while (query.next()) {
        int id = query.value("id").toInt();
        QString name = query.value("item_name").toString();
        itemComboBox->addItem(name,id);
    }
}

void SelfPaymentWidget::calculatePay()
{
    QString itemName = itemComboBox->currentText();
    QString quantityStr = quantityEdit->text();
    int id = itemComboBox->currentData().toInt();
    if (itemName.isEmpty() || quantityStr.isEmpty()) {
        amountEdit->clear();
        return;
    }
    QSqlQuery query = db.executeSQL("SELECT price FROM paymentitems WHERE id = ?",{id});
    if (query.next()) {
        double price = query.value("price").toDouble();
        double quantity = quantityStr.toDouble();
        double amount = price * quantity;
        amountEdit->setText(QString::number(amount, 'f', 2));
    } else {
        amountEdit->clear();
        QMessageBox::warning(this, "错误", "获取项目价格失败");
    }
}

void SelfPaymentWidget::confirmPayment()
{
    QString itemName = itemComboBox->currentText();
    QString quantityStr = quantityEdit->text().trimmed();
    QString amountStr = amountEdit->text().trimmed();
    if (itemName.isEmpty() || quantityStr.isEmpty() || amountStr.isEmpty()) {
        QMessageBox::warning(this, "错误", "请填写完整的缴费信息！");
        return;
    }
    int itemId = itemComboBox->currentData().toInt();
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    db.executeSQL("INSERT INTO paymentrecords (pay_name,owner_id,item_id,quantity,amount,pay_time,pay_method) VALUES (?,?,?,?,?,?,1)",{name,userId,itemId,quantityStr.toDouble(),amountStr.toDouble(),time});
    QMessageBox::information(this, "成功", "缴费记录添加成功");
    quantityEdit->setText("1");
    amountEdit->clear();
    return;
}

SelfPaymentWidget::~SelfPaymentWidget()
{
    delete ui;
}
