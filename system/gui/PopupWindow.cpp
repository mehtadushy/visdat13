//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "PopupWindow.h"
//---------------------------------------------------------------------------
#include <QtGui/QDesktopWidget>
#include <QtGui/QApplication>
//---------------------------------------------------------------------------



PopupWindow::PopupWindow(QWidget *parent) : QWidget(parent, Qt::Window | Qt::FramelessWindowHint) {
  setFocus(Qt::OtherFocusReason);
  idontcare = false;
}

#ifdef _WIN32
bool PopupWindow::winEvent (MSG *message, long *result ) {
  if (idontcare) return false;
  if (message->message == WM_ACTIVATE) {
    if ((message->wParam & 0xFFFF) == WA_INACTIVE) {
      idontcare = true;
      hide();
      idontcare = false;
    }
  }
  return false;
}
#else
void PopupWindow::leaveEvent(QEvent *event) {
  hide();
}
#endif

void PopupWindow::popup(int x, int y) {
   QRect screen = QApplication::desktop()->availableGeometry(this);
   if (x>screen.width()-width()) x -= x+width()-screen.width();
   if (y>screen.height()-height()) y -= y+height()-screen.height();
   if (x<0) x=0;
   if (y<0) y=0;
  move(x,y);
  show();
}

void PopupWindow::popup(QWidget *belowThis) {
  QPoint pos = belowThis->mapToGlobal(QPoint(0,0));
  int x = pos.x();
  int y = pos.y();
  y += belowThis->height();
  popup(x,y);
}

#ifndef CMAKE_INTDIR
#ifdef _WIN32
#include "moc/PopupWindow_moc.cpp"
#else
#ifndef CMAKE_UNIX_INCLUDE_MOC
#include "moc_PopupWindow.cpp"
#endif
#endif
#endif
