//---------------------------------------------------------------------------
#ifndef ObjectCreationDialogH
#define ObjectCreationDialogH
//---------------------------------------------------------------------------
#include <QtGui/QDialog>
//---------------------------------------------------------------------------
#include "ui_ObjectCreationDialogDesigner.h"
//---------------------------------------------------------------------------

class ClassCreator;
class Object;

/**
	Dialog for interactive creation of new object instances
	Used to display the (default) editor of a class creator
	and let the user edit its properties.
*/
class ObjectCreationDialog : public QDialog {
	Q_OBJECT
 private:
	Ui_ObjectCreationDialog ui;

 public:
	ObjectCreationDialog(QWidget* parent);
	/// display a dialog that lets the user create a new object 
	/// returns NULL if user cancels
	static Object *execute(QWidget *parent, ClassCreator *cc);

};



#endif
