#include "recvthread.h"
#include <QByteArray>
#include <QDateTime>
#include <QImage>
#include <QPainter>
#include <QTcpSocket>
#include "util.h"
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

RecvThread::RecvThread(QObject *parent) : QThread(parent) {}

RecvThread::RecvThread(QString ip, int port, QObject *parent)
    : QThread(parent) {
  this->ip = ip;
  this->port = port;
  setIsShowTime(true);
}

RecvThread::~RecvThread() {}

void RecvThread::setIsEdgeDetect(bool isEdgeDetect)
{
    this->isEdgeDetect = isEdgeDetect;
}

void RecvThread::setIsSharpen(bool isSharpen)
{
    this->isSharpen = isSharpen;
}

void RecvThread::setIsThreshold(bool isThreshold)
{
    this->isThreshold = isThreshold;
}

void RecvThread::setIsShowTime(bool isShowTime)
{
    this->isShowTime = isShowTime;
}

void RecvThread::run() {
  isRunning = true;
  QTcpSocket tcpsocket;
  connect(&tcpsocket, SIGNAL(disconnected()), this, SIGNAL(disconnectSlot()));
  tcpsocket.connectToHost(ip, port);
  //	socket.connectToHost("192.168.47.131", 10086);
  if (!tcpsocket.waitForConnected(3000)) {
    qDebug() << "连接失败：" << tcpsocket.errorString();
    emit disconnectSlot();
    return;
  } else {
    qDebug() << "连接成功！";
  }

  //发送HTTP请求
  tcpsocket.write("\r\n\r\n");
  if (tcpsocket.waitForBytesWritten(3000)) {
    qDebug() << "发送Http Request成功！";
  } else {
    qDebug() << "发送Http Request失败！";
    return;
  }
  cv::VideoWriter videowriter;  //录屏
  int recvtimes=0;  //接收次数
  QByteArray jpgArr; //存储图片
  int totalsize;     //总大小
  while (isRunning) {
    if (tcpsocket.waitForReadyRead()) {
      QByteArray buf = tcpsocket.readAll();
      qDebug() << "recv buf: " << buf << "\n";
      if (buf.contains("size=")) {
        buf = buf.replace("size=", "");
        totalsize = buf.toInt();
        jpgArr.clear();
        tcpsocket.write("ok");           //发送一个响应给客户机
        tcpsocket.waitForBytesWritten(); //等待数据写入
      } else {
        //如果不是图片大小数据，就是图片数据，图片数据追加写到数组里面
        jpgArr.append(buf);
      }
      if (jpgArr.length() == totalsize) {
        QImage img_qt;
        img_qt.loadFromData(jpgArr, "JPG");
        cv::Mat src_img_cv;   //源数据
        src_img_cv = Util::QImage2cvMat(img_qt);
        recvtimes ++;
        if(isShowTime){  //显示时间
            //设置绘制文本的相关参数
            QDateTime current_time = QDateTime::currentDateTime();
            //显示时间，格式为：年-月-日 时：分：秒
            QString StrCurrentTime = current_time.toString("yyyy-MM-dd hh:mm:ss");
            std::string text_time = StrCurrentTime.toStdString();
            int font_face = cv::FONT_HERSHEY_COMPLEX;
            double font_scale = 0.5;
            int thickness = 1;
            int baseline;
            //获取文本框的长宽
            cv::Size text_size = cv::getTextSize(text_time, font_face, font_scale, thickness, &baseline);
            //将文本框居中绘制
            cv::Point origin;
            origin.x = src_img_cv.cols / 2 - text_size.width / 2;
            origin.y = text_size.height * 2;
            cv::putText(src_img_cv, text_time, origin, font_face, font_scale, cv::Scalar(0, 255, 255), thickness, 4, 0);
            if(recvtimes==1){
//                cv::cvtColor(src_img_cv, src_img_cv, cv::COLOR_RGB2BGR);
                videowriter.open("./videos/" + text_time + ".avi",CV_FOURCC('M', 'J', 'P', 'G'),40,cv::Size(src_img_cv.cols,src_img_cv.rows));
                qDebug("video open");
            }
            videowriter.write(src_img_cv);
        }
        if(isEdgeDetect){  //边缘检测
            cv::Mat dstPic, edge, grayImage;
            //创建与src同类型和同大小的矩阵
            dstPic.create(src_img_cv.size(), src_img_cv.type());
            //将原始图转化为灰度图
            cvtColor(src_img_cv, grayImage, COLOR_BGR2GRAY);
            //先使用3*3内核来降噪
            blur(grayImage, edge, Size(3, 3));
            //运行canny算子
            Canny(edge, edge, 3, 9, 3);
            src_img_cv = edge;
        }
        if(isSharpen){ //锐化
            cv::Mat blur_img, usm;
            GaussianBlur(src_img_cv, blur_img, Size(0, 0), 25);//高斯滤波
            addWeighted(src_img_cv, 1.5, blur_img, -0.5, 0, usm);
            src_img_cv = usm;
        }
        if(isThreshold){ //阈值分割
            double the = 150;//阈值
            cv::Mat threshold_dst;
            threshold(src_img_cv, threshold_dst, the, 255, THRESH_BINARY);//手动设置阈值
            src_img_cv = threshold_dst;
        }
        img_qt = Util::cvMat2QImage(src_img_cv);
        emit transmitData(img_qt);
        jpgArr.clear();
        tcpsocket.write("ok");
      }
    }
  }
  videowriter.release();
  qDebug("video release");
  /*
//仿真
while (isRunning) {
 QImage image("/home/shenjun/Pictures/Link0STL.png");
 emit transmitData(image);
 sleep(1);
}*/
}
