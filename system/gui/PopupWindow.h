//---------------------------------------------------------------------------
#ifndef PopupWindowH
#define PopupWindowH
//---------------------------------------------------------------------------
#include <QtGui/QWidget>
//---------------------------------------------------------------------------


class  PopupWindow : public QWidget {
	Q_OBJECT

 private:
	bool idontcare;
	
 protected:
	#ifdef _WIN32
	bool winEvent(MSG *message, long *result);
	#else
	void leaveEvent(QEvent *event);
	#endif
	
 public:
	PopupWindow(QWidget *parent);
	
	void popup(int x, int y);
	void popup(QWidget *belowThis);
};



#endif