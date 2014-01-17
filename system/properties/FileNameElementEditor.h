//---------------------------------------------------------------------------
#ifndef FileNameElementEditorH
#define FileNameElementEditorH
//---------------------------------------------------------------------------
#include "ElementEditor.h"
//---------------------------------------------------------------------------

class QLineEdit;
class QPushButton;

class ClassProperty;
class Object;



/**
	edit string properties with file dialog
*/
class  FileNameElementEditor : public ElementEditor
{
	Q_OBJECT

private:
	QLineEdit		*	m_Edit;
	ClassProperty	*	m_Property;
	Object			*	m_Object;
	QPushButton    *  m_dotdotdot;


protected:
	/// update editor
	virtual void updateEditor();

public:
	FileNameElementEditor(QWidget * parent=NULL, Qt::WFlags f=0);
	~FileNameElementEditor();

	/// setup in property mode
	void setup( Object * obj, pointer_diff offset, ClassProperty * prop, bool readOnly );

public slots:

	void sltTextChanged( const QString &text );
	void sltReturnPressed();
	void sltDotDotDotClicked();

};



#endif