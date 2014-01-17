//---------------------------------------------------------------------------
#ifndef ObjectElementEditorH
#define ObjectElementEditorH
//---------------------------------------------------------------------------
#include "ElementEditor.h"
#include "Object.h"
//---------------------------------------------------------------------------
#include <map>
using namespace std;
//---------------------------------------------------------------------------

class QAction;
class QPushButton;
class QMenu;
class QLineEdit;

class Object;
class ClassProperty;
class ClassSelectionDialog;
class ObjectClassProperty;
class MetaClass;

class  ObjectElementEditor : public ElementEditor
{
	Q_OBJECT

private:

	void updateMenu();
	void openEditor( MetaClass * editor_mc );
	//Object ** getObjectPointer();

	Object * getObject();
	void setObject( Object * obj );
	
	ObjectClassProperty	*	m_Property;
	Object			*	m_Object;
	Object			**	m_ObjectPointer;
	MetaClass		*	m_BaseClass;
	ClassSelectionDialog	*	m_ClassSelectionDialog;
	bool			m_ReadOnly;
	bool			m_IsOwner;

	QAction	*	m_ReplaceObjectAction;
	QAction	*	m_SaveObjectAction;
	QAction	*	m_LoadObjectAction;

	QMenu		*		m_Menu;
	QMenu		*		m_EditorsMenu;

	QWidget	*		m_EditorWidget;
	MetaClass	*	m_ActMetaClass;
	QLineEdit	*	m_TypeAddress;
	
	QPushButton * button_edit;

	struct TMetaInfo
	{
		MetaClass * mc;
		Object	*	obj;
		string		name;
		bool			isDefault;
	};
	map<QAction*, TMetaInfo >	m_EditorClasses;

public:
	ObjectElementEditor(QWidget * parent=NULL,Qt::WFlags f=0);
	~ObjectElementEditor(void);

	/// setup in direct access mode ( for custom editors )
	void setup( Object *& obj, const MetaClass * baseClass, bool readOnly = false);

	/// setup in property mode
	void setup( Object * obj, pointer_diff offset, ClassProperty * prop, bool readOnly );

public slots:

	void sltEditButton();
	void sltViewObjectButton();
	void sltHandleMenu( QAction * action );
	void sltDestroyEditorWidget( QObject * obj );

protected:

	/// update editor
	virtual void updateEditor();
	virtual void destroy( bool dw = true, bool ds = true );

};



#endif