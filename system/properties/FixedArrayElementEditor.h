//---------------------------------------------------------------------------
#ifndef FixedArrayElementEditorH
#define FixedArrayElementEditorH
//---------------------------------------------------------------------------
#include "ElementEditor.h"
//---------------------------------------------------------------------------
#include <vector>
//---------------------------------------------------------------------------


using namespace std;

class FixedArrayClassProperty;
class PropertyEditor;
class ClassProperty;
class Object;

class FixedArrayElementEditor : public ElementEditor
{
private:
	
	FixedArrayClassProperty	*	m_Property;
	Object						*	m_Object;
	vector<PropertyEditor*>		m_Editors;
	pointer_diff					m_Offset;


public:
	FixedArrayElementEditor(QWidget * parent=NULL, Qt::WFlags f=0);
	~FixedArrayElementEditor(void);

	/// setup in property mode
	void setup( Object * obj, pointer_diff offset, ClassProperty * prop, bool readOnly );


protected:
		/// update editor
	virtual void updateEditor();

};


#endif