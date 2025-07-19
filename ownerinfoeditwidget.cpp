#include "ownerinfoeditwidget.h"
#include "ui_ownerinfoeditwidget.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QMessageBox>

OwnerInfoEditWidget::OwnerInfoEditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OwnerInfoEditWidget)
{
    ui->setupUi(this);

    //创建布局
    QFormLayout* formLayout = new QFormLayout();
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QHBoxLayout* hLayout = new QHBoxLayout();

    //创建标题标签
    QLabel *title = new QLabel("业主信息修改", this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");

    //创建按钮
    QPushButton* enter = new QPushButton("确认修改",this);

    //创建输入框
    usernameEdit = new QLineEdit(this);
    valueEdit = new QLineEdit(this);
    fieldEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("输入要修改的用户名");
    valueEdit->setPlaceholderText("输入要改为的值");
    fieldEdit->setPlaceholderText("输入要修改的字段(password,name,phone,address)");

    //创建弹簧
    QSpacerItem* spacer = new QSpacerItem(10,50,QSizePolicy::Fixed,QSizePolicy::Fixed);
    QSpacerItem* hspacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

    //创建按钮布局
    hLayout->addItem(hspacer);
    hLayout->addWidget(enter);

    // 创建表单布局
    formLayout->setLabelAlignment(Qt::AlignRight);
    formLayout->setHorizontalSpacing(10);
    formLayout->setVerticalSpacing(15);
    formLayout->addItem(spacer);
    formLayout->addRow("用户名：", usernameEdit);
    formLayout->addRow("字段名：", fieldEdit);
    formLayout->addRow("值：", valueEdit);

    //创建GroupBox
    QGroupBox* groupBox = new QGroupBox("修改信息", this);
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
    connect(enter, &QPushButton::clicked, this, &OwnerInfoEditWidget::enterClicked);

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

void OwnerInfoEditWidget::enterClicked()
{
    QString username = usernameEdit->text().trimmed();
    QString field = fieldEdit->text().trimmed();
    QString value = valueEdit->text().trimmed();

    if (username.isEmpty() || field.isEmpty() || value.isEmpty()) {
        QMessageBox::warning(this, "提示", "请填写完整信息！");
        return;
    }

    // 检查字段名是否合法
    QStringList validFields = {"name","password","phone","address"};
    if (!validFields.contains(field)) {
        QMessageBox::warning(this, "错误", "无效的字段名！\n合法字段：name,password, phone, address");
        return;
    }

    // 检查用户是否为业主
    QSqlQuery query = db.getUserByUsername(username);
    if (query.next()) {
        int role = query.value("role").toInt();
        if (role != 2) {
            QMessageBox::warning(this, "错误", "该用户不是业主，没有修改权限！");
            return;
        }
    } else {
        QMessageBox::warning(this, "错误", "未找到该用户！");
        return;
    }

    if(field == "name") query.prepare("UPDATE users SET name = ? WHERE username = ?");
    else if (field == "password") query.prepare("UPDATE users SET password = ? WHERE username = ?");
    else if (field == "phone") query.prepare("UPDATE users SET phone = ? WHERE username = ?");
    else if (field == "address") query.prepare("UPDATE users SET address = ? WHERE username = ?");
    query.addBindValue(value);
    query.addBindValue(username);
    if(query.exec()) {
        QMessageBox::information(this, "成功", "业主信息修改成功！");
        usernameEdit->clear();
        fieldEdit->clear();
        valueEdit->clear();
    } else {
        QMessageBox::warning(this, "失败", "业主信息修改失败！");
    }
}

OwnerInfoEditWidget::~OwnerInfoEditWidget()
{
    delete ui;
}
