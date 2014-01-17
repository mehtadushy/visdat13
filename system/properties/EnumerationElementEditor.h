//---------------------------------------------------------------------------
#ifndef EnumerationElementEditorH
#define EnumerationElementEditorH
//---------------------------------------------------------------------------
#include "ElementEditor.h"
//---------------------------------------------------------------------------

class QComboBox;

class Object;
class Card32ClassProperty;
class ClassProperty;

class EnumerationElementEditor : public ElementEditor
{
	Q_OBJECT

private:

	QComboBox		     *m_Edit;
	Card32ClassProperty *m_Property;
	Object				  *m_Object;
	bool                 m_internalChange;

public:
	EnumerationElementEditor(QWidget * parent=NULL, Qt::WFlags f=0);

	/// setup in property mode
	void setup( Object * obj, pointer_diff offset, ClassProperty * prop, bool readOnly );

public slots:

	void sltIndexChanged( int i );

protected:

	/// update editor
	virtual void updateEditor();



};



#endif
