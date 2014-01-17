//---------------------------------------------------------------------------
#ifndef EmbeddedObjectElementEditorH
#define EmbeddedObjectElementEditorH
//---------------------------------------------------------------------------
#include "ElementEditor.h"
#include "PTypes.h"
//---------------------------------------------------------------------------
#include <QtGui/QHBoxLayout>
//---------------------------------------------------------------------------



class EmbeddedObjectClassProperty;
class ClassEditor;
class MetaClass;
class Object;
class ClassProperty;

class EmbeddedObjectElementEditor : public ElementEditor
{
	Q_OBJECT

private:

	void openEditor( MetaClass * editor_mc );
	Object * getObject();
	
	EmbeddedObjectClassProperty	*	m_Property;
	Object			*	m_Object;
	bool			m_ReadOnly;

	QWidget	*		m_EditorWidget;

public:
	EmbeddedObjectElementEditor(QWidget * parent=NULL,Qt::WFlags f=0);
	~EmbeddedObjectElementEditor(void);

	/// setup in property mode
	void setup( Object * obj, pointer_diff offset, ClassProperty * prop, bool readOnly );

public slots:

	void sltButton();
	void sltDestroyEditorWidget( QObject * obj );

protected:

	/// update editor
	virtual void updateEditor() {}
	virtual void destroy( bool dw = true, bool ds = true );



};


// --- same as above, but showing all properties inline


class EmbeddedObjectInlineElementEditor : public ElementEditor
{
	Q_OBJECT

private:

	void openEditor( MetaClass * editor_mc );
	Object * getObject();
	
	EmbeddedObjectClassProperty	*	m_Property;
	Object			*	m_Object;
	bool			m_ReadOnly;

	QWidget	*		m_EditorWidget;
   QHBoxLayout * layout;
   ClassEditor *ce;
   
public:
	EmbeddedObjectInlineElementEditor(QWidget * parent=NULL,Qt::WFlags f=0);
	~EmbeddedObjectInlineElementEditor(void);

	/// setup in property mode
	void setup( Object * obj, pointer_diff offset, ClassProperty * prop, bool readOnly );

protected:

	/// update editor
	virtual void updateEditor() {}
};


#endif