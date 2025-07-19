#include "database.h"
#include <QString>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QSqlRecord>
#include <QMessageBox>

Database::Database()
{

}

Database::~Database()
{

}

//打开数据库
bool Database::openDatabase(const QString &dbPath)
{
    if (m_db.isOpen()) return true;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);
    if(!m_db.open()) {
        qWarning() << "Failed to open database:" << m_db.lastError().text();
        return false;
    }
    return true;
}

//通过用户名查询数据库
QSqlQuery Database::getUserByUsername(const QString &username)
{
    QSqlQuery query;
    query.prepare("SELECT id,password,role,name FROM users WHERE username = ?");
    query.addBindValue(username);
    query.exec();
    return query;
}

//执行传入的SQL语句
QSqlQuery Database::executeSQL(const QString &sql, const QVector<QVariant> &params)
{
    QSqlQuery query(m_db);
    query.prepare(sql);
    for (int i=0;i<params.size();i++) {
        query.addBindValue(params[i]);
    }
    if(!query.exec())
        QMessageBox::critical(nullptr,"错误","SQL执行失败！");
    return query;
}
