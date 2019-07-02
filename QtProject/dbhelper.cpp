#include "dbhelper.h"
#include <QDebug>
#include <QString>

DBHelper *DBHelper::instance = NULL;

DBHelper::DBHelper()
{
    db = QSqlDatabase::addDatabase("QODBC");
    QString dsn = QString::fromLocal8Bit("QtDSN");
    db.setHostName("127.0.0.1");
    db.setDatabaseName(dsn);
    db.setUserName("user");
    db.setPassword("4751xjxxjx");
}
void DBHelper::connectDatabase()
{
    bool ret = db.open();
//    if(ret){qDebug()<<"open db success";}
//    else{qDebug()<<"open db failed";}
}

void DBHelper::disconnectDatabase()
{
    db.close();
//    db.removeDatabase("dbmos");
}


//开门
DBHelper *DBHelper::getInstance()
{
    if(instance == NULL)
    {
        qDebug() << "new Instance !!!!!!!!!!!!";
        instance = new DBHelper();
    }
    return instance;
}
