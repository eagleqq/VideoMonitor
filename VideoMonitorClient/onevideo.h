#ifndef ONEVIDEO_H
#define ONEVIDEO_H

#include "iconbutton.h"
#include "recvthread.h"

#include <QCheckBox>
#include <QFrame>
#include <QImage>

class IconButton;
class RecvThread;
class QLabel;

class OneVideo : public QFrame {
  Q_OBJECT
public:
  explicit OneVideo(QWidget *parent = 0);
  ~OneVideo();
  /**
   * @brief createToolButtons  工具按钮
   */
  void createToolButtons();
  /**
   * @brief createIpPortLabel  地址端口显示
   */
  void createIpPortLabel();
  /**
   * @brief createCheckBox  图片处理算法
   */
  void createCheckBox();

  static const int WIDTH;   //显示宽度
  static const int HEIGHT;  //显示高度

private slots:
  /**
   * @brief playBtnSlot
   */
  void playBtnSlot();
  /**
   * @brief cameraBtnSlot
   */
  void cameraBtnSlot();
  /**
   * @brief recordBtnSlot
   */
  void recordBtnSlot();
  /**
   * @brief fullScreenBtnSlot
   */
  void fullScreenBtnSlot();
  /**
   * @brief updateImage
   * @param image
   */
  void updateImage(QImage image);
  /**
   * @brief disconnectSlot
   */
  void disconnectSlot();
  /**
   * @brief edgeDetectSlot 是否边缘检测
   * @param isChecked
   */
  void edgeDetectSlot(bool isChecked);
  /**
   * @brief sharpenSlot 是否锐化
   * @param isChecked
   */
  void sharpenSlot(bool isChecked);
  /**
   * @brief thresholdSlot 是否阈值分割
   * @param isChecked
   */
  void thresholdSlot(bool isChecked);

signals:
  void closeSignal(OneVideo *who);

protected:
  void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
  void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
  void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
  void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private:
  IconButton *closeBtn;          //关闭
  IconButton *playBtn;           //开始、暂停
  IconButton *cameraBtn;         //拍照
  IconButton *recordBtn;         //录屏
  IconButton *fullScreenBtn;     //全屏
  IconButton *pauseBtn;
  QLabel *ipPortLabel;           // ip 端口号显示
  QCheckBox *edgeDetectCheckBox; //边缘检测
  QCheckBox *sharpenCheckBox;    //锐化
  QCheckBox *thresholdCheckBox;  //阈值分割

  //显示的图像
  QImage centralImage;
  //按钮工具栏显示状态
  bool isButtonsShow;
  //是否正在运行
  bool isPlay;
  bool isPause;
  //网络线程
  RecvThread *networkThread;
};
#endif // ONEVIDEO_H
