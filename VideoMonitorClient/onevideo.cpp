#include "onevideo.h"
#include "configdialog.h"
#include "iconbutton.h"
#include "recvthread.h"

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QLinearGradient>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>

const int OneVideo::WIDTH = 480;  // 480
const int OneVideo::HEIGHT = 320; // 320

OneVideo::OneVideo(QWidget *parent)
    : QFrame(parent), centralImage(":/images/icon.png") {
  resize(WIDTH, HEIGHT);
  createToolButtons();
  createIpPortLabel();
  createCheckBox();
  setFrameShape(QFrame::Box);
  setBackgroundRole(QPalette::Light);
  setAutoFillBackground(true);
  setMouseTracking(true);
  isPlay = false;
  isPause = false;
  cameraBtn->setEnabled(false);
//  recordBtn->setEnabled(false);
//  fullScreenBtn->setEnabled(false);
  pauseBtn->setEnabled(false);
  edgeDetectCheckBox->setEnabled(false);
  sharpenCheckBox->setEnabled(false);
  thresholdCheckBox->setEnabled(false);
  networkThread = NULL;
}

OneVideo::~OneVideo() {
  if (networkThread != NULL) {
    networkThread->setRunning(false);
    networkThread->wait();
    networkThread->deleteLater();
    networkThread = NULL;
  }
}

void OneVideo::createToolButtons() {
  closeBtn = new IconButton(this);
  closeBtn->setIcon(QIcon(":/images/close.png"));
  closeBtn->move(WIDTH - closeBtn->width() - 8, 8);
  connect(closeBtn, SIGNAL(clicked(bool)), SLOT(close()));
  connect(closeBtn, SIGNAL(clicked(bool)), SLOT(deleteLater()));

  QHBoxLayout *buttonsLayout = new QHBoxLayout(this);
  buttonsLayout->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
  buttonsLayout->setSpacing(30);

  playBtn = new IconButton(this); //连接服务器，开始接收/暂停接收
  playBtn->setIcon(QIcon(":/images/play.png"));
  buttonsLayout->addWidget(playBtn);
  connect(playBtn, SIGNAL(clicked(bool)), SLOT(playBtnSlot()));

  cameraBtn = new IconButton(this); //拍照
  cameraBtn->setIcon(QIcon(":/images/camera.png"));
  buttonsLayout->addWidget(cameraBtn);
  connect(cameraBtn, SIGNAL(clicked(bool)), SLOT(cameraBtnSlot()));

//  recordBtn = new IconButton(this); //录屏
//  recordBtn->setIcon(QIcon(":/images/record.png"));
//  buttonsLayout->addWidget(recordBtn);
//  connect(recordBtn, SIGNAL(clicked(bool)), SLOT(recordBtnSlot()));

//  fullScreenBtn = new IconButton(this); //全屏
//  fullScreenBtn->setIcon(QIcon(":/images/fullscreen.png"));
//  buttonsLayout->addWidget(fullScreenBtn);
//  connect(fullScreenBtn, SIGNAL(clicked(bool)), SLOT(fullScreenBtnSlot()));

    pauseBtn = new IconButton(this); //pause/continue
    pauseBtn->setIcon(QIcon(":/images/pause.png"));
    buttonsLayout->addWidget(pauseBtn);
    connect(pauseBtn, SIGNAL(clicked(bool)), SLOT(fullScreenBtnSlot()));
}

void OneVideo::createIpPortLabel() {
  ipPortLabel = new QLabel(this);
  ipPortLabel->setFont(QFont("Consolas", 10, 1));
  ipPortLabel->setMargin(10);
  ipPortLabel->resize(200, 50);
}

void OneVideo::createCheckBox() {
  edgeDetectCheckBox = new QCheckBox(this);
  edgeDetectCheckBox->setText("边缘检测");
  edgeDetectCheckBox->move(10, HEIGHT - 110);
  connect(edgeDetectCheckBox, SIGNAL(toggled(bool)),
          SLOT(edgeDetectSlot(bool)));

  sharpenCheckBox = new QCheckBox(this);
  sharpenCheckBox->setText("锐化");
  sharpenCheckBox->move(10, HEIGHT - 80);
  connect(sharpenCheckBox, SIGNAL(toggled(bool)), SLOT(sharpenSlot(bool)));


  thresholdCheckBox = new QCheckBox(this);
  thresholdCheckBox->setText("阈值分割");
  thresholdCheckBox->move(10, HEIGHT - 50);
  connect(thresholdCheckBox, SIGNAL(toggled(bool)), SLOT(thresholdSlot(bool)));
}

