#include "usermainwindow.h"
#include "ui_usermainwindow.h"
#include <QDateTime>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

UserMainWindow::UserMainWindow(int userId,QString name,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserMainWindow)
{
    ui->setupUi(this);
    this->userId = userId;
    this->name = name;

    //创建菜单栏
    setupMenus();

    //创建窗口树
    stackedWidget = new QStackedWidget(this);
    parkingApply = new ParkingApplyWidget(userId,this);
    myParkingspot = new MyParkingspotWidget(userId,this);
    carIdentification = new CarIdentificationWidget(userId,this);
    userPayment = new UserPaymentWidget(userId);
    selfPayment = new SelfPaymentWidget(userId,name,this);
    faultReport = new FaultReportWidget(userId,name,this);
    repairQuery = new RepairQueryWidget(userId,this);
    repairComment = new RepairCommentWidget(userId,this);

    //将窗口添加到窗口树
    stackedWidget->addWidget(parkingApply);
    stackedWidget->addWidget(myParkingspot);
    stackedWidget->addWidget(carIdentification);
    stackedWidget->addWidget(userPayment);
    stackedWidget->addWidget(selfPayment);
    stackedWidget->addWidget(faultReport);
    stackedWidget->addWidget(repairQuery);
    stackedWidget->addWidget(repairComment);
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
    QLabel *permanentLabel = new QLabel(QString("业主id：%1").arg(this->userId),this);
    statusBar->addPermanentWidget(permanentLabel);
    QLabel *permanentLabel2 = new QLabel(QString("当前操作人员：%1").arg(this->name),this);
    statusBar->addPermanentWidget(permanentLabel2);

    //创建标题标签
    QLabel *title = new QLabel("公告",this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");

    //创建表
    table = new QTableWidget(this);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"发布时间","标题","详情"});
    table->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    table->setColumnWidth(0,170);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);

    //创建布局
    QVBoxLayout *vLayout = new QVBoxLayout();

    //设置布局
    vLayout->addWidget(title);
    vLayout->addWidget(table);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setSpacing(0);
    ui->centralwidget->setLayout(vLayout);

    //连接数据库
    if(!db.openDatabase("smartcommunity.db")) {
        QMessageBox::critical(this,"错误","数据库打开失败");
    }

    //刷新表
    refreshTable();

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
                  })");
}

void UserMainWindow::setupMenus()
{
    QMenuBar *menubar = ui->menubar;
    menubar->clear();

    QMenu *parkingMenu = menubar->addMenu("车位管理");
    QMenu *paymentMenu = menubar->addMenu("缴费管理");
    QMenu *repairMenu = menubar->addMenu("维修管理");
    QMenu* announcement = menubar->addMenu("公告");
    QAction *actParkingApply = parkingMenu->addAction("车位申请");
    QAction *actMyParking = parkingMenu->addAction("我的车位");
    QAction *actCarRecognization = parkingMenu->addAction("车辆进入识别");
    QAction *actPaymentQuery = paymentMenu->addAction("缴费查询");
    QAction *actSelfPayment = paymentMenu->addAction("自助缴费");
    QAction *actFaultReport = repairMenu->addAction("故障报修");
    QAction *actRepairQuery = repairMenu->addAction("维修进度查询");
    QAction *actRepairComment = repairMenu->addAction("维修评价");

    connect(actParkingApply,&QAction::triggered,this,&UserMainWindow::showParkingApply);
    connect(actMyParking,&QAction::triggered,this,&UserMainWindow::showMyParkingspot);
    connect(actCarRecognization,&QAction::triggered,this,&UserMainWindow::showCarIdentification);
    connect(actPaymentQuery,&QAction::triggered,this,&UserMainWindow::showUserPayment);
    connect(actSelfPayment,&QAction::triggered,this,&UserMainWindow::showSelfPayment);
    connect(actFaultReport,&QAction::triggered,this,&UserMainWindow::showFaultReport);
    connect(actRepairQuery,&QAction::triggered,this,&UserMainWindow::showRepairQuery);
    connect(actRepairComment,&QAction::triggered,this,&UserMainWindow::showRepairComment);
    connect(announcement,&QMenu::aboutToShow,this,&UserMainWindow::showAnnouncement);
}

void UserMainWindow::refreshTable()
{
    table->setRowCount(0);
    QSqlQuery query = db.executeSQL("SELECT create_time,title,text FROM announcements",{});
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

void UserMainWindow::showParkingApply() {
    stackedWidget->setCurrentWidget(parkingApply);
}

void UserMainWindow::showMyParkingspot() {
    stackedWidget->setCurrentWidget(myParkingspot);
}

void UserMainWindow::showCarIdentification() {
    stackedWidget->setCurrentWidget(carIdentification);
}

void UserMainWindow::showUserPayment() {
    stackedWidget->setCurrentWidget(userPayment);
}

void UserMainWindow::showSelfPayment() {
    stackedWidget->setCurrentWidget(selfPayment);
}

void UserMainWindow::showFaultReport() {
    stackedWidget->setCurrentWidget(faultReport);
}

void UserMainWindow::showRepairQuery() {
    stackedWidget->setCurrentWidget(repairQuery);
}

void UserMainWindow::showRepairComment() {
    stackedWidget->setCurrentWidget(repairComment);
}

void UserMainWindow::showAnnouncement() {
    stackedWidget->setCurrentWidget(ui->centralwidget);
}

UserMainWindow::~UserMainWindow()
{
    delete ui;
}
