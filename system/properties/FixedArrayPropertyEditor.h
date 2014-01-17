#ifndef FixedArrayPropertyEditorH
#define FixedArrayPropertyEditorH
//---------------------------------------------------------------------------
#include "PropertyEditor.h"
//---------------------------------------------------------------------------


class FixedArrayElementEditor;

class FixedArrayPropertyEditor : public PropertyEditor
{
	GEOX_CLASS( FixedArrayPropertyEditor )

private:

	FixedArrayElementEditor	*	m_ElementEditor;


public:
	FixedArrayPropertyEditor(void);
	~FixedArrayPropertyEditor(void);

	/// creates an editor
	virtual QWidget *makeEditor(QWidget * parent, bool readOnly);	

	/// update editor
	virtual void update();

   virtual card32 getPreferredHeight();



};



#endif
