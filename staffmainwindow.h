#ifndef STAFFMAINWINDOW_H
#define STAFFMAINWINDOW_H

#include "monthlyattendancewidget.h"
#include "leavewidget.h"
#include "attendancerecordwidget.h"
#include "ownerrecordwidget.h"
#include "ownerinfoeditwidget.h"
#include "ownerinfoquerywidget.h"
#include "houseregistrationwidget.h"
#include "carrecordwidget.h"
#include "parkingmanagewidget.h"
#include "parkingrentwidget.h"
#include "pricemaintenancewidget.h"
#include "offlinepaywidget.h"
#include "paymentquerywidget.h"
#include "warrantyquerywidget.h"
#include "repairprocesswidget.h"
#include "addannouncementwidget.h"
#include <QMainWindow>
#include <QStackedWidget>
#include <QLabel>
#include <QTimer>

namespace Ui {
class StaffMainWindow;
}

class StaffMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StaffMainWindow(int userId,QString name,QWidget *parent = nullptr);
    ~StaffMainWindow();

signals:
    void singleItemSelectedChangedEdit(bool hasSingleItem);
    void singleItemSelectedChanged(bool hasSingleItem);

private slots:
    void addAnnouncement();
    void editAnnouncement();
    void deleteAnnouncement();
    void onItemSelectionChanged();
    void onItemSelectionChangedEdit();
    void showMonthlyAttendance();
    void showLeave();
    void showAttendanceRecord();
    void showOwnerRecord();
    void showOwnerInfoEdit();
    void showOwnerInfoQuery();
    void showHouseRegistration();
    void showCarRecord();
    void showParkingManage();
    void showParkingRent();
    void showPriceMaintenance();
    void showOfflinePay();
    void showPaymentQuery();
    void showWarrantyQuery();
    void showRepairProcess();
    void showAnnouncement();

private:
    Ui::StaffMainWindow *ui;
    int userId;
    QString name;
    QTableWidget* table;
    void refreshTable();
    Database db;
    void setupMenus();
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *refreshButton;
    QTableWidgetItem* getSingleSelectedItem();
    QTableWidgetItem* getSingleSelectedItemEdit();
    QStackedWidget* stackedWidget;
    MonthlyAttendanceWidget* monthlyAttendance;
    LeaveWidget* leave;
    AttendanceRecordWidget* attendanceRecord;
    OwnerRecordWidget* ownerRecord;
    OwnerInfoEditWidget* ownerInfoEdit;
    OwnerInfoQueryWidget* ownerInfoQuery;
    HouseRegistrationWidget* houseRegistration;
    CarRecordWidget* carRecord;
    ParkingManageWidget* parkingManage;
    ParkingRentWidget* parkingRent;
    PriceMaintenanceWidget* priceMaintenance;
    OfflinePayWidget* offlinePay;
    PaymentQueryWidget* paymentQuery;
    WarrantyQueryWidget* warrantyQuery;
    RepairProcessWidget* repairProcess;
    AddAnnouncementWidget* announce;
};

#endif // STAFFMAINWINDOW_H
