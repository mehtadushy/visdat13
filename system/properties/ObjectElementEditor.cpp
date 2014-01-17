//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ObjectElementEditor.h"
//---------------------------------------------------------------------------
#include <QtGui/QMenu>
#include <QtGui/QPushButton>
#include <QtGui/QBoxLayout>
#include <QtGui/QFileDialog>
#include <QtGui/QDesktopWidget>
#include <QtGui/QLineEdit>
#include "ObjectClassProperty.h"
#include "ClassSelectionDialog.h"
#include "ClassProperty.h"
#include "MetaClass.h"
#include "BinaryObjectStreams.h"
#include "ClassEditor.h"
#include "ObjectViewsTable.h"
#include "Tools.h"
#include "ObjectBrowserWidget.h"
#include "FileDialogs.h"
#include "ExceptionHandling.h"
//---------------------------------------------------------------------------



ObjectElementEditor::ObjectElementEditor(QWidget * parent,Qt::WFlags f)
:ElementEditor(parent,f)
{
  m_ObjectPointer = NULL;
  m_Object = NULL;
  m_Property = NULL;
  m_BaseClass = NULL;
  m_ReadOnly = false;
  m_IsOwner = true;
  m_ClassSelectionDialog = new ClassSelectionDialog(this);
  m_ClassSelectionDialog->hide();

  m_Menu = new QMenu(this);
  m_Menu->hide();
  connect( m_Menu, SIGNAL(triggered(QAction*)), SLOT(sltHandleMenu(QAction*)));
  m_EditorsMenu = m_Menu->addMenu( "Editors" );
  m_Menu->addSeparator();
  m_ReplaceObjectAction = m_Menu->addAction( "Replace Object" );
  m_Menu->addSeparator();
  m_SaveObjectAction = m_Menu->addAction( "Save Object");
  m_LoadObjectAction = m_Menu->addAction( "Load Object");

  m_TypeAddress = new QLineEdit(this);
  m_TypeAddress->setReadOnly(true);
  button_edit = new QPushButton( "Edit", this );
  button_edit->setIcon(QIcon(":/foundation/Images/Buttons/ArrowDropDownSmall.png"));
  QPushButton * button_viewobj = new QPushButton( "", this );
  button_viewobj->setIcon(QIcon(":/foundation/Images/Buttons/DotDotDot.png"));
  QHBoxLayout * layout = new QHBoxLayout( this );
  layout->addWidget( m_TypeAddress );
  layout->addWidget( button_edit );
  layout->addWidget( button_viewobj );
  layout->setMargin(0);
  layout->setSpacing(1);
  button_edit->setMaximumWidth( 38 );
  button_viewobj->setMaximumWidth( 20 );
  button_edit->setMaximumHeight( 22 );
  button_viewobj->setMaximumHeight( 22 );

  connect( button_edit, SIGNAL(clicked()), SLOT(sltEditButton()));
  connect( button_viewobj, SIGNAL(clicked()), SLOT(sltViewObjectButton()));
  setMinimumHeight(36);
  setMaximumHeight(36);

  m_ActMetaClass = NULL;
  m_EditorWidget = NULL;
}

ObjectElementEditor::~ObjectElementEditor(void)
{
}

void ObjectElementEditor::setup(Object *&obj, const MetaClass *baseClass, bool readOnly)
{
  m_TypeAddress->setCursorPosition(0);
  m_ReadOnly = readOnly;
  m_IsOwner = true;
  m_ObjectPointer = &obj;
  m_ClassSelectionDialog->init( baseClass );
  m_BaseClass = (MetaClass*)baseClass;
  updateEditor();
}