void OneVideo::playBtnSlot() {
  ConfigDialog dialog(this);
  if (isPlay) {
    isPlay = false;

    if (networkThread != NULL) {
      networkThread->setRunning(false);
      networkThread->wait();
      networkThread->deleteLater();
      networkThread = NULL;
    }

    playBtn->setIcon(QIcon(":/images/play.png"));
    ipPortLabel->clear();
    cameraBtn->setEnabled(false);
//    recordBtn->setEnabled(false);
//    fullScreenBtn->setEnabled(false);
    pauseBtn->setEnabled(false);
    edgeDetectCheckBox->setEnabled(false);
    sharpenCheckBox->setEnabled(false);
    thresholdCheckBox->setEnabled(false);
    centralImage.load(":/images/icon.png");
    update();
  } else {
    if (dialog.exec() == QDialog::Accepted) {
      networkThread = new RecvThread(dialog.getIP(), dialog.getPort());
      connect(networkThread, SIGNAL(disconnectSlot()), SLOT(disconnectSlot()));
      connect(networkThread, SIGNAL(transmitData(QImage)),
              SLOT(updateImage(QImage)));
      networkThread->start();

      playBtn->setIcon(QIcon(":/images/stop.png"));
      ipPortLabel->setText(QString("IP  : %1\nPORT: %2")
                               .arg(dialog.getIP())
                               .arg(dialog.getPort()));
      cameraBtn->setEnabled(true);
//      recordBtn->setEnabled(true);
//      fullScreenBtn->setEnabled(true);
      pauseBtn->setEnabled(true);
      edgeDetectCheckBox->setEnabled(true);
      sharpenCheckBox->setEnabled(true);
      thresholdCheckBox->setEnabled(true);
      isPlay = true;
    }
  }
}

void OneVideo::cameraBtnSlot() {
  QFile file;
  QString fileName =
      QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".jpg";
  qDebug() << "保存图片" << fileName;
  centralImage.save("./images/" + fileName, "JPEG");
  QMessageBox::information(this, "拍照",
                           tr("拍照成功，保存与%1").arg("./images/" + fileName));
}

void OneVideo::recordBtnSlot() {

}

void OneVideo::fullScreenBtnSlot() {
    if(isPause){
        isPause = false;
    }else{
        isPause = true;
    }
//    QMessageBox::information(this, "全屏","暂不支持全屏，开发中...");
  /*
  //先取消发送消息到小窗口
  disconnect(networkThread, SIGNAL(transmitData(QImage)),
             this, SLOT(updateImage(QImage)));

  //发送消息到全屏窗口
  FullScreenVideo fullScreenVideo(this);
  connect(networkThread, SIGNAL(transmitData(QImage)),
          &fullScreenVideo, SLOT(updateImage(QImage)));
  fullScreenVideo.exec();

  //恢复发送消息到小窗口
  connect(networkThread, SIGNAL(transmitData(QImage)),
          this, SLOT(updateImage(QImage)));*/
}

void OneVideo::updateImage(QImage image) {
  if (isPlay) {
    if(!isPause){
       centralImage = image;
       update();
    }
  }
}

void OneVideo::disconnectSlot() {
  QMessageBox::warning(this, "连接失败", "请注意！\n您的连接已经断开！");
  if (networkThread != NULL) {
    networkThread->setRunning(false);
    networkThread->wait();
    networkThread->deleteLater();
    networkThread = NULL;
  }
  playBtn->setIcon(QIcon(":/images/play.png"));
  ipPortLabel->clear();
  cameraBtn->setEnabled(false);
//  recordBtn->setEnabled(false);
//  fullScreenBtn->setEnabled(false);
  pauseBtn->setEnabled(false);
  centralImage.load(":/images/icon.png");
  update();
  isPlay = false;
}

void OneVideo::edgeDetectSlot(bool isChecked) {
  qDebug() << "边缘检测" << isChecked;
  networkThread->setIsEdgeDetect(isChecked);
}

void OneVideo::sharpenSlot(bool isChecked) {
  qDebug() << "锐化" << isChecked;
  networkThread->setIsSharpen(isChecked);
}

void OneVideo::thresholdSlot(bool isChecked)
{
    qDebug() << "阈值分割" << isChecked;
    networkThread->setIsThreshold(isChecked);
}

void OneVideo::closeEvent(QCloseEvent *event) {
  QFrame::closeEvent(event);
  if (networkThread != NULL) {
    networkThread->setRunning(false);
    networkThread->wait();
    networkThread->deleteLater();
    networkThread = NULL;
  }
  emit closeSignal(this);
}

void OneVideo::paintEvent(QPaintEvent *event) {
  QFrame::paintEvent(event);
  QPainter p(this);
  p.drawImage(width() / 2 - centralImage.width() / 2,
              height() / 2 - centralImage.height() / 2, centralImage);
}

void OneVideo::mouseMoveEvent(QMouseEvent *event) {
  if (!isButtonsShow) {
    isButtonsShow = true;
    closeBtn->show();
    playBtn->show();
    cameraBtn->show();
//    recordBtn->hide(); //目前是自动保存视频，此按钮先屏蔽
//    fullScreenBtn->show();
    pauseBtn->show();
    edgeDetectCheckBox->show();
    sharpenCheckBox->show();
    thresholdCheckBox->show();
  }
}

void OneVideo::leaveEvent(QEvent *event) {
  Q_UNUSED(event);
  if (isButtonsShow) {
    closeBtn->hide();
    playBtn->hide();
    cameraBtn->hide();
    pauseBtn->hide();
//    recordBtn->hide();
//    fullScreenBtn->hide();
    edgeDetectCheckBox->hide();
    sharpenCheckBox->hide();
    thresholdCheckBox->hide();
    isButtonsShow = false;
  }
}
