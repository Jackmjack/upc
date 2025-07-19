#include "managermainwindow.h"
#include "ui_managermainwindow.h"
#include <QDateTime>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

ManagerMainWindow::ManagerMainWindow(int userId,QString name,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ManagerMainWindow)
{
    ui->setupUi(this);
    this->userId = userId;
    this->name = name;

    //创建菜单栏
    setupMenus();

    //创建窗口树
    stackedWidget = new QStackedWidget(this);
    staffManageWidget = new StaffManageWidget(this);
    addStaffWidget = new AddStaffWidget(this);
    leaveApproveWidget = new LeaveApproveWidget(userId,name,this);
    attendanceWidget = new AttendanceWidget(this);
    sysInitializeWidget = new SysInitializeWidget(this);
    announce = new AddAnnouncementWidget(this);

    //将窗口添加到窗口树
    stackedWidget->addWidget(staffManageWidget);
    stackedWidget->addWidget(addStaffWidget);
    stackedWidget->addWidget(leaveApproveWidget);
    stackedWidget->addWidget(attendanceWidget);
    stackedWidget->addWidget(sysInitializeWidget);
    stackedWidget->addWidget(announce);
    stackedWidget->addWidget(ui->centralwidget);

    //设置CentralWidget
    setCentralWidget(stackedWidget);
    stackedWidget->setCurrentWidget(ui->centralwidget);

    //创建状态栏
    QStatusBar* statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    //增加时间标签
    QLabel* timeLabel = new QLabel(this);
    QTimer* timer = new QTimer(this);
    statusBar->addPermanentWidget(timeLabel);
    connect(timer,&QTimer::timeout,this,[timeLabel](){
        timeLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    });
    timer->start(1000);

    //增加信息标签
    QLabel *permanentLabel = new QLabel(QString("管理员id：%1").arg(this->userId),this);
    statusBar->addPermanentWidget(permanentLabel);
    QLabel *permanentLabel2 = new QLabel(QString("当前操作人员：%1").arg(this->name),this);
    statusBar->addPermanentWidget(permanentLabel2);

    //关联信号与槽
    connect(staffManageWidget,&StaffManageWidget::newAddStaffWidget,this,&ManagerMainWindow::showAddStaff);
    connect(addStaffWidget,&AddStaffWidget::backToStaffWidget,this,&ManagerMainWindow::showStaffManage);

    //创建标题标签
    QLabel *title = new QLabel("公告",this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");

    //创建表
    table = new QTableWidget(this);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"ID","发布时间","标题","详情"});
    table->setColumnHidden(0,true);
    table->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    table->setColumnWidth(1,170);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);

    //创建布局
    QVBoxLayout *vLayout = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();

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
    ui->centralwidget->setLayout(vLayout);

    //连接数据库
    if(!db.openDatabase("smartcommunity.db")) {
        QMessageBox::critical(this,"错误","数据库打开失败");
    }

    //刷新表
    refreshTable();

    //连接按钮与槽函数
    connect(addButton,&QPushButton::clicked,this,&ManagerMainWindow::addAnnouncement);
    connect(editButton,&QPushButton::clicked,this,&ManagerMainWindow::editAnnouncement);
    connect(deleteButton,&QPushButton::clicked,this,&ManagerMainWindow::deleteAnnouncement);
    connect(refreshButton,&QPushButton::clicked,this,&ManagerMainWindow::refreshTable);

    //编辑模式设置
    connect(table,&QTableWidget::itemSelectionChanged,this,&ManagerMainWindow::onItemSelectionChangedEdit);
    connect(table,&QTableWidget::itemSelectionChanged,this,&ManagerMainWindow::onItemSelectionChanged);
    connect(this,&ManagerMainWindow::singleItemSelectedChangedEdit,editButton,&QPushButton::setEnabled);
    connect(this,&ManagerMainWindow::singleItemSelectedChanged,deleteButton,&QPushButton::setEnabled);
    connect(announce,&AddAnnouncementWidget::backToAnnouncement,this,&ManagerMainWindow::showAnnouncement);

    //设置样式表
    setStyleSheet(R"(QLabel#title {
                      color: #000000;
                      font-size: 48px;
                      font-weight: bold;
                      font-family: "华文琥珀",sans-serif;
                      margin-bottom: 0px;
                      background-color: #f0f5ff;
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
                  })");
}

void ManagerMainWindow::refreshTable()
{
    table->setRowCount(0);
    QSqlQuery query = db.executeSQL("SELECT id,create_time,title,text FROM announcements",{});
    int row = 0;
    while (query.next()) {
        table->insertRow(row);
        for (int col=0;col<table->columnCount();col++) {
            QString val = query.value(col).toString();
            QTableWidgetItem* item = new QTableWidgetItem(val);
            table->setItem(row,col,item);
        }
        row++;
    }
}

