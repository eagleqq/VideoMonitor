#include "loginsystemdialog.h"
#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>
#include<iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void createDir(QString path){
    // 判断文件夹是否存在，不存在则创建
    QDir dir(path);
    if(!dir.exists()){
        bool ismkdir = dir.mkdir(path);
        if(!ismkdir)
            qDebug() << "Create path fail" << endl;
        else
            qDebug() << "Create fullpath success" << endl;
    }
    else{
        qDebug() << "fullpath exist" << endl;
    }
}


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow mainWin;
    QString rootPath = QCoreApplication::applicationDirPath();
    qDebug() << rootPath;
    createDir(rootPath + "/videos");
    createDir(rootPath + "/images");
    //登录
    LoginSystemDialog w;
    w.exec();
    if(w.getIsLoginSuccess()){ //登录成功
       mainWin.show();
    }
    return a.exec();
}
