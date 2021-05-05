#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "onevideo.h"

#include <QMainWindow>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  //创建菜单
  void CreateMenu();

  //创建内容
  void CreateContent();

  //重新布局
  void layoutChild();

private slots:
  //添加窗口
  void slotAddVideoWindow();

  //子窗口关闭
  void slotChildClosed(OneVideo *who);

  //删除所有子窗口
  void slotRemoveAllWindow();

  //关于
  void slotShowAbout();

protected:
  void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
  Ui::MainWindow *ui;
  //子窗口容器
  QList<OneVideo *> showList;
  // UI元素
  QWidget *mainContent;
  QPushButton *addBtn; //添加窗口
};

#endif // MAINWINDOW_H
