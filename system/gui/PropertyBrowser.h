#ifndef PropertyBrowserH
#define PropertyBrowserH
//---------------------------------------------------------------------------
#include <QtGui/QScrollArea>
#include "ClassEditor.h"
//---------------------------------------------------------------------------

using namespace std;

class PropertyBrowserWidget;
class ClassProperty;
class PropertyEditor;

class  PropertyBrowser : public ClassEditor
{
	GEOX_CLASS( PropertyBrowser )

private:

	struct TEditEntry
	{
		TEditEntry( ClassProperty * p, PropertyEditor * e ) {prop = p; edit = e;}
		ClassProperty	*	prop;
		PropertyEditor	*	edit;
	};
	vector<TEditEntry>	m_Entries;
	Object	*				m_Object;
	PropertyBrowserWidget	*	m_Widget;

public:
	PropertyBrowser();
	~PropertyBrowser() {}

	virtual void apply() {}
	virtual void update();
	virtual card32 getPreferredWidth();
   virtual card32 getPreferredHeight();

	virtual QWidget * makeEditor( QWidget * parent, Object * obj );
	virtual Object * getObject() {return m_Object;}

};

class  PropertyBrowserWidget : public QScrollArea
{
	Q_OBJECT

private:

	struct TEditEntry
	{
		QWidget	*	labelWidget;
		QWidget	*	editorWidget;
		PropertyEditor *edit;
		TEditEntry( QWidget * lw, QWidget * ew, PropertyEditor *ed) {labelWidget = lw; editorWidget = ew; edit = ed;}
	};
	vector<TEditEntry>	m_Entries;
	card32					m_Height;
	QWidget			*		m_MainWidget;


public:
	PropertyBrowserWidget(QWidget * parent);
	~PropertyBrowserWidget(void) {}

	void setup( Object * obj );
	void update();

public slots:

	void sltModified();

signals:

	void sigModified();

protected:

	void resizeEvent( QResizeEvent * e);

};



#endif
