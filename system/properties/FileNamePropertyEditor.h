//---------------------------------------------------------------------------
#ifndef FileNamePropertyEditorH
#define FileNamePropertyEditorH
//---------------------------------------------------------------------------
#include "PropertyEditor.h"
//---------------------------------------------------------------------------



class FileNameElementEditor;

class  FileNamePropertyEditor : public PropertyEditor
{
	GEOX_CLASS( FileNamePropertyEditor )

private:

	FileNameElementEditor	*	m_ElementEditor;

public:
	FileNamePropertyEditor(void);
	~FileNamePropertyEditor(void);

	/// creates an editor
	virtual QWidget *makeEditor(QWidget * parent, bool readOnly);	

	/// update editor
	virtual void update();

   virtual card32 getPreferredHeight();


};



#endif
