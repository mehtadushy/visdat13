//---------------------------------------------------------------------------
#ifndef VariableArrayClassPropertyEditorH
#define VariableArrayClassPropertyEditorH
//---------------------------------------------------------------------------
#include "PropertyEditor.h"
//---------------------------------------------------------------------------



class VariableArrayElementEditor;
	
class  VariableArrayClassPropertyEditor : public PropertyEditor
{
	GEOX_CLASS( VariableArrayClassPropertyEditor )

private:
	VariableArrayElementEditor	*	m_ElementEditor;	

public:
	VariableArrayClassPropertyEditor(void);

	/// creates an editor
	virtual QWidget *makeEditor(QWidget * parent, bool readOnly);	

	/// update editor
	virtual void update();

   virtual card32 getPreferredHeight();

};



#endif