#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QBuffer>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
//  setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
//                                  this->size(),
//                                  qApp->desktop()->availableGeometry()));

  tcpserver = new QTcpServer(this);
  //主机连接从机是采用监听的方式，一般监听的对象设成Any，你也可以根据自己网络情况设成AnyIPv4或者AnyIPv6，
  //但是端口号一定要和我们从机写的端口号一致才能通信成功
  int port = 48797;
  tcpserver->listen(QHostAddress::Any, 48797);
  connect(tcpserver, SIGNAL(newConnection()), this,
          SLOT(slotTcpNewConnection()));

  QString ipAddress = getLocalIp();
  ui->label_ip->setText(
      tr("服务器地址： %1 (%2)").arg(ipAddress).arg(QString::number(port)));
  ui->textBrowser->append("请连接服务器，当前只支持1对1连接，如果需要连接多个服"
                          "务器，请在不同的电脑上分别运行服务器，保证服务器IP不"
                          "一样");

  connect(&vapi, &V4l2Api::sendImage, this, &MainWindow::slotSendImage);
}

MainWindow::~MainWindow() {
  if (tcpserver != NULL) {
    tcpserver->deleteLater();
    tcpserver = NULL;
  }
  delete ui;
}

QString MainWindow::getLocalIp() {
  QString ipAddress;
  // 获取本机IP
  QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
  // use the first non-localhost IPv4 address
  for (int i = 0; i < ipAddressesList.size(); ++i) {
    if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
        ipAddressesList.at(i).toIPv4Address()) {
      ipAddress = ipAddressesList.at(i).toString();
      break;
    }
  }
  // if we did not find one, use IPv4 localhost
  if (ipAddress.isEmpty())
    ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
  qDebug() << "IP为：" << ipAddress;
  return ipAddress;
}

void MainWindow::slotTcpNewConnection() {
  qDebug("NEW CONNECTION");
  tcpsocket = tcpserver->nextPendingConnection(); //获取监听到的socket
  /*获取对方IP和端口*/
  QString ip = tcpsocket->peerAddress().toString();
  quint16 port = tcpsocket->peerPort();
  QString str = QString("[%1:%2]成功连接").arg(ip).arg(port);
  ui->textBrowser->append(str); /*显示编辑区*/
  //连接成功后我们再连接一个信号槽到准备接收信号槽函数中去
  connect(tcpsocket, SIGNAL(readyRead()), this, SLOT(slotTcpReadyRead()));
  //已连接套接字的断开信号与自身的稍后删除信号相连接
  connect(tcpsocket, SIGNAL(disconnected()), this, SLOT(slotDisconnect()));
}

// qCompress压缩图片
void MainWindow::slotTcpReadyRead() {
  QByteArray recvbuf = tcpsocket->readAll();
  qDebug("recv:");
  qDebug(recvbuf);
  vapi.start();
}

void MainWindow::slotSendImage(QImage image) {
  qDebug("send image:");
  QImage tempimage = image;

  // 1920x1080  缩放图片
  tempimage =
      tempimage.scaled(800, 480, Qt::KeepAspectRatio, Qt::FastTransformation)
          .scaled(400, 240, Qt::KeepAspectRatio, Qt::SmoothTransformation);

  QPixmap pixmap =
      QPixmap::fromImage(tempimage); //把img转成位图，我们要转成jpg格式
  QByteArray ba;
  QBuffer buf(&ba); //把ba绑定到buf上，操作buf就等于操作ba
  pixmap.save(&buf, "jpg", 50); //把pixmap保存成jpg，压缩质量50 数据保存到buf
  //先写大小过去，告诉主机我们要传输的数据有多大
  tcpsocket->write(QString("size=%1").arg(ba.size()).toLocal8Bit().data());
  tcpsocket->waitForReadyRead(); //等待主机响应“ok”
  tcpsocket->write(ba);          //把图像数据写入传输给主机
  tcpsocket->waitForReadyRead();
}

void MainWindow::slotDisconnect() {
  qDebug("disconnected");
  vapi.setRunning(false);
  /*获取对方IP和端口*/
  QString ip = tcpsocket->peerAddress().toString();
  quint16 port = tcpsocket->peerPort();
  QString str = QString("[%1:%2]断开连接").arg(ip).arg(port);
  ui->textBrowser->append(str); /*显示编辑区*/
  tcpsocket->deleteLater();
}
