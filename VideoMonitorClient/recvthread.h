#ifndef RECVTHREAD_H
#define RECVTHREAD_H

#include <QImage>
#include <QThread>

class RecvThread : public QThread {
  Q_OBJECT
public:
  explicit RecvThread(QObject *parent = NULL);
  explicit RecvThread(QString ip, int port, QObject *parent = NULL);
  ~RecvThread();
  inline void setRunning(bool running);
  void setIsEdgeDetect(bool isEdgeDetect);
  void setIsSharpen(bool isSharpen);
  void setIsThreshold(bool isThreshold);
  void setIsShowTime(bool isShowTime);

signals:
  void transmitData(QImage image);
  void disconnectSlot();

protected:
  void run() Q_DECL_OVERRIDE;

private:
  QString ip;
  int port;
  volatile bool isRunning;
  bool isEdgeDetect;  //是否边缘检测
  bool isSharpen; //是否锐化
  bool isThreshold; //是否阈值分割
  bool isShowTime; //是否显示时间
};

void RecvThread::setRunning(bool running) { this->isRunning = running; }

#endif // RECVTHREAD_H
