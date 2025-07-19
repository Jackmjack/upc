#include "repaircommentwidget.h"
#include "ui_repaircommentwidget.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

RepairCommentWidget::RepairCommentWidget(int userId,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RepairCommentWidget)
{
    ui->setupUi(this);
    this->userId = userId;

    //创建布局
    QVBoxLayout *vLayout = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();

    //创建标题标签
    QLabel *title = new QLabel("维修评价",this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");

    //创建标签
    QLabel* score = new QLabel("选择评分：",this);
    QLabel* commentLabel = new QLabel("填写评价：",this);

    //创建下拉框
    comboBox = new QComboBox(this);
    comboBox->addItem("5",5);
    comboBox->addItem("4",4);
    comboBox->addItem("3",3);
    comboBox->addItem("2",2);
    comboBox->addItem("1",1);

    //创建输入框
    comment = new QLineEdit(this);
    comment->setPlaceholderText("请输入评价...");

    //创建表
    table = new QTableWidget(this);
    table->setColumnCount(9);
    table->setHorizontalHeaderLabels({"报修单号","报修时间","报修原因","报修状态","维修人","维修人联系电话","业主评分","业主评价","评价"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);

    //设置布局
    hLayout->addWidget(score);
    hLayout->addWidget(comboBox);
    hLayout->addWidget(commentLabel);
    hLayout->addWidget(comment);
    hLayout->setSpacing(4);
    vLayout->addWidget(title);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(table);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setSpacing(0);
    setLayout(vLayout);

    //连接数据库
    if(!db.openDatabase("smartcommunity.db")) {
        QMessageBox::critical(this,"错误","数据库打开失败");
    }

    //刷新表
    refreshTable();

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

void RepairCommentWidget::refreshTable()
{
    table->setRowCount(0);
    QSqlQuery query = db.executeSQL("SELECT id,report_time,fault,status,repairman_name,repairman_phone,score,comment FROM repairs WHERE reporter_id = ?",{userId});
    int row = 0;
    while (query.next()) {
        table->insertRow(row);
        int colCount = table->columnCount();
        for(int col=0;col<colCount;col++) {
            QString val = query.value(col).toString();
            if(col == 3) {
                if(val == "0") val = "报修中";
                else if(val == "1") val = "已派单";
                else if(val == "2") val = "已结束";
                else val = "报修失败";
            }
            if(col == 8 && query.value("status").toString() == "1") {
                QPushButton* commentButton = new QPushButton("评价",this);
                commentButton->setProperty("row",row);
                connect(commentButton,&QPushButton::clicked,this,&RepairCommentWidget::commentClicked);
                table->setCellWidget(row,col,commentButton);
                continue;
            } else if (col == 8 && query.value("status").toString() == "2") {
                QTableWidgetItem *item = new QTableWidgetItem("已评价");
                table->setItem(row,col,item);
                continue;
            }
            QTableWidgetItem *item = new QTableWidgetItem(val);
            table->setItem(row,col,item);
        }
        row++;
    }
    return;
}

void RepairCommentWidget::commentClicked()
{
    QPushButton* commentButton = qobject_cast<QPushButton*>(sender());
    int row = commentButton->property("row").toInt();
    int id = table->item(row,0)->text().toInt();
    int score = comboBox->currentData().toInt();
    QString commentString = comment->text();
    db.executeSQL("UPDATE repairs SET status = 2,score = ?,comment = ? WHERE id = ?",{score,commentString,id});
    QMessageBox::information(this,"评价成功","已收到评价，感谢您的反馈！");
    comment->clear();
    refreshTable();
    return;
}

RepairCommentWidget::~RepairCommentWidget()
{
    delete ui;
}
