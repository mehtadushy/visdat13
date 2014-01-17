//---------------------------------------------------------------------------
#ifndef ClassTypeClassPropertyEditorWidgetH 
#define ClassTypeClassPropertyEditorWidgetH
//---------------------------------------------------------------------------
#include "ClassEditor.h"
#include <QtGui/QComboBox>
#include "Delegate.h"
//---------------------------------------------------------------------------


using namespace std;

class ClassTypeClassProperty;

class  ClassTypeClassPropertyEditorWidget : public QComboBox
{
	Q_OBJECT

 private:
	void collectClassList(const string baseClass, QStringList &list );

	ClassTypeClassProperty	*	m_Object;
	bool initialized;
	Delegate callBack;

 public:
	ClassTypeClassPropertyEditorWidget(QWidget * parent = NULL, Qt::WFlags f=0);
	~ClassTypeClassPropertyEditorWidget(void);

	void setup( ClassTypeClassProperty * obj );
	void setNotify(Delegate callBack) {this->callBack = callBack;}
	
	/// update editor content with object data
	void updateEditor();

	void initialize(card32 ident, const MetaClass *mc);
   const MetaClass *getSelected();
   void setSelection(const MetaClass *mc);
	void collectClasses(const string baseClass, bool sortByName = false );
   void setSelection(const string className);

 private slots:
   virtual void selectionChanged(int index);

 signals:
   void sigModified();
   
};

MAKE_CLASS_EDITOR(  , ClassTypeClassProperty )


#endif
