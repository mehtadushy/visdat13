//---------------------------------------------------------------------------
#ifndef ObjectListPropertyEditorH
#define ObjectListPropertyEditorH
//---------------------------------------------------------------------------
#include "PropertyEditor.h"
//---------------------------------------------------------------------------



class ObjectListElementEditor;

class ObjectListPropertyEditor : public PropertyEditor
{
	GEOX_CLASS( ObjectListPropertyEditor )

private:
	
	ObjectListElementEditor	*	m_ElementEditor;
public:
	ObjectListPropertyEditor(void);

	/// creates an editor
	virtual QWidget *makeEditor(QWidget * parent, bool readOnly);	

	/// update editor
	virtual void update();

   virtual card32 getPreferredHeight();
};



#endif