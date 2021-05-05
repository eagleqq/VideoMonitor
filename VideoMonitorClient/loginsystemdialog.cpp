#include "loginsystemdialog.h"
#include "mainwindow.h"
#include "ui_loginsystemdialog.h"
#include <QMessageBox>
#include "sqlitesingleton.h"

LoginSystemDialog::LoginSystemDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::LoginSystemDialog), isLoginSuccess(false) {
  ui->setupUi(this);
  this->setWindowIcon(QIcon(":/images/icon.png"));
  ui->stackedWidget->setCurrentWidget(ui->loginPage);
  SQLiteSingleton::getInstance().initDB();
  SQLiteSingleton::getInstance().createTable();
}

LoginSystemDialog::~LoginSystemDialog() { delete ui; }

void LoginSystemDialog::on_backButton_clicked() {
  //跳转登录页面
  ui->stackedWidget->setCurrentWidget(ui->loginPage);
}

void LoginSystemDialog::on_completeRegButton_clicked() {
    QString username = ui->uBox->text();
    QString pwd1 = ui->pBox->text();
    QString pwd2 = ui->pBox2->text();
    if(username.isEmpty() || pwd1.isEmpty() || pwd2.isEmpty()){
        QMessageBox::information(this, "提示","输入内容不能为空！");
        return;
    }
    if(pwd1 != pwd2){
        QMessageBox::information(this, "提示","两次密码输入不一致！");
        return;
    }
    //插入数据库
    SQLiteSingleton::getInstance().insertUserTable(username, pwd1);
    QMessageBox::information(this, "提示","注册成功！");
    //跳转注册页面
    ui->stackedWidget->setCurrentWidget(ui->loginPage);
}

void LoginSystemDialog::on_loginButton_clicked() {
    QString username = ui->usernameLineEdit->text();
    QString pwd = ui->passwordLineEdit->text();
    if(username.isEmpty() || pwd.isEmpty()){
        QMessageBox::information(this, "提示","输入内容不能为空,请输入正确用户名和密码！");
        return;
    }
    isLoginSuccess = SQLiteSingleton::getInstance().queryUserExist(username, pwd);
    if(isLoginSuccess){
        this->close();
    }else{
        QMessageBox::information(this, "提示","用户名或密码输入错误！");
    }

}

void LoginSystemDialog::on_regButton_clicked() {
  //跳转注册页面
    ui->stackedWidget->setCurrentWidget(ui->registerPage);
}

bool LoginSystemDialog::getIsLoginSuccess()
{
    return isLoginSuccess;
}
