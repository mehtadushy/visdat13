//---------------------------------------------------------------------------
#include "stdafx.h"
#include "EmbeddedObjectElementEditor.h"
//---------------------------------------------------------------------------
#include "ObjectElementEditor.h"
//---------------------------------------------------------------------------
#include <QtGui/QMenu>
#include <QtGui/QPushButton>
#include <QtGui/QBoxLayout>
#include <QtGui/QDesktopWidget>
#include "EmbeddedObjectClassProperty.h"
#include "ClassProperty.h"
#include "MetaClass.h"
#include "ClassEditor.h"
#include "ObjectViewsTable.h"
#include "ExceptionHandling.h"
//---------------------------------------------------------------------------



EmbeddedObjectElementEditor::EmbeddedObjectElementEditor(QWidget * parent,Qt::WFlags f)
:ElementEditor(parent,f)
{
  m_Object = NULL;
  m_Property = NULL;
  m_ReadOnly = false;

  QPushButton * button_edit = new QPushButton( "...", this );
  QHBoxLayout * layout = new QHBoxLayout( this );
  layout->addWidget( button_edit );
  layout->addStretch( 20 );
  layout->setMargin(0);
  layout->setSpacing(3);

  connect( button_edit, SIGNAL(clicked()), SLOT(sltButton()));
  setMinimumHeight(36);
  setMaximumHeight(36);

  m_EditorWidget = NULL;
}

EmbeddedObjectElementEditor::~EmbeddedObjectElementEditor(void)
{
}

void EmbeddedObjectElementEditor::setup(Object *obj, pointer_diff offset, ClassProperty *prop, bool readOnly)
{
  m_ReadOnly = readOnly;
  m_Property = (EmbeddedObjectClassProperty*)prop;
  m_Object = obj;
  setOffset( offset );
}

void EmbeddedObjectElementEditor::sltButton()
{
   STD_GLOBAL_EXCEPTION_HANDLER_START
  Object * obj = getObject();
  if( obj )
    openEditor( (MetaClass *)obj->getInstanceClass()->getGUIDescriptor()->getDefaultClassEditor() );
   STD_GLOBAL_EXCEPTION_HANDLER_END
}



Object * EmbeddedObjectElementEditor::getObject()
{
  pAssert( m_Object != NULL );
  return (Object*)((char*)m_Object + getOffset() + m_Property->getOffset());
}

void EmbeddedObjectElementEditor::openEditor( MetaClass * mc )
{
  Object * obj = getObject();
  pAssert( obj != NULL );
  delete m_EditorWidget;

  ClassEditor * ce = (ClassEditor*)mc->newInstance();
  QWidget * w = ce->makeEditor( this, obj );
  w->setWindowFlags( Qt::Tool );
  w->setAttribute( Qt::WA_DeleteOnClose );
   connect( w, SIGNAL( destroyed( QObject* ) ), SLOT( sltDestroyEditorWidget( QObject* ) ) );
   QDesktopWidget desktop;
   QPoint pos = QCursor::pos();
   QPoint maxPos = QPoint(desktop.screen()->width() - w->width(),desktop.screen()->height() - w->height() );
   maxPos = maxPos - this->pos();
   pos.setX( min(pos.x(), maxPos.x()  ));
   pos.setY( min(pos.y(), maxPos.y()  ));
   w->move( pos );
   w->show();
  m_EditorWidget = w;
  ObjectViewsTable::bindView( obj, ce );
}

void EmbeddedObjectElementEditor::destroy( bool dw , bool ds )
{
  delete m_EditorWidget;
  m_EditorWidget = NULL;
  QWidget::destroy( dw,ds );
}

void EmbeddedObjectElementEditor::sltDestroyEditorWidget( QObject * obj )
{
  m_EditorWidget = NULL;
}



// ----



EmbeddedObjectInlineElementEditor::EmbeddedObjectInlineElementEditor(QWidget * parent,Qt::WFlags f)
:ElementEditor(parent,f)
{
  m_Object = NULL;
  m_Property = NULL;
  m_ReadOnly = false;
  ce = NULL;

  layout = new QHBoxLayout( this );
  layout->setMargin(3);
  layout->setSpacing(0);

  m_EditorWidget = NULL;
}

EmbeddedObjectInlineElementEditor::~EmbeddedObjectInlineElementEditor() {
   if (ce) {
    Object * obj = getObject();
    ObjectViewsTable::releaseView( obj, ce );
   }
}

void EmbeddedObjectInlineElementEditor::setup(Object *obj, pointer_diff offset, ClassProperty *prop, bool readOnly)
{
   STD_GLOBAL_EXCEPTION_HANDLER_START
  m_ReadOnly = readOnly;
  m_Property = (EmbeddedObjectClassProperty*)prop;
  m_Object = obj;
  setOffset( offset );
   openEditor((MetaClass *)obj->getInstanceClass()->getGUIDescriptor()->getDefaultClassEditor());
   STD_GLOBAL_EXCEPTION_HANDLER_END
}

Object * EmbeddedObjectInlineElementEditor::getObject()
{
  pAssert( m_Object != NULL );
  return (Object*)((char*)m_Object + getOffset() + m_Property->getOffset());
}

void EmbeddedObjectInlineElementEditor::openEditor( MetaClass * mc )
{
  Object * obj = getObject();
  pAssert( obj != NULL );
  delete m_EditorWidget;

  ce = (ClassEditor*)mc->newInstance();
  QWidget * w = ce->makeEditor( this, obj );
  layout->addWidget(w);
  m_EditorWidget = w;
  ObjectViewsTable::bindView( obj, ce );
  card32 h = w->sizeHint().height();
  setMinimumHeight(h+3);
  //setMaximumHeight(h);
}

#ifndef CMAKE_INTDIR
#ifdef _WIN32
#include "moc/EmbeddedObjectElementEditor_moc.cpp"
#else
#ifndef CMAKE_UNIX_INCLUDE_MOC
#include "moc_EmbeddedObjectElementEditor.cpp"
#endif
#endif
#endif
