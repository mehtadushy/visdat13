//---------------------------------------------------------------------------
#ifndef BooleanPropertyEditorH
#define BooleanPropertyEditorH
//---------------------------------------------------------------------------
#include "PropertyEditor.h"
//---------------------------------------------------------------------------


class BooleanElementEditor;

class BooleanPropertyEditor : public PropertyEditor
{
	GEOX_CLASS( BooleanPropertyEditor )

private:

	BooleanElementEditor	*	m_ElementEditor;


public:
	BooleanPropertyEditor(void);
	~BooleanPropertyEditor(void);

	/// creates an editor
	virtual QWidget *makeEditor(QWidget * parent, bool readOnly);	

	/// update editor
	virtual void update();

   virtual card32 getPreferredHeight();



};



#endif
