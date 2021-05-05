#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "v4l2api.h"

#include <QMainWindow>
#include <QNetworkInterface>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  /**
   * @brief getLocalIp 获取本地ip
   * @return
   */
  QString getLocalIp();

private slots:
  /**
   * @brief slotTcpNewConnection 新客户端接入
   */
  void slotTcpNewConnection();
  /**
   * @brief slotTcpReadyRead 接收到数据
   */
  void slotTcpReadyRead();
  /**
   * @brief slotSendImage 定时发送图片
   */
  void slotSendImage(QImage image);
  /**
   * @brief slotDisconnect  断开连接
   */
  void slotDisconnect();

private:
  Ui::MainWindow *ui;
  QTcpServer *tcpserver;
  QTcpSocket *tcpsocket;
  QTimer *sendtimer; //发送图片定时器
  V4l2Api vapi;
};

#endif // MAINWINDOW_H
