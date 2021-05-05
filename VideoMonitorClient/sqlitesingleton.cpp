#include "sqlitesingleton.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

SQLiteSingleton &SQLiteSingleton::getInstance() {
  static SQLiteSingleton tSQLiteSingleton;
  return tSQLiteSingleton;
}

void SQLiteSingleton::initDB(QString db_type) {
  qDebug() << "初始化数据库";
  if (db_type == "QSQLITE") {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("UserInfo.dat");
    if (!db.open()) {
      QSqlError lastError = db.lastError();
      QMessageBox::warning(0, QObject::tr("Database Error"),
                           "数据库打开失败," + lastError.driverText());
      return;
    }
  }
}

void SQLiteSingleton::createTable() {
  QSqlQuery query(db);
  bool ret = query.exec("create table user_info (username varchar(40) primary "
                        "key, password varchar(40))");
  qDebug() << "create user_info " << ret;
}

bool SQLiteSingleton::insertUserTable(QString username, QString pwd) {
  QSqlQuery query(db);
  bool ret = query.exec(
      QObject::tr("insert into user_info values('%1', '%2')").arg(username).arg(pwd));
  qDebug() << "insertUserTable" << ret;
  return ret;
}

bool SQLiteSingleton::queryUserExist(QString username, QString pwd) {
  bool isExist = false;
  QSqlQuery query(db);
  bool ret = query.exec(
      QObject::tr("select * from user_info where username='%1' and password='%2'")
          .arg(username)
          .arg(pwd));
  qDebug() << "queryUserExist" << ret;
  while (query.next()) {
    isExist = true;
  }
  return isExist;
}

SQLiteSingleton::SQLiteSingleton() {}

SQLiteSingleton::~SQLiteSingleton() {}