void ObjectElementEditor::setup(Object *obj, pointer_diff offset, ClassProperty *prop, bool readOnly)
{
   ObjectClassProperty *objProp = dynamic_cast<ObjectClassProperty*>(prop);
  m_TypeAddress->setCursorPosition(0);
  m_ReadOnly = readOnly;
  m_Property = (ObjectClassProperty*)prop;
  m_IsOwner = m_Property->getObjectIsOwnerOfSubobject();
  m_Object = obj;
  m_ClassSelectionDialog->init( m_Property->getBaseClass() );
  setOffset( offset );
  updateEditor();
}

void ObjectElementEditor::sltEditButton()
{
   STD_GLOBAL_EXCEPTION_HANDLER_START
  updateMenu();
  m_Menu->exec(button_edit->mapToGlobal(QPoint(0, button_edit->height())));
  STD_GLOBAL_EXCEPTION_HANDLER_END
}

void ObjectElementEditor::sltViewObjectButton()
{
   STD_GLOBAL_EXCEPTION_HANDLER_START
  Object * obj = getObject();
  if( obj )
    openEditor( (MetaClass *)(obj)->getInstanceClass()->getGUIDescriptor()->getDefaultClassEditor() );
   STD_GLOBAL_EXCEPTION_HANDLER_END
}

void ObjectElementEditor::sltHandleMenu( QAction * action )
{
   STD_GLOBAL_EXCEPTION_HANDLER_START
  if( action == m_ReplaceObjectAction )
  {
    //m_ClassSelectionDialog->move( QCursor::pos());
    m_ClassSelectionDialog->autoMove(button_edit);
    if( m_ClassSelectionDialog->exec() == QDialog::Accepted )
    {
      MetaClass * mc = m_ClassSelectionDialog->getSelectedClass();
      Object * obj = getObject();
      Object * objToDelete = NULL;
      if( m_IsOwner )
      {
        objToDelete = obj;
      }
      setObject( mc->newInstance() );
      delete objToDelete;
      updateEditor();
    }
  }

  if( action == m_SaveObjectAction )
  {
    if( getObject() == NULL )
      return;

    string filename = FileDialogs::getSaveFileName(this, "save object", "*.object" );

    if( filename != "" )
    {
      Object * obj = getObject();
      BinaryOutputObjectStream out( filename.c_str() );
      out.writeObject( (Persistent *)obj );
    }
  }

  if( action == m_LoadObjectAction )
  {
    string filename = FileDialogs::getOpenFileName(this, "load object", "*.object" );
    if( filename != "" )
    {
      Object * obj = getObject();
      BinaryInputObjectStream in(filename.c_str());
      Persistent *instance = NULL;
      in.readObject(instance);
      Object *objToDelete = NULL;
      if (m_IsOwner) {
        objToDelete = obj;
      }

      setObject( instance );

      delete objToDelete;
    }

  }

  if( m_EditorClasses.find( action ) != m_EditorClasses.end() )
  {
    openEditor( m_EditorClasses[ action ].mc );
  }
   STD_GLOBAL_EXCEPTION_HANDLER_END
}

void ObjectElementEditor::updateMenu()
{
  m_ReplaceObjectAction->setEnabled( !m_ReadOnly );
  m_SaveObjectAction->setEnabled( getObject() != NULL );
  m_LoadObjectAction->setEnabled( !m_ReadOnly );

  const MetaClass * mc = NULL;
  Object* obj = getObject();
  if( obj != NULL )
    mc = obj->getInstanceClass();

  m_EditorsMenu->setEnabled( obj != NULL );

  if( m_ActMetaClass != mc )
  {
    m_ActMetaClass = (MetaClass *)mc;

    m_EditorsMenu->clear();
    m_EditorClasses.clear();

    const GUIDescriptor * gd = mc->getGUIDescriptor();
    m_EditorsMenu->setEnabled( gd->getNumClassEditors() > 0 );
    for( unsigned i=0;i<gd->getNumClassEditors();i++ )
    {
      MetaClass * mc = (MetaClass*)gd->getClassEditor(i);
      QAction * action = m_EditorsMenu->addAction( mc->getClassName().c_str() );
      TMetaInfo mi;
      mi.mc = mc;
      mi.obj = obj;
      mi.name = "";
      mi.isDefault = (i == 0);
      m_EditorClasses[ action ] = mi;
      if( mi.isDefault )
      {
        QFont font;
        font.setBold(true);
        action->setFont( font );
      }
    }
  }


}

