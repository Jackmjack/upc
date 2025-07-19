#include "staffmainwindow.h"
#include "ui_staffmainwindow.h"
#include <QDateTime>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

StaffMainWindow::StaffMainWindow(int userId,QString name,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StaffMainWindow)
{
    ui->setupUi(this);
    this->userId = userId;
    this->name = name;

    //创建菜单栏
    setupMenus();

    //创建窗口树
    stackedWidget = new QStackedWidget(this);
    monthlyAttendance = new MonthlyAttendanceWidget(userId,this);
    leave = new LeaveWidget(userId,name,this);
    attendanceRecord = new AttendanceRecordWidget(userId,name,this);
    ownerRecord = new OwnerRecordWidget(this);
    ownerInfoEdit = new OwnerInfoEditWidget(this);
    ownerInfoQuery = new OwnerInfoQueryWidget(this);
    houseRegistration = new HouseRegistrationWidget(this);
    carRecord = new CarRecordWidget(this);
    parkingManage = new ParkingManageWidget(this);
    parkingRent = new ParkingRentWidget(name,this);
    priceMaintenance = new PriceMaintenanceWidget(this);
    offlinePay = new OfflinePayWidget(userId,this);
    paymentQuery = new PaymentQueryWidget(this);
    warrantyQuery = new WarrantyQueryWidget(this);
    repairProcess = new RepairProcessWidget(this);
    announce = new AddAnnouncementWidget(this);

    //将窗口添加到窗口树
    stackedWidget->addWidget(monthlyAttendance);
    stackedWidget->addWidget(leave);
    stackedWidget->addWidget(attendanceRecord);
    stackedWidget->addWidget(ownerRecord);
    stackedWidget->addWidget(ownerInfoEdit);
    stackedWidget->addWidget(ownerInfoQuery);
    stackedWidget->addWidget(houseRegistration);
    stackedWidget->addWidget(carRecord);
    stackedWidget->addWidget(parkingManage);
    stackedWidget->addWidget(parkingRent);
    stackedWidget->addWidget(priceMaintenance);
    stackedWidget->addWidget(offlinePay);
    stackedWidget->addWidget(paymentQuery);
    stackedWidget->addWidget(warrantyQuery);
    stackedWidget->addWidget(repairProcess);
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
    QLabel *permanentLabel = new QLabel(QString("工作人员id：%1").arg(this->userId),this);
    statusBar->addPermanentWidget(permanentLabel);
    QLabel *permanentLabel2 = new QLabel(QString("当前操作人员：%1").arg(this->name),this);
    statusBar->addPermanentWidget(permanentLabel2);

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
    connect(addButton,&QPushButton::clicked,this,&StaffMainWindow::addAnnouncement);
    connect(editButton,&QPushButton::clicked,this,&StaffMainWindow::editAnnouncement);
    connect(deleteButton,&QPushButton::clicked,this,&StaffMainWindow::deleteAnnouncement);
    connect(refreshButton,&QPushButton::clicked,this,&StaffMainWindow::refreshTable);

    //编辑模式设置
    connect(table,&QTableWidget::itemSelectionChanged,this,&StaffMainWindow::onItemSelectionChangedEdit);
    connect(table,&QTableWidget::itemSelectionChanged,this,&StaffMainWindow::onItemSelectionChanged);
    connect(this,&StaffMainWindow::singleItemSelectedChangedEdit,editButton,&QPushButton::setEnabled);
    connect(this,&StaffMainWindow::singleItemSelectedChanged,deleteButton,&QPushButton::setEnabled);
    connect(announce,&AddAnnouncementWidget::backToAnnouncement,this,&StaffMainWindow::showAnnouncement);

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

void StaffMainWindow::refreshTable()
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
void StaffMainWindow::setupMenus()
{
    QMenuBar *menubar = ui->menubar;

    //清空菜单栏
    menubar->clear();

    //设置菜单
    QMenu* staffMenu = menubar->addMenu("人事管理");
    QMenu* ownerMenu = menubar->addMenu("业主管理");
    QMenu* parkingMenu = menubar->addMenu("车位管理");
    QMenu* paymentMenu = menubar->addMenu("缴费管理");
    QMenu* repairMenu = menubar->addMenu("维修管理");
    QMenu* announcement = menubar->addMenu("公告");

    //添加行为
    QAction* actMonthlyAttendance = staffMenu->addAction("月度出勤");
    QAction* actLeave = staffMenu->addAction("请销假申请");
    QAction* actAttendanceRecord = staffMenu->addAction("出勤登记");
    QAction* actOwnerRecord = ownerMenu->addAction("业主登记");
    QAction* actOwnerInfoEdit = ownerMenu->addAction("业主信息修改");
    QAction* actOwnerInfoQuery = ownerMenu->addAction("业主信息查询");
    QAction* actHouseRegistration = ownerMenu->addAction("房产登记");
    QAction* actCarRecord = parkingMenu->addAction("车辆登记");
    QAction* actParkingManage = parkingMenu->addAction("车位管理");
    QAction* actParkingRent = parkingMenu->addAction("车位承租");
    QAction* actPriceMaintenance = paymentMenu->addAction("价格维护");
    QAction* actOfflinePay = paymentMenu->addAction("线下缴费");
    QAction* actPaymentQuery = paymentMenu->addAction("缴费查询");
    QAction* actWarrantyQuery = repairMenu->addAction("保修查询");
    QAction* actRepairProcess = repairMenu->addAction("维修过程");

    //关联槽函数
    connect(actMonthlyAttendance,&QAction::triggered,this,&StaffMainWindow::showMonthlyAttendance);
    connect(actLeave,&QAction::triggered,this,&StaffMainWindow::showLeave);
    connect(actAttendanceRecord,&QAction::triggered,this,&StaffMainWindow::showAttendanceRecord);
    connect(actOwnerRecord,&QAction::triggered,this,&StaffMainWindow::showOwnerRecord);
    connect(actOwnerInfoEdit,&QAction::triggered,this,&StaffMainWindow::showOwnerInfoEdit);
    connect(actOwnerInfoQuery,&QAction::triggered,this,&StaffMainWindow::showOwnerInfoQuery);
    connect(actHouseRegistration,&QAction::triggered,this,&StaffMainWindow::showHouseRegistration);
    connect(actCarRecord,&QAction::triggered,this,&StaffMainWindow::showCarRecord);
    connect(actParkingManage,&QAction::triggered,this,&StaffMainWindow::showParkingManage);
    connect(actParkingRent,&QAction::triggered,this,&StaffMainWindow::showParkingRent);
    connect(actPriceMaintenance,&QAction::triggered,this,&StaffMainWindow::showPriceMaintenance);
    connect(actOfflinePay,&QAction::triggered,this,&StaffMainWindow::showOfflinePay);
    connect(actPaymentQuery,&QAction::triggered,this,&StaffMainWindow::showPaymentQuery);
    connect(actWarrantyQuery,&QAction::triggered,this,&StaffMainWindow::showWarrantyQuery);
    connect(actRepairProcess,&QAction::triggered,this,&StaffMainWindow::showRepairProcess);
    connect(announcement,&QMenu::aboutToShow,this,&StaffMainWindow::showAnnouncement);
}

QTableWidgetItem* StaffMainWindow::getSingleSelectedItem()
{
    QList<QTableWidgetItem*> selectedItems = table->selectedItems();
    if(selectedItems.size() == 1) {
        return selectedItems.first();
    }
    return nullptr;
}

QTableWidgetItem* StaffMainWindow::getSingleSelectedItemEdit()
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

void StaffMainWindow::onItemSelectionChanged()
{
    bool hasSingle = (getSingleSelectedItem() != nullptr);
    emit singleItemSelectedChanged(hasSingle);
}

void StaffMainWindow::onItemSelectionChangedEdit()
{
    bool hasSingle = (getSingleSelectedItemEdit() != nullptr);
    emit singleItemSelectedChangedEdit(hasSingle);
}

void StaffMainWindow::addAnnouncement()
{
    stackedWidget->setCurrentWidget(announce);
}

void StaffMainWindow::editAnnouncement()
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

void StaffMainWindow::deleteAnnouncement()
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

void StaffMainWindow::showMonthlyAttendance() {
    stackedWidget->setCurrentWidget(monthlyAttendance);
}

void StaffMainWindow::showLeave() {
    stackedWidget->setCurrentWidget(leave);
}

void StaffMainWindow::showAttendanceRecord() {
    stackedWidget->setCurrentWidget(attendanceRecord);
}

void StaffMainWindow::showOwnerRecord() {
    stackedWidget->setCurrentWidget(ownerRecord);
}

void StaffMainWindow::showOwnerInfoEdit() {
    stackedWidget->setCurrentWidget(ownerInfoEdit);
}

void StaffMainWindow::showOwnerInfoQuery() {
    stackedWidget->setCurrentWidget(ownerInfoQuery);
}

void StaffMainWindow::showHouseRegistration() {
    stackedWidget->setCurrentWidget(houseRegistration);
}

void StaffMainWindow::showCarRecord() {
    stackedWidget->setCurrentWidget(carRecord);
}

void StaffMainWindow::showParkingManage() {
    stackedWidget->setCurrentWidget(parkingManage);
}

void StaffMainWindow::showParkingRent() {
    stackedWidget->setCurrentWidget(parkingRent);
}

void StaffMainWindow::showPriceMaintenance() {
    stackedWidget->setCurrentWidget(priceMaintenance);
}

void StaffMainWindow::showOfflinePay() {
    stackedWidget->setCurrentWidget(offlinePay);
}

void StaffMainWindow::showPaymentQuery() {
    stackedWidget->setCurrentWidget(paymentQuery);
}

void StaffMainWindow::showWarrantyQuery() {
    stackedWidget->setCurrentWidget(warrantyQuery);
}

void StaffMainWindow::showRepairProcess() {
    stackedWidget->setCurrentWidget(repairProcess);
}

void StaffMainWindow::showAnnouncement() {
    stackedWidget->setCurrentWidget(ui->centralwidget);
}

StaffMainWindow::~StaffMainWindow()
{
    delete ui;
}
