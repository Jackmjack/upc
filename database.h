#ifndef DATABASE_H
#define DATABASE_H
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>

class Database
{
public:
    Database();
    ~Database();

    bool openDatabase(const QString& dbPath);
    QSqlQuery getUserByUsername(const QString& username);
    QSqlQuery executeSQL(const QString &sql,const QVector<QVariant> &params);

private:
    QSqlDatabase m_db;
};

#endif // DATABASE_H
