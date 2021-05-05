#ifndef V4L2API_H
#define V4L2API_H
#include <QImage>
#include <QObject>
#include <QThread>
#include <exception>
#include <fcntl.h>
#include <iostream>
#include <linux/videodev2.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
using namespace std;

const int WIDTH = 640;
const int HEIGHT = 480;

//异常类
class VideoException : public exception {
public:
  VideoException(string err) : errStr(err) {}
  ~VideoException() {}
  const char *what() const noexcept { return errStr.c_str(); }

private:
  string errStr;
};

struct VideoFrame {
  char *start; //保存内核空间映射到用户空间的空间首地址
  int length;  //空间长度
};

// v4l2封装成线程
class V4l2Api : public QThread {
  Q_OBJECT
public:
  V4l2Api(const char *dname = "/dev/video0", int count = 4);
  ~V4l2Api();
  /**
   * @brief open 打开摄像头
   */
  void open();
  /**
   * @brief close 关闭摄像头
   */
  void close();
  /**
   * @brief grapImage
   * @param imageBuffer
   * @param length
   */
  void grapImage(char *imageBuffer, int *length);
  /**
   * @brief yuyv_to_rgb888 yuyv转rgb算法
   * @param yuyvdata
   * @param rgbdata
   * @param picw
   * @param pich
   * @return
   */
  bool yuyv_to_rgb888(unsigned char *yuyvdata, unsigned char *rgbdata,
                      int picw = WIDTH, int pich = HEIGHT);
  /**
   * @brief jpeg_to_rgb888   jpeg转rgb算法
   * @param jpegData
   * @param size
   * @param rgbdata
   */
  void jpeg_to_rgb888(unsigned char *jpegData, int size,
                      unsigned char *rgbdata);
  inline void setRunning(bool running);
  /**
   * @brief run定义run函数
   */
  void run();

private:
  /**
   * @brief video_init
   */
  void video_init();
  /**
   * @brief video_mmap
   */
  void video_mmap();

private:
  string deviceName; //摄像头名称
  int vfd;           //保存文件描述符
  int count;         //缓冲区个数
  vector<struct VideoFrame> framebuffers;
  volatile bool isRunning; //是否运行

signals:
  /**
   * @brief sendImage  发送图片
   */
  void sendImage(QImage);
};
void V4l2Api::setRunning(bool running) { this->isRunning = running; }

#endif // V4L2API_H
