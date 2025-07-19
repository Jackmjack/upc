#include "staffmanagewidget.h"
#include "ui_staffmanagewidget.h"
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QSpacerItem>
#include <QMessageBox>

StaffManageWidget::StaffManageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StaffManageWidget)
{
    ui->setupUi(this);

    //创建布局
    QVBoxLayout *vLayout = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();

    //创建标题标签
    QLabel *title = new QLabel("物业工作人员维护",this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");

    //创建表
    table = new QTableWidget(this);
    table->setColumnCount(8);
    table->setHorizontalHeaderLabels({"ID","用户名","姓名","部门","岗位","电话","地址","就职情况"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);

    //创建按钮
    addButton = new QPushButton("新增",this);
    editButton = new QPushButton("编辑",this);
    deleteButton = new QPushButton("删除",this);
    refreshButton = new QPushButton("刷新",this);
    editButton->setDisabled(true);
    deleteButton->setDisabled(true);

    //创建弹簧
    QSpacerItem *hspacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

    //设置布局
    hLayout->addItem(hspacer);
    hLayout->addWidget(addButton);
    hLayout->addWidget(editButton);
    hLayout->addWidget(deleteButton);
    hLayout->addWidget(refreshButton);
    hLayout->setSpacing(4);
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
    connect(addButton,&QPushButton::clicked,this,&StaffManageWidget::addStaff);
    connect(editButton,&QPushButton::clicked,this,&StaffManageWidget::editStaff);
    connect(deleteButton,&QPushButton::clicked,this,&StaffManageWidget::deleteStaff);
    connect(refreshButton,&QPushButton::clicked,this,&StaffManageWidget::refreshTable);

    //编辑模式设置
    connect(table,&QTableWidget::itemSelectionChanged,this,&StaffManageWidget::onItemSelectionChangedEdit);
    connect(table,&QTableWidget::itemSelectionChanged,this,&StaffManageWidget::onItemSelectionChanged);
    connect(this,&StaffManageWidget::singleItemSelectedChangedEdit,editButton,&QPushButton::setEnabled);
    connect(this,&StaffManageWidget::singleItemSelectedChanged,deleteButton,&QPushButton::setEnabled);

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
                  QPushButton:disabled {
                      backgroud-color: gray;
                      color: lightgray;
                      border: 1px solid lightgray;
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

void StaffManageWidget::refreshTable()
{
    table->setRowCount(0);
    QSqlQuery query = db.executeSQL("SELECT id,username,name,department,position,phone,address,status FROM users WHERE role = 1",{});
    int row = 0;
    while (query.next()) {
        table->insertRow(row);
        for (int col=0;col<table->columnCount();col++) {
            QString val = query.value(col).toString();
            if(col == 7) {
                if(val == "1") val = "在职";
                else if(val == "0") val = "离职";
                else val = "未知";
            }
            QTableWidgetItem* item = new QTableWidgetItem(val);
            table->setItem(row,col,item);
        }
        row++;
    }
}

void StaffManageWidget::addStaff()
{
    emit newAddStaffWidget();
}

void StaffManageWidget::editStaff()
{
    QTableWidgetItem *item = this->getSingleSelectedItemEdit();
    QString oldText = item->text();
    if (!item) return;

    if(editButton->text() == "退出编辑") {
        table->closePersistentEditor(item);
        table->setSelectionMode(QAbstractItemView::ExtendedSelection);
        editButton->setText("编辑");
        refreshTable();
        addButton->setDisabled(false);
        deleteButton->setDisabled(false);
        refreshButton->setDisabled(false);
        return;
    }

    table->openPersistentEditor(item);
    editButton->setText("退出编辑");
    table->setSelectionMode(QAbstractItemView::NoSelection);
    addButton->setDisabled(true);
    deleteButton->setDisabled(true);
    refreshButton->setDisabled(true);

    if (QLineEdit *editor = qobject_cast<QLineEdit*>(table->cellWidget(item->row(), item->column()))) {
        editor->setFocus();
        connect(editor, &QLineEdit::returnPressed, this, [=]() {
            QString newText = editor->text();
            if (newText == oldText) {
                table->closePersistentEditor(item);
                table->setSelectionMode(QAbstractItemView::ExtendedSelection);
                editButton->setText("编辑");
                refreshTable();
                addButton->setDisabled(false);
                deleteButton->setDisabled(false);
                refreshButton->setDisabled(false);
                return;
            }

            int row = item->row();
            int col = item->column();
            QTableWidgetItem* idItem = table->item(row,0);
            int id = idItem->text().toInt();
            int val = -1;
            QSqlQuery query;
            if(col == 2) {
                query.prepare("UPDATE users SET name = ? WHERE id = ?");
            } else if (col == 3) {
                query.prepare("UPDATE users SET department = ? WHERE id = ?");
            } else if (col == 4) {
                query.prepare("UPDATE users SET position = ? WHERE id = ?");
            } else if (col == 5) {
                query.prepare("UPDATE users SET phone = ? WHERE id = ?");
            } else if (col == 6) {
                query.prepare("UPDATE users SET address = ? WHERE id = ?");
            } else if (col == 7) {
                query.prepare("UPDATE users SET status = ? WHERE id = ?");
                if(newText == "在职") val = 1;
                else if(newText == "离职") val = 0;
            }
            if(col != 7) query.addBindValue(newText);
            else query.addBindValue(val);
            query.addBindValue(id);
            if (query.exec()) {
                QMessageBox::information(this,"提示","修改成功！");
            } else {
                QMessageBox::warning(this,"错误","修改失败！");
            }

            table->closePersistentEditor(item);
            table->setSelectionMode(QAbstractItemView::ExtendedSelection);
            editButton->setText("编辑");
            refreshTable();
            addButton->setDisabled(false);
            deleteButton->setDisabled(false);
            refreshButton->setDisabled(false);

            disconnect(editor, &QLineEdit::returnPressed, this, nullptr);
        });
    }
}

void StaffManageWidget::deleteStaff()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this,"确认删除","确认删除该工作人员数据吗？此操作不可恢复！",QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if(reply == QMessageBox::No) {
        QMessageBox::information(this,"提示","已取消操作！");
        return;
    }
    QTableWidgetItem *item = this->getSingleSelectedItem();
    int row = item->row();
    QTableWidgetItem* idItem = table->item(row,0);
    int id = idItem->text().toInt();
    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE id = ?");
    query.addBindValue(id);
    if(query.exec()) {
        QMessageBox::information(this,"提示","删除成功！");
    } else {
        QMessageBox::warning(this,"错误","删除失败！");
    }
    refreshTable();
    return;
}

QTableWidgetItem* StaffManageWidget::getSingleSelectedItemEdit()
{
    QList<QTableWidgetItem*> selectedItems = table->selectedItems();
    if(selectedItems.size() == 1) {
        int col = selectedItems[0]->column();
        QString headerName = table->horizontalHeader()->model()->headerData(col,Qt::Horizontal).toString();
        if(headerName == "ID" || headerName == "用户名") return nullptr;
        else return selectedItems.first();
    }
    return nullptr;
}

QTableWidgetItem* StaffManageWidget::getSingleSelectedItem()
{
    QList<QTableWidgetItem*> selectedItems = table->selectedItems();
    if(selectedItems.size() == 1) {
        return selectedItems.first();
    }
    return nullptr;
}

void StaffManageWidget::onItemSelectionChangedEdit()
{
    bool hasSingle = (getSingleSelectedItemEdit() != nullptr);
    emit singleItemSelectedChangedEdit(hasSingle);
}

void StaffManageWidget::onItemSelectionChanged()
{
    bool hasSingle = (getSingleSelectedItem() != nullptr);
    emit singleItemSelectedChanged(hasSingle);
}

StaffManageWidget::~StaffManageWidget()
{
    delete ui;
}
