#ifndef MANAGERMAINWINDOW_H
#define MANAGERMAINWINDOW_H

#include "staffmanagewidget.h"
#include "addstaffwidget.h"
#include "leaveapprovewidget.h"
#include "attendancewidget.h"
#include "sysinitializewidget.h"
#include "addannouncementwidget.h"
#include <QMainWindow>
#include <QStackedWidget>
#include <QLabel>
#include <QTimer>

namespace Ui {
class ManagerMainWindow;
}

class ManagerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManagerMainWindow(int userId,QString name,QWidget *parent = nullptr);
    ~ManagerMainWindow();

signals:
    void singleItemSelectedChangedEdit(bool hasSingleItem);
    void singleItemSelectedChanged(bool hasSingleItem);

private slots:
    void addAnnouncement();
    void editAnnouncement();
    void deleteAnnouncement();
    void onItemSelectionChanged();
    void onItemSelectionChangedEdit();
    void showStaffManage();
    void showAddStaff();
    void showLeaveApprove();
    void showAttendance();
    void showSysInitialize();
    void showAnnouncement();

private:
    Ui::ManagerMainWindow *ui;
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
    StaffManageWidget* staffManageWidget;
    AddStaffWidget* addStaffWidget;
    LeaveApproveWidget* leaveApproveWidget;
    AttendanceWidget* attendanceWidget;
    SysInitializeWidget* sysInitializeWidget;
    AddAnnouncementWidget* announce;
};

#endif // MANAGERMAINWINDOW_H
