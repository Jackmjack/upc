#include "houseregistrationwidget.h"
#include "ui_houseregistrationwidget.h"
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

HouseRegistrationWidget::HouseRegistrationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HouseRegistrationWidget)
{
    ui->setupUi(this);

    //创建布局
    QFormLayout* formLayout = new QFormLayout();
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QHBoxLayout* hLayout = new QHBoxLayout();

    //创建标题标签
    QLabel *title = new QLabel("房产登记", this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");

    //创建按钮
    QPushButton* submit = new QPushButton("提交",this);

    //创建输入框
    nameEdit = new QLineEdit(this);
    houseNumberEdit = new QLineEdit(this);
    buildingNumberEdit = new QLineEdit(this);
    areaEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("输入业主姓名");
    houseNumberEdit->setPlaceholderText("输入房间号");
    buildingNumberEdit->setPlaceholderText("输入楼栋号");
    areaEdit->setPlaceholderText("输入小区名称");

    //创建弹簧
    QSpacerItem* spacer = new QSpacerItem(10,50,QSizePolicy::Fixed,QSizePolicy::Fixed);
    QSpacerItem* hspacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

    //创建按钮布局
    hLayout->addItem(hspacer);
    hLayout->addWidget(submit);

    // 创建表单布局
    formLayout->setLabelAlignment(Qt::AlignRight);
    formLayout->setHorizontalSpacing(10);
    formLayout->setVerticalSpacing(15);
    formLayout->addItem(spacer);
    formLayout->addRow("姓名：",nameEdit);
    formLayout->addRow("小区：", areaEdit);
    formLayout->addRow("楼栋：", buildingNumberEdit);
    formLayout->addRow("房间：",houseNumberEdit);

    //创建GroupBox
    QGroupBox* groupBox = new QGroupBox("房产信息", this);
    QSizePolicy policy = groupBox->sizePolicy();
    policy.setVerticalPolicy(QSizePolicy::Expanding);
    groupBox->setSizePolicy(policy);
    groupBox->setLayout(formLayout);

    // 创建主布局
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(title);
    mainLayout->addWidget(groupBox);
    mainLayout->addLayout(hLayout);
    setLayout(mainLayout);

    //连接按钮与槽函数
    connect(submit, &QPushButton::clicked, this, &HouseRegistrationWidget::onSubmitClicked);

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

void HouseRegistrationWidget::onSubmitClicked()
{
    QString name = nameEdit->text().trimmed();
    QString area = areaEdit->text().trimmed();
    QString building = buildingNumberEdit->text().trimmed();
    QString house = houseNumberEdit->text().trimmed();
    if (name.isEmpty() || area.isEmpty() || building.isEmpty() || house.isEmpty()) {
        QMessageBox::warning(this, "提示", "请填写完整信息！");
        return;
    }
    db.executeSQL("INSERT INTO properties (owner_name,house_number,building_number,area) VALUES (?,?,?,?)",{name,house,building,area});
    QMessageBox::information(this, "登记成功", "房产登记成功！\n房产ID已自动分配!");
    nameEdit->clear();
    areaEdit->clear();
    buildingNumberEdit->clear();
    houseNumberEdit->clear();
    return;
}

HouseRegistrationWidget::~HouseRegistrationWidget()
{
    delete ui;
}
