#include "login.h"
#include <QApplication>
#include "managermainwindow.h"
#include "staffmainwindow.h"
#include "usermainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int userId = -1;
    int role = -1;
    QString username;
    Login l;
    ManagerMainWindow *mw;
    StaffMainWindow *sw;
    UserMainWindow *uw;

    QObject::connect(&l,&Login::loginSuccess,[&](int id,int r,QString name){
        userId = id;
        role = r;
        username = name;
        if(role == 0) {
            mw = new ManagerMainWindow(userId,username);
            mw->show();
        } else if (role == 1) {
            sw = new StaffMainWindow(userId,username);
            sw->show();
        } else if (role == 2) {
            uw = new UserMainWindow(userId,username);
            uw->show();
        }
    });
    l.show();
    return a.exec();
}
