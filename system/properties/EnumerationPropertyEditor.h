//---------------------------------------------------------------------------
#ifndef EnumerationPropertyEditorH
#define EnumerationPropertyEditorH
//---------------------------------------------------------------------------
#include "PropertyEditor.h"
//---------------------------------------------------------------------------


class EnumerationElementEditor;

class EnumerationPropertyEditor : public PropertyEditor
{
	GEOX_CLASS( EnumerationPropertyEditor )

private:

	EnumerationElementEditor	*	m_ElementEditor;

public:
	EnumerationPropertyEditor(void);

	/// creates an editor
	virtual QWidget *makeEditor(QWidget * parent, bool readOnly);	

	/// update editor
	virtual void update();

   virtual card32 getPreferredHeight();


};


#endif
