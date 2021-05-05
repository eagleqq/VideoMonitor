#include "mainwindow.h"
#include "loginsystemdialog.h"
#include "onevideo.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScrollArea>
#include <QMessageBox>
#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  this->setWindowIcon(QIcon(":/images/icon.png"));
  //屏幕居中
//  this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
//                                        this->size(),
//                                        qApp->desktop()->availableGeometry()));
  CreateMenu();
  CreateContent();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::CreateMenu() {
  //菜单栏
  QMenuBar *menuBar = new QMenuBar(this);

  QMenu *menuFile = new QMenu("文件(&F)", menuBar);
  QMenu *menuHelp = new QMenu("帮助(&H)", menuBar);
  menuBar->addAction(menuFile->menuAction());
  menuBar->addAction(menuHelp->menuAction());
  setMenuBar(menuBar);

  QAction *actionAdd = new QAction("添加窗口(&A)", menuFile);
  QAction *actionRemove = new QAction("删除所有窗口(&D)", menuFile);
  QAction *actionExit = new QAction("退出(&Q)", menuFile);
  QAction *actionAbout = new QAction("关于(&A)", menuHelp);
  menuFile->addAction(actionAdd);
  menuFile->addAction(actionRemove);
  menuFile->addSeparator();
  menuFile->addAction(actionExit);
  menuHelp->addAction(actionAbout);

  connect(actionAdd, SIGNAL(triggered(bool)), SLOT(slotAddVideoWindow()));
  connect(actionRemove, SIGNAL(triggered(bool)), SLOT(slotRemoveAllWindow()));
  connect(actionExit, SIGNAL(triggered(bool)), SLOT(close()));
  connect(actionAbout, SIGNAL(triggered(bool)), SLOT(slotShowAbout()));
}

void MainWindow::CreateContent() {
  QScrollArea *scrollArea = new QScrollArea(this);
  scrollArea->setFrameStyle(QFrame::NoFrame);

  mainContent = new QWidget(scrollArea);
  mainContent->resize(size());
  scrollArea->setWidget(mainContent);
  setCentralWidget(scrollArea);

  addBtn = new QPushButton("+", mainContent);
  addBtn->setGeometry(0, 0, OneVideo::WIDTH, OneVideo::HEIGHT);
  addBtn->setFont(QFont("黑体", 200, 87));
  addBtn->setVisible(true);

  connect(addBtn, SIGNAL(clicked(bool)), SLOT(slotAddVideoWindow()));
}

void MainWindow::layoutChild() {
  //安排子窗体
  int cols = (width() - 20) / OneVideo::WIDTH; //动态获取列数
  int i;
  try {
    for (i = 0; i < showList.size(); ++i) {
      showList.at(i)->move((i % cols) * OneVideo::WIDTH,
                           (i / cols) * OneVideo::HEIGHT);
    }
  } catch (...) {
    std::cout << "重新布局异常\n";
  }
  //安排添加按钮位置
  addBtn->move((i % cols) * OneVideo::WIDTH, (i / cols) * OneVideo::HEIGHT);

  //重设ScrollArea大小
  int sw = width() - 20;
  int sh = ((i / cols) + 1) * OneVideo::HEIGHT;
  mainContent->resize(sw, sh);
}

void MainWindow::slotAddVideoWindow() {
  OneVideo *one = new OneVideo(mainContent); //设置父子关系就可以显示出子界面
  connect(one, SIGNAL(closeSignal(OneVideo *)),
          SLOT(slotChildClosed(OneVideo *)));
  one->show();
  showList.append(one);
  layoutChild();
}

void MainWindow::slotChildClosed(OneVideo *who) {
  showList.removeOne(who);
  layoutChild();
}

void MainWindow::slotRemoveAllWindow() {
  for (OneVideo *one : showList) {
    one->close();
    one->deleteLater();
  }
  showList.clear();
  layoutChild();
}

void MainWindow::slotShowAbout()
{
    QMessageBox::information(this, "关于","软件名称：视频监控系统V1.0.0 \n\n "
                                        "软件依赖：Qt5.6.3 | Opencv3.4 | V4L2 | SQLite\n "
                                        "支持系统：ubuntu16.04");
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);
  layoutChild();
}
