//---------------------------------------------------------------------------
#ifndef ObjectPropertyEditorH
#define ObjectPropertyEditorH
//---------------------------------------------------------------------------
#include "PropertyEditor.h"
#include "Object.h"
//---------------------------------------------------------------------------



class ObjectElementEditor;

class ObjectPropertyEditor : public PropertyEditor
{
	GEOX_CLASS( ObjectPropertyEditor )

private:
	
	ObjectElementEditor	*	m_ElementEditor;

public:
	ObjectPropertyEditor(void);
	~ObjectPropertyEditor(void);

	/// creates an editor
	virtual QWidget *makeEditor(QWidget * parent, bool readOnly);	

	/// update editor
	virtual void update();

   virtual card32 getPreferredHeight();

};



#endif
