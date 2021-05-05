#ifndef ICONBUTTON_H
#define ICONBUTTON_H

#include <QPushButton>

class IconButton : public QPushButton {
  Q_OBJECT
public:
  explicit IconButton(QWidget *parent = 0);

protected:
  void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
  void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
  void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};

#endif // ICONBUTTON_H