/*Object ** ObjectElementEditor::getObjectPointer()
{
  if( m_ObjectPointer != NULL )
    return m_ObjectPointer;
  else
  {
    pAssert( m_Object != NULL );
    pAssert( !m_Property->getUseGetSetMethods() );
    return (Object**)((char*)m_Object + getOffset() + m_Property->getOffset());
  }
}*/

Object * ObjectElementEditor::getObject()
{
  if( m_ObjectPointer != NULL )
    return *m_ObjectPointer;
  else
  {
    return m_Property->getValue( m_Object, getOffset() );
  }
}

void ObjectElementEditor::setObject( Object * obj )
{
  if( m_ObjectPointer != NULL )
  {
    *m_ObjectPointer = obj;
  }
  else
  {
    m_Property->setValue( m_Object, getOffset(), obj );
  }
}

void ObjectElementEditor::openEditor( MetaClass * mc )
{
  Object * obj = getObject();
  pAssert( obj != NULL );
  delete m_EditorWidget;

  ClassEditor * ce = (ClassEditor*)mc->newInstance();
  ObjectBrowserWidget *newBrowser = new ObjectBrowserWidget(obj, ce, this);
  QWidget * w = ce->makeEditor( newBrowser, obj );
  newBrowser->setupEditor(w);
  //w->setWindowFlags( Qt::Tool );
  //w->setAttribute( Qt::WA_DeleteOnClose );
  connect( w, SIGNAL( destroyed( QObject* ) ), SLOT( sltDestroyEditorWidget( QObject* ) ) );
  QDesktopWidget desktop;
  //QPoint pos = QCursor::pos();
  //QWidget *parentw = dynamic_cast<QWidget*>(parent());
  //if (parentw) {
  //	parentw = dynamic_cast<QWidget*>(parentw->parent());
  //	if (parentw) {
  //		parentw = dynamic_cast<QWidget*>(parentw->parent());
  //		if (parentw) {
  //			pos = parentw->mapToGlobal(QPoint(25,15));
  //		}
  //	}
  //}
  QPoint pos = mapToGlobal(QPoint(0,height()));
  QPoint maxPos = QPoint(desktop.screen()->width() - max(w->width(), 525),desktop.screen()->height() - max(w->height(), 440) );
  maxPos = maxPos - this->pos();
  pos.setX( max(0, min(pos.x(), maxPos.x())) );
  pos.setY( max(0, min(pos.y(), maxPos.y())) );
  newBrowser->move( pos );
  newBrowser->show();
  newBrowser->show();
  m_EditorWidget = w;
  ObjectViewsTable::bindView( obj, ce );
}

void ObjectElementEditor::updateEditor()
{
  m_TypeAddress->setText(makeObjectDescription(getObject()).c_str());
  m_TypeAddress->setCursorPosition(0);
}

void ObjectElementEditor::destroy( bool dw , bool ds )
{
  delete m_EditorWidget;
  m_EditorWidget = NULL;
  QWidget::destroy( dw,ds );
}

void ObjectElementEditor::sltDestroyEditorWidget( QObject * obj )
{
   STD_GLOBAL_EXCEPTION_HANDLER_START
  m_EditorWidget = NULL;
  STD_GLOBAL_EXCEPTION_HANDLER_END
}

#ifndef CMAKE_INTDIR
#ifdef _WIN32
#include "moc/ObjectElementEditor_moc.cpp"
#else
#ifndef CMAKE_UNIX_INCLUDE_MOC
#include "moc_ObjectElementEditor.cpp"
#endif
#endif
#endif
