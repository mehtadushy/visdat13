//---------------------------------------------------------------------------
#ifndef BooleanElementEditorH
#define BooleanElementEditorH
//---------------------------------------------------------------------------
#include "ElementEditor.h"
//---------------------------------------------------------------------------

class QCheckBox;
class QWidget;

class Object;
class ClassProperty;


class BooleanElementEditor : public ElementEditor
{
	Q_OBJECT

private:
	QCheckBox	*		m_CheckBox;
	ClassProperty	*	m_Property;
	Object			*	m_Object;
	bool				*	m_Pointer;


public:
	BooleanElementEditor(QWidget * parent=NULL, Qt::WFlags f=0);
	~BooleanElementEditor(void);

	/// setup in direct access mode ( for custom editors )
	void setup( bool &b, bool readOnly );

	/// setup in property mode
	void setup( Object * obj, pointer_diff offset, ClassProperty * prop, bool readOnly );

public slots:

	void sltStateChanged( int state );

protected:
	/// update editor
	virtual void updateEditor();
	virtual void resizeEvent( QResizeEvent * e);

private:

	bool getElement();
	void setElement( bool b );


};



#endif
