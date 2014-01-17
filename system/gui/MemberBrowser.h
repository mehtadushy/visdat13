//---------------------------------------------------------------------------
#ifndef MemberBrowserH 
#define MemberBrowserH
//---------------------------------------------------------------------------
#include "ClassEditor.h"
//---------------------------------------------------------------------------
#include <QtGui/QSplitter>
//---------------------------------------------------------------------------

	
class PropertyBrowser;
class MethodBrowser;
class MemberBrowserWidget;


class  MemberBrowser : public ClassEditor {
private:
	static MetaClass *MemberBrowserMetaClass;
   static Object* newInstance();

	MemberBrowserWidget  *m_Widget;
	Object	*	m_Object;
	
	PropertyBrowser *propBrowser;
	MethodBrowser *methodBrowser;

public:
	MemberBrowser() {m_Widget = NULL; m_Object = NULL;}

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

	virtual ~MemberBrowser();
};



class  MemberBrowserWidget : public QSplitter {

	Q_OBJECT

 private:
	Object* m_Object;
	
 public:
	MemberBrowserWidget(QWidget * parent = NULL);
	~MemberBrowserWidget(void);

	void setup(Object* obj, PropertyBrowser *propBrowser, MethodBrowser *methodBrowser);
	
	void updateEditor();
};



#endif