//创建菜单栏函数
void ManagerMainWindow::setupMenus()
{
    QMenuBar *menubar = ui->menubar;

    //清空菜单栏
    menubar->clear();

    //设置菜单
    QMenu *staffMenu = menubar->addMenu("人事管理");
    QMenu *sysMenu = menubar->addMenu("系统设置");
    QMenu* announcement = menubar->addMenu("公告");

    //添加行为
    QAction *actStaffManage = staffMenu->addAction("物业工作人员维护");
    QAction *actLeaveApprove = staffMenu->addAction("请销假审批");
    QAction *actAttendance = staffMenu->addAction("出勤记录管理");
    QAction *actSysInitialize = sysMenu->addAction("系统初始化");

    //关联槽函数
    connect(actStaffManage,&QAction::triggered,this,&ManagerMainWindow::showStaffManage);
    connect(actLeaveApprove,&QAction::triggered,this,&ManagerMainWindow::showLeaveApprove);
    connect(actAttendance,&QAction::triggered,this,&ManagerMainWindow::showAttendance);
    connect(actSysInitialize,&QAction::triggered,this,&ManagerMainWindow::showSysInitialize);
    connect(announcement,&QMenu::aboutToShow,this,&ManagerMainWindow::showAnnouncement);
}

QTableWidgetItem* ManagerMainWindow::getSingleSelectedItem()
{
    QList<QTableWidgetItem*> selectedItems = table->selectedItems();
    if(selectedItems.size() == 1) {
        return selectedItems.first();
    }
    return nullptr;
}

QTableWidgetItem* ManagerMainWindow::getSingleSelectedItemEdit()
{
    QList<QTableWidgetItem*> selectedItems = table->selectedItems();
    if(selectedItems.size() == 1) {
        int col = selectedItems[0]->column();
        QString headerName = table->horizontalHeader()->model()->headerData(col,Qt::Horizontal).toString();
        if(headerName == "发布时间") return nullptr;
        else return selectedItems.first();
    }
    return nullptr;
}

void ManagerMainWindow::onItemSelectionChanged()
{
    bool hasSingle = (getSingleSelectedItem() != nullptr);
    emit singleItemSelectedChanged(hasSingle);
}

void ManagerMainWindow::onItemSelectionChangedEdit()
{
    bool hasSingle = (getSingleSelectedItemEdit() != nullptr);
    emit singleItemSelectedChangedEdit(hasSingle);
}

void ManagerMainWindow::addAnnouncement()
{
    stackedWidget->setCurrentWidget(announce);
}

void ManagerMainWindow::editAnnouncement()
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
            QSqlQuery query;
            if(col == 2) {
                query.prepare("UPDATE announcements SET title = ? WHERE id = ?");
            } else if (col == 3) {
                query.prepare("UPDATE announcements SET text = ? WHERE id = ?");
            }
            query.addBindValue(newText);
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

void ManagerMainWindow::deleteAnnouncement()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this,"确认删除","确认删除此条公告吗？此操作不可恢复！",QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if(reply == QMessageBox::No) {
        QMessageBox::information(this,"提示","已取消操作！");
        return;
    }
    QTableWidgetItem *item = this->getSingleSelectedItem();
    int row = item->row();
    QTableWidgetItem* idItem = table->item(row,0);
    int id = idItem->text().toInt();
    QSqlQuery query;
    query.prepare("DELETE FROM announcements WHERE id = ?");
    query.addBindValue(id);
    if(query.exec()) {
        QMessageBox::information(this,"提示","删除成功！");
    } else {
        QMessageBox::warning(this,"错误","删除失败！");
    }
    refreshTable();
    return;
}

void ManagerMainWindow::showStaffManage() {
    stackedWidget->setCurrentWidget(staffManageWidget);
}

void ManagerMainWindow::showAddStaff() {
    stackedWidget->setCurrentWidget(addStaffWidget);
}

void ManagerMainWindow::showLeaveApprove() {
    stackedWidget->setCurrentWidget(leaveApproveWidget);
}

void ManagerMainWindow::showAttendance() {
    stackedWidget->setCurrentWidget(attendanceWidget);
}

void ManagerMainWindow::showSysInitialize() {
    stackedWidget->setCurrentWidget(sysInitializeWidget);
}

void ManagerMainWindow::showAnnouncement() {
    stackedWidget->setCurrentWidget(ui->centralwidget);
}

ManagerMainWindow::~ManagerMainWindow()
{
    delete ui;
}
