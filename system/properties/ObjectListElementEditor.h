//---------------------------------------------------------------------------
#ifndef ObjectListElementEditorH
#define ObjectListElementEditorH
//---------------------------------------------------------------------------
#include "ElementEditor.h"
#include <QtGui/QTreeWidgetItem>
//---------------------------------------------------------------------------

class QPushButton;
class QTreeWidget;
class QTreeWidget;
class QLabel;
class QWidget;


class Object;
class ObjectListProperty;
class ClassSelectionDialog;
class MetaClass;
class ClassProperty;

class ObjectListElementEditor : public ElementEditor
{
	Q_OBJECT

private:

	class	ObjectListItem : public QTreeWidgetItem
	{
		Object	*	m_MyItem;
		int			m_Index;
	public:
		ObjectListItem( QTreeWidget * parent, Object * obj, int index ) 
			: QTreeWidgetItem(parent) {m_MyItem = obj;m_Index=index;}
		Object * getObject() {return m_MyItem;}
		int getIndex() const {return m_Index;}
	};

	void updateTreeView();
	void openEditor( Object * obj, const MetaClass * mc );

	Object					*	m_Object;
	ObjectListProperty	*	m_ObjectList;

	QPushButton				*	m_ToTop, *m_ToBottom, *m_Up, *m_Down, *m_Edit, 
		*m_Delete, *m_Add, *m_AddAddr,*m_EditMember;
	QTreeWidget				*	m_TreeView;
	QLabel					*	m_NumElementsLabel;
	
	QWidget					*	m_SubObjectWidget;
	ClassSelectionDialog	*	m_ClassSelectionDialog;

public:
	ObjectListElementEditor(QWidget * parent=NULL,Qt::WFlags f=0);

	/// setup in property mode
	void setup( Object * obj, pointer_diff offset, ClassProperty * prop, bool readOnly );

public slots:

	void sltMoveToTop();
	void sltMoveToBottom();
	void sltMoveUp();
	void sltMoveDown();
	void sltEdit();
	void sltEditMember();
	void sltDelete();
	void sltAdd();
	void sltAddAddr();
	void sltItemDoubleClicked ( QTreeWidgetItem * item, int column );
	void sltEditorDestroyed( QObject * obj );

	void sltSubObjectModified();

protected:

	/// update editor
	virtual void updateEditor() {updateTreeView();}
	void destroy(bool dw=true,bool ds=true);

};



#endif