//---------------------------------------------------------------------------
#ifndef MethodBrowserH 
#define MethodBrowserH
//---------------------------------------------------------------------------
#include "ClassEditor.h"
//---------------------------------------------------------------------------
#include <QtGui/QScrollArea>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
//---------------------------------------------------------------------------

class QFrame;

using namespace std;

class NoArgsClassMethod;
class MethodBrowserWidget;


class  MethodBrowser : public ClassEditor {
private:
	static MetaClass *MethodBrowserMetaClass;
   static Object* newInstance();

	MethodBrowserWidget  *m_Widget;
	Object	*	m_Object;

public:
	MethodBrowser() {m_Widget = NULL; m_Object = NULL;}

   virtual const MetaClass *getInstanceClass() const;
   static const MetaClass *getClass();
   static void init(const MetaClass *parentClass);
   static void shutdown();

   virtual QWidget *makeEditor(QWidget *parent, Object *obj);
	virtual void apply() {}
   virtual void update();
   virtual card32 getPreferredWidth();
   virtual card32 getPreferredHeight();
   virtual Object *getObject();

	virtual bool standAloneEditorApplication();

	virtual ~MethodBrowser();
};



class  MethodBrowserCustomButton : public QPushButton {
	Q_OBJECT
	
 private:
	int index;

 private slots:
	void receiveClick();
		
 public:
	MethodBrowserCustomButton ( int index, QWidget * parent = 0 );
	MethodBrowserCustomButton ( int index, const QString & text, QWidget * parent = 0 );
	MethodBrowserCustomButton ( int index, const QIcon & icon, const QString & text, QWidget * parent = 0 );

 signals:
	void methodClicked(int index);
};



class  MethodBrowserWidget : public QScrollArea
{
	Q_OBJECT

 private:

	Object* m_Object;
	vector<const NoArgsClassMethod*> methods;
	int defaultHeight;
	
	QFrame *frame;
	QVBoxLayout *layout;
	
 private slots:
	void methodClicked(int index);

 public:
	MethodBrowserWidget(QWidget * parent = NULL);
	~MethodBrowserWidget(void);

	void setup( Object* obj );
	int getDefaultHeight() {return defaultHeight;}
	
	void updateEditor();


};




#endif