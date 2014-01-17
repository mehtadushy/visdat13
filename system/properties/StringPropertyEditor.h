//---------------------------------------------------------------------------
#ifndef StringPropertyEditorH
#define StringPropertyEditorH
//---------------------------------------------------------------------------
#include "PropertyEditor.h"
//---------------------------------------------------------------------------



class StringElementEditor;

class StringPropertyEditor : public PropertyEditor
{
	GEOX_CLASS( StringPropertyEditor )

private:

	StringElementEditor	*	m_ElementEditor;

public:
	StringPropertyEditor(void);
	~StringPropertyEditor(void);

	/// creates an editor
	virtual QWidget *makeEditor(QWidget * parent, bool readOnly);	

	/// update editor
	virtual void update();

   virtual card32 getPreferredHeight();


};



#endif
