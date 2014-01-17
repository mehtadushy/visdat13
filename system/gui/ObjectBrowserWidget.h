//---------------------------------------------------------------------------
#ifndef ObjectBrowserWidgetH
#define ObjectBrowserWidgetH
//---------------------------------------------------------------------------
#include "ui_ObjectBrowserWidgetDesigner.h"
//---------------------------------------------------------------------------
#include <QtGui/QDialog>
#include <map>
//---------------------------------------------------------------------------

using namespace std;


class QHBoxLayout;
class ClassEditor;
class Object;
class MetaClass;

/// a dialog that displays an object editor, with some additional options
class  ObjectBrowserWidget : public QDialog {
   Q_OBJECT
 
 private:
   Ui_ObjectBrowserWidget ui;
   Object *obj;
   QHBoxLayout *edLayout;  
   QWidget *edWidget;
   ClassEditor *clEd;

	QAction	*	m_SaveObjectAction;
	QAction	*	m_UpdateAction;
	QMenu		*		m_Menu;
	QMenu		*		m_EditorsMenu;

	struct TMetaInfo
	{
		MetaClass * mc;
		Object	*	obj;
		string		name;
		bool			isDefault;
	};
	map<QAction*, TMetaInfo >	m_EditorClasses;
   
 protected:
	void keyPressEvent(QKeyEvent *event);

 public:
   /// create object specifying the object to be edited and the parent widget this dialog is attached to
   ObjectBrowserWidget(Object *obj, ClassEditor *clEd, QWidget *parent = 0, Qt::WFlags f = 0);
   /// after creating the widget, this method can be called to obtain the reference to the container the new editor is to be placed into
   QWidget *getEditorContainer();
   /// and then, setupEditor must be called on the widget to layout it correctly and register it for editor exchange
   void setupEditor(QWidget *ed);
   
 public slots:
   void sltHandleMenu(QAction *action);
   void on_editButton_clicked();
   void sltUpdate(bool x);
   
};




#endif