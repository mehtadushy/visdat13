//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ClassTypeClassPropertyEditorWidget.h"
//---------------------------------------------------------------------------
#include "ClassTypeClassProperty.h"
//---------------------------------------------------------------------------



IMPLEMENT_CLASS_EDITOR( ClassTypeClassProperty )

ClassTypeClassPropertyEditorWidget::ClassTypeClassPropertyEditorWidget(QWidget * parent, Qt::WFlags f)
:QComboBox(parent), callBack(0)
{
   m_Object=NULL;
   initialized = false;
   QObject::connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(selectionChanged(int)));
}

ClassTypeClassPropertyEditorWidget::~ClassTypeClassPropertyEditorWidget(void)
{
}

void ClassTypeClassPropertyEditorWidget::setup( ClassTypeClassProperty * obj )
{
  m_Object = obj;


}

void ClassTypeClassPropertyEditorWidget::updateEditor()
{

//	const MetaClass *mc = ((ClassTypeClassProperty*)m_Object)->getValue((Object*)obj);
   if (!initialized) {
      initialize(0, ((ClassTypeClassProperty*)m_Object)->getBaseClass());
    setCurrentIndex(0);
      initialized = true;
   }
//   setSelection(mc);
}


void ClassTypeClassPropertyEditorWidget::collectClasses(const string baseClass, bool sortByName )
{
  const MetaClass * mc = MetaClass::getClassByName( baseClass );
  QStringList list;
  if( !mc->isAbstract() )
    list << baseClass.c_str();

  collectClassList( baseClass, list);
  if( sortByName )
    list.sort();

  addItems( list );
}

void ClassTypeClassPropertyEditorWidget::collectClassList(const string baseClass,QStringList &list )
{
  const MetaClass * mc = MetaClass::getClassByName( baseClass );
  for( card32 i=0;i<mc->getNumDescendants();i++ )
  {
    string className = mc->getDescendant(i)->getClassName();
    list << className.c_str();
    collectClassList( className, list );
  }
}


void ClassTypeClassPropertyEditorWidget::setSelection(const string className){
  QString x=className.c_str();
  int index=findText(x, Qt::MatchEndsWith | Qt::MatchCaseSensitive);
  setCurrentIndex(index);
}


void ClassTypeClassPropertyEditorWidget::initialize(card32 ident, const MetaClass *mc) {
   string identStr;
   for (card32 i=0; i<ident; i++) {
      identStr += "  ";
   }

  // Qt dosen't support sotring mc pointer, using classnames (=> identStr="")
  identStr="";


   if (mc->isAbstract()) {
    //addItem((identStr+mc->getClassName() + string(" (abstract)")).c_str());
      //cbClasses->Items->AddObject((identStr+mc->getClassName() + string(" (abstract)")).c_str(), (System::TObject*)mc);
   } else {
    addItem((identStr+mc->getClassName()).c_str());
      //cbClasses->Items->AddObject((identStr+mc->getClassName()).c_str(), (System::TObject*)mc);
   }
   const card32 numDescendants = mc->getNumDescendants();

   for (card32 i=0; i<numDescendants; i++) {
      initialize(ident+1, mc->getDescendant(i));
   }
  this->setCurrentIndex(0);

}

const MetaClass* ClassTypeClassPropertyEditorWidget::getSelected() {
   card32 i = currentIndex();
  const MetaClass *mc = MetaClass::getClassByName(qPrintable(this->itemText(i)));

   return mc;
}

void ClassTypeClassPropertyEditorWidget::setSelection(const MetaClass *mc) {
  setSelection(mc->getClassName());
}

void ClassTypeClassPropertyEditorWidget::selectionChanged(int index) {
   if (callBack != NULL) {
      callBack();
      emit sigModified();
   }
}



#ifndef CMAKE_INTDIR
#ifdef _WIN32
#include "moc/ClassTypeClassPropertyEditorWidget_moc.cpp"
#else
#ifndef CMAKE_UNIX_INCLUDE_MOC
#include "moc_ClassTypeClassPropertyEditorWidget.cpp"
#endif
#endif
#endif
