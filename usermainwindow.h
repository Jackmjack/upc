#ifndef USERMAINWINDOW_H
#define USERMAINWINDOW_H

#include "database.h"
#include "parkingapplywidget.h"
#include "myparkingspotwidget.h"
#include "caridentificationwidget.h"
#include "userpaymentwidget.h"
#include "selfpaymentwidget.h"
#include "faultreportwidget.h"
#include "repaircommentwidget.h"
#include "repairquerywidget.h"
#include <QMainWindow>
#include <QStackedWidget>
#include <QTableWidget>
#include <QLabel>
#include <QTimer>

namespace Ui {
class UserMainWindow;
}

class UserMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserMainWindow(int userId,QString name,QWidget *parent = nullptr);
    ~UserMainWindow();

private slots:
    void showParkingApply();
    void showMyParkingspot();
    void showCarIdentification();
    void showUserPayment();
    void showSelfPayment();
    void showFaultReport();
    void showRepairQuery();
    void showRepairComment();
    void showAnnouncement();

private:
    Ui::UserMainWindow *ui;
    int userId;
    QString name;
    void setupMenus();
    QTableWidget* table;
    void refreshTable();
    Database db;
    QStackedWidget* stackedWidget;
    ParkingApplyWidget* parkingApply;
    MyParkingspotWidget* myParkingspot;
    CarIdentificationWidget* carIdentification;
    UserPaymentWidget* userPayment;
    SelfPaymentWidget* selfPayment;
    FaultReportWidget* faultReport;
    RepairQueryWidget* repairQuery;
    RepairCommentWidget* repairComment;
};

#endif // USERMAINWINDOW_H
