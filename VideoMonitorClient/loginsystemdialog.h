#ifndef LOGINSYSTEMDIALOG_H
#define LOGINSYSTEMDIALOG_H

#include "mainwindow.h"

#include <QDialog>

namespace Ui {
class LoginSystemDialog;
}

class LoginSystemDialog : public QDialog {
  Q_OBJECT

public:
  explicit LoginSystemDialog(QWidget *parent = 0);
  ~LoginSystemDialog();

  /**
   * @brief getIsLoginSuccess 获取是否登录成功
   * @return
   */
  bool getIsLoginSuccess();

private slots:
  /**
   * @brief on_backButton_clicked  返回登录界面
   */
  void on_backButton_clicked();
  /**
   * @brief on_completeRegButton_clicked  完成注册
   */
  void on_completeRegButton_clicked();
  /**
   * @brief on_loginButton_clicked  登录
   */
  void on_loginButton_clicked();
  /**
   * @brief on_regButton_clicked 进入注册页面
   */
  void on_regButton_clicked();

private:
  Ui::LoginSystemDialog *ui;
  bool isLoginSuccess; //记录是否登录成功
};

#endif // LOGINSYSTEMDIALOG_H
