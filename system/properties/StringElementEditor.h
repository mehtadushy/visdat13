#ifndef StringElementEditorH
#define StringElementEditorH
//---------------------------------------------------------------------------
#include "ElementEditor.h"
//---------------------------------------------------------------------------

class QLineEdit;


class Object;
class ClassProperty;

class StringElementEditor : public ElementEditor
{
	Q_OBJECT

private:

	QLineEdit		*	m_Edit;
	ClassProperty	*	m_Property;
	Object			*	m_Object;

public:
	StringElementEditor(QWidget * parent=NULL, Qt::WFlags f=0);
	~StringElementEditor(void);

	/// setup in property mode
	void setup( Object * obj, pointer_diff offset, ClassProperty * prop, bool readOnly );

public slots:

	void sltTextChanged( const QString &text );
	void sltReturnPressed();

protected:

	/// update editor
	virtual void updateEditor();



};



#endif