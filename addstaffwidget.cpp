#include "addstaffwidget.h"
#include "ui_addstaffwidget.h"
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

AddStaffWidget::AddStaffWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddStaffWidget)
{
    ui->setupUi(this);

    //创建布局
    QFormLayout* formLayout = new QFormLayout();
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QHBoxLayout* hLayout = new QHBoxLayout();

    //创建标题标签
    QLabel *title = new QLabel("新增工作人员", this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");

    //创建按钮
    QPushButton* submit = new QPushButton("提交",this);
    QPushButton* back = new QPushButton("返回",this);

    //创建输入框
    nameEdit = new QLineEdit(this);
    usernameEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);
    passwordEdit2 = new QLineEdit(this);
    departmentEdit = new QLineEdit(this);
    positionEdit = new QLineEdit(this);
    phoneEdit = new QLineEdit(this);
    addressEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("输入工作人员姓名");
    usernameEdit->setPlaceholderText("输入工作人员账号");
    passwordEdit->setPlaceholderText("输入工作人员密码");
    passwordEdit2->setPlaceholderText("再次输入工作人员密码");
    departmentEdit->setPlaceholderText("输入工作人员部门");
    positionEdit->setPlaceholderText("输入工作人员岗位");
    phoneEdit->setPlaceholderText("输入工作人员电话");
    addressEdit->setPlaceholderText("输入工作人员地址");

    //隐藏密码
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit2->setEchoMode(QLineEdit::Password);

    //创建弹簧
    QSpacerItem* spacer = new QSpacerItem(10,50,QSizePolicy::Fixed,QSizePolicy::Fixed);
    QSpacerItem* hspacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

    //创建按钮布局
    hLayout->addItem(hspacer);
    hLayout->addWidget(submit);
    hLayout->addWidget(back);
    hLayout->setSpacing(4);

    // 创建表单布局
    formLayout->setLabelAlignment(Qt::AlignRight);
    formLayout->setHorizontalSpacing(10);
    formLayout->setVerticalSpacing(15);
    formLayout->addItem(spacer);
    formLayout->addRow("姓名：",nameEdit);
    formLayout->addRow("用户名：", usernameEdit);
    formLayout->addRow("密码：", passwordEdit);
    formLayout->addRow("确认密码：",passwordEdit2);
    formLayout->addRow("部门：", departmentEdit);
    formLayout->addRow("岗位：",positionEdit);
    formLayout->addRow("电话：", phoneEdit);
    formLayout->addRow("地址：", addressEdit);

    //创建GroupBox
    QGroupBox* groupBox = new QGroupBox("工作人员信息", this);
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
    connect(submit, &QPushButton::clicked, this, &AddStaffWidget::onSubmitClicked);
    connect(back,&QPushButton::clicked,this,&AddStaffWidget::onBackClicked);

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

void AddStaffWidget::onSubmitClicked()
{
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();
    QString password2 = passwordEdit2->text();
    QString name = nameEdit->text().trimmed();
    QString department = departmentEdit->text().trimmed();
    QString position = positionEdit->text().trimmed();
    QString phone = phoneEdit->text().trimmed();
    QString address = addressEdit->text().trimmed();

    if(password != password2) {
        QMessageBox::warning(this, "错误", "两次密码输入不一致！");
        return;
    }

    QSqlQuery query = db.executeSQL("SELECT id FROM users WHERE username = ?",{username});
    if (query.next()) {
        QMessageBox::warning(this, "错误", "该用户名已被注册！");
        return;
    }
    if (username.isEmpty() || password.isEmpty() || name.isEmpty()|| department.isEmpty() || position.isEmpty() || phone.isEmpty() || address.isEmpty()) {
        QMessageBox::warning(this, "提示", "请填写完整信息！");
        return;
    }
    db.executeSQL("INSERT INTO users (username,password,role,name,department,position,status,phone,address) VALUES (?,?,1,?,?,?,1,?,?)",{username,password,name,department,position,phone,address});
    QMessageBox::information(this, "注册成功", "工作人员新增成功！\n员工ID已自动分配!");
    nameEdit->clear();
    usernameEdit->clear();
    passwordEdit->clear();
    passwordEdit2->clear();
    departmentEdit->clear();
    positionEdit->clear();
    phoneEdit->clear();
    addressEdit->clear();
    emit backToStaffWidget();
    return;
}

void AddStaffWidget::onBackClicked()
{
    nameEdit->clear();
    usernameEdit->clear();
    passwordEdit->clear();
    passwordEdit2->clear();
    departmentEdit->clear();
    positionEdit->clear();
    phoneEdit->clear();
    addressEdit->clear();
    emit backToStaffWidget();
    return;
}

AddStaffWidget::~AddStaffWidget()
{
    delete ui;
}
