#ifndef SQLITESINGLETON_H
#define SQLITESINGLETON_H

#include <QDebug>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QSharedPointer>
#include <QSqlDatabase>

class SQLiteSingleton {

public:
  static SQLiteSingleton &getInstance();
  /**
   * @brief initDB  初始化数据库
   * @param db_type
   */
  void initDB(QString db_type = "QSQLITE");
  /**
   * @brief createTable 创建数据库表
   */
  void createTable();
  /**
   * @brief insertUserTable  添加用户
   * @param username 用户名
   * @param pwd  密码
   */
  bool insertUserTable(QString username, QString pwd);
  /**
   * @brief queryUserExist 查询用户是否存在
   * @param username
   * @param pwd
   * @return
   */
  bool queryUserExist(QString username, QString pwd);

private:
  SQLiteSingleton();
  ~SQLiteSingleton();
  QSqlDatabase db;
};

#endif // SQLITESINGLETON_H
