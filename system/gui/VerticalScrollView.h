//---------------------------------------------------------------------------
#ifndef VerticalScrollViewH
#define VerticalScrollViewH
//---------------------------------------------------------------------------
#include <QtGui/QScrollArea>
//---------------------------------------------------------------------------


/**
	holds a content widget and resizes it only in horizontal direction.
	vertical size is fixed by the content widgets height.
*/
class VerticalScrollView : public QScrollArea
{
   Q_OBJECT
 private:

	QWidget * m_Content;

 public:
	VerticalScrollView(QWidget * parent);

 protected:

	void resizeEvent( QResizeEvent * e );
};

#endif
