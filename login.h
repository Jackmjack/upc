#ifndef LOGIN_H
#define LOGIN_H
#include "database.h"
#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

signals:
    void loginSuccess(int userId,int role,QString name);

private slots:
    void on_confirm_button_clicked();

private:
    Ui::Login *ui;
    Database db;
};

#endif // LOGIN_H
