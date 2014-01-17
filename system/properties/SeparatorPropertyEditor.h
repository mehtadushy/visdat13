//---------------------------------------------------------------------------
#ifndef SeparatorPropertyEditorH 
#define SeparatorPropertyEditorH
//---------------------------------------------------------------------------
#include "PropertyEditor.h"
#include "ElementEditor.h"
//---------------------------------------------------------------------------


class SeparatorPropertyEditor : public PropertyEditor
{
	GEOX_CLASS( SeparatorPropertyEditor )
	
	class MyElementEditor : public ElementEditor {
	 public:
		MyElementEditor(QWidget * parent=NULL, Qt::WFlags f=0) : ElementEditor(parent, f) {};
		virtual void updateEditor() {};	 
	};

public:
	SeparatorPropertyEditor() {}

	virtual QWidget *makeEditor(QWidget * parent, bool readOnly);	
	virtual void update() {}
   virtual card32 getPreferredHeight();


};



#endif