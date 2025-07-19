#include "login.h"
#include "ui_login.h"
#include <QString>
#include <QMessageBox>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    //连接数据库
    if(!db.openDatabase("smartcommunity.db")) {
        QMessageBox::critical(this,"错误","数据库打开失败");
    }

    //隐藏问号按钮
    this->setWindowFlags(this->windowFlags()&~Qt::WindowContextHelpButtonHint);

    //密码隐藏
    ui->le_password->setEchoMode(QLineEdit::Password);

    //设置样式表
    setStyleSheet(R"(
                  QWidget {
                      background-color: #f0f5ff;
                      font-family: "Microsoft YaHei", sans-serif;
                  }

                  QPushButton#confirm_button {
                      background-color: #165DFF;
                      color: white;
                      border: none;
                      border-radius: 4px;
                      padding: 4px 20px;
                      font-size: 18px;
                  }
                  QPushButton#confirm_button:hover {
                      background-color: #0E4CD5;
                  }
                  QPushButton#confirm_button:pressed {
                      background-color: #0A3DA8;
                  }

                  QPushButton#exit_button {
                      background-color: #165DFF;
                      color: white;
                      border: none;
                      border-radius: 4px;
                      padding: 4px 20px;
                      font-size: 18px;
                  }
                  QPushButton#exit_button:hover {
                      background-color: #0E4CD5;
                  }
                  QPushButton#exit_button:pressed {
                      background-color: #0A3DA8;
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
                  )");
}

Login::~Login()
{
    delete ui;
}

void Login::on_confirm_button_clicked()
{
    //获取输入框内容
    QString username = ui->le_username->text().trimmed();
    QString password = ui->le_password->text();

    //判断内容是否为空
    if(username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this,"提示","请输入用户名和密码！");
        return;
    }

    //从数据库中查询数据
    QSqlQuery query = db.getUserByUsername(username);
    if(query.next()) {
        QString dbPassword = query.value("password").toString();
        if(dbPassword != password) {
            QMessageBox::warning(this,"登录失败","密码错误！");
            return;
        } else {
            int userId = query.value("id").toInt();
            int role = query.value("role").toInt();
            QString name = query.value("name").toString();
            emit loginSuccess(userId,role,name);
            accept();
        }
    } else {
        QMessageBox::warning(this,"登录失败","未找到用户名，请先注册！");
        return;
    }
}
