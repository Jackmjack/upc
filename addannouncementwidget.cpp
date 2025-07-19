#include "addannouncementwidget.h"
#include "ui_addannouncementwidget.h"
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDateTime>

AddAnnouncementWidget::AddAnnouncementWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddAnnouncementWidget)
{
    ui->setupUi(this);

    //创建布局
    QFormLayout* formLayout = new QFormLayout();
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QHBoxLayout* hLayout = new QHBoxLayout();

    //创建标题标签
    QLabel *title = new QLabel("公告发布页", this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");

    //创建按钮
    QPushButton* submit = new QPushButton("发布",this);
    QPushButton* back = new QPushButton("返回",this);

    //创建输入框
    titleEdit = new QLineEdit(this);
    textEdit = new QTextEdit(this);
    titleEdit->setPlaceholderText("输入公告标题");
    textEdit->setPlaceholderText("输入公告内容");

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
    formLayout->addRow("标题：",titleEdit);
    formLayout->addRow("内容：", textEdit);

    //创建GroupBox
    QGroupBox* groupBox = new QGroupBox("公告信息", this);
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
    connect(submit, &QPushButton::clicked, this, &AddAnnouncementWidget::onSubmitClicked);
    connect(back,&QPushButton::clicked,this,&AddAnnouncementWidget::onBackClicked);

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
                  })");
}

void AddAnnouncementWidget::onSubmitClicked()
{
    QString title = titleEdit->text().trimmed();
    QString text = textEdit->toPlainText();
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
    if(title.isEmpty() || text.isEmpty() || time.isEmpty()) {
        QMessageBox::warning(this, "错误", "请输入完整内容！");
        return;
    }
    db.executeSQL("INSERT INTO announcements (title,text,create_time) VALUES (?,?,?)",{title,text,time});
    QMessageBox::information(this,"发布成功","已发布公告！");
    titleEdit->clear();
    textEdit->clear();
    emit backToAnnouncement();
    return;
}

void AddAnnouncementWidget::onBackClicked()
{
    titleEdit->clear();
    textEdit->clear();
    emit backToAnnouncement();
}

AddAnnouncementWidget::~AddAnnouncementWidget()
{
    delete ui;
}
