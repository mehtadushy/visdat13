//---------------------------------------------------------------------------
#ifndef VariableArrayElementEditorH
#define VariableArrayElementEditorH
//---------------------------------------------------------------------------
#include "ElementEditor.h"
//---------------------------------------------------------------------------

class QTreeWidget;
class QPushButton;
class QTreeWidgetItem;

class VariableArrayClassProperty;
class Object;
class ClassSelectionDialog;
class MetaClass;
class PropertyEditor;
class ClassProperty;

class  VariableArrayElementEditor : public ElementEditor
{
	Q_OBJECT

private:

	void updateView();
	void openEditor( MetaClass * mc, card32 index );

	QTreeWidget		*	m_TreeView;
	QPushButton		*	m_Add, *m_Delete;
	ClassSelectionDialog	*	m_ClassSelectionDialog;
	QWidget			*	m_EditorParent;

	VariableArrayClassProperty	*	m_Property;
	Object			*	m_Object;
	bool					m_ReadOnly;
	
	QWidget			*	m_ActEditor;
	PropertyEditor	*	m_ActPropertyEditor;

public:
	VariableArrayElementEditor(QWidget * parent=NULL, Qt::WFlags f=0);

	/// setup in property mode
	void setup( Object * obj, pointer_diff offset, ClassProperty * prop, bool readOnly );

public slots:

	void sltAdd();
	void sltDelete();
	void sltItemActivated( QTreeWidgetItem * item, int column );
	void sltModified();
	void sltEditorDestroyed( QObject * obj );

protected:

	/// update editor
	virtual void updateEditor();
	void destroy(bool dw=true,bool ds=true);

};



#endif