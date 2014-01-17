//---------------------------------------------------------------------------
#ifndef ClassTypePropertyEditorH
#define ClassTypePropertyEditorH
//---------------------------------------------------------------------------
#include "PropertyEditor.h"
#include "Object.h"
//---------------------------------------------------------------------------


class ClassTypeClassPropertyEditorWidget;

class ClassTypePropertyEditor : public PropertyEditor
{
	GEOX_CLASS( ClassTypePropertyEditor )

private:
	
	ClassTypeClassPropertyEditorWidget	*	m_ElementEditor;

	void writeBackSelection();
	
public:
	ClassTypePropertyEditor();
	~ClassTypePropertyEditor();

	/// creates an editor
	virtual QWidget *makeEditor(QWidget * parent, bool readOnly);	

	/// update editor
	virtual void update();

   virtual card32 getPreferredHeight();

};




#endif