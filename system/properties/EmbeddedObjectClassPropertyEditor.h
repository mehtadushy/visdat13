//---------------------------------------------------------------------------
#ifndef EmbeddedObjectClassPropertyEditorH
#define EmbeddedObjectClassPropertyEditorH
//---------------------------------------------------------------------------
#include "ElementEditor.h"
#include "PropertyEditor.h"
//---------------------------------------------------------------------------



class EmbeddedObjectElementEditor;
class EmbeddedObjectInlineElementEditor;


class  EmbeddedObjectClassPropertyEditor : public PropertyEditor
{
	GEOX_CLASS( EmbeddedObjectClassPropertyEditor )

private:
	
	EmbeddedObjectElementEditor	*	m_ElementEditor;

public:
	EmbeddedObjectClassPropertyEditor(void);

	/// creates an editor
	virtual QWidget *makeEditor(QWidget * parent, bool readOnly);	

	/// update editor
	virtual void update();

   virtual card32 getPreferredHeight();

};


// ----


class  EmbeddedObjectClassPropertyInlineEditor : public PropertyEditor
{
	GEOX_CLASS( EmbeddedObjectClassPropertyInlineEditor )

private:
	
	EmbeddedObjectInlineElementEditor	*	m_ElementEditor;

public:
	EmbeddedObjectClassPropertyInlineEditor(void);

	/// creates an editor
	virtual QWidget *makeEditor(QWidget * parent, bool readOnly);	

	/// update editor
	virtual void update();

   virtual card32 getPreferredHeight();

};



#endif