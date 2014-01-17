#include "stdafx.h"
//---------------------------------------------------------------------------
#include "VariableArrayElementEditor.h"
//---------------------------------------------------------------------------
#include "VariableArrayClassProperty.h"
#include <QtGui/QTreeWidget>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include "qtHelpers.h"
#include "ExceptionHandling.h"
#include "PropertyEditor.h"
//---------------------------------------------------------------------------



VariableArrayElementEditor::VariableArrayElementEditor(QWidget * parent, Qt::WFlags f)
:ElementEditor(parent,f)
{
  m_Property = NULL;
  m_Object = NULL;
  m_ActEditor = NULL;
  m_ActPropertyEditor = NULL;

  resize(100,200);

  m_Add = new QPushButton( "Add",this);
  m_Add->resize(50,24);
  connect( m_Add, SIGNAL(clicked()), SLOT(sltAdd()));
  m_Delete = new QPushButton( "Del",this );
  m_Delete->resize( 50,24 );
  connect( m_Delete, SIGNAL(clicked()), SLOT(sltDelete()));

  m_TreeView = new QTreeWidget(this);
  m_TreeView->setColumnCount(1);
  m_TreeView->setMaximumHeight( 100 );
  connect( m_TreeView, SIGNAL(itemClicked( QTreeWidgetItem *, int )), SLOT(sltItemActivated( QTreeWidgetItem *, int )));

  m_EditorParent = new QWidget(this);
  m_EditorParent->setMinimumHeight( 24 );
  QHBoxLayout * l3 = new QHBoxLayout(m_EditorParent);
  l3->setSpacing(0);
  l3->setMargin(0);

  QHBoxLayout * l1 = new QHBoxLayout();
  l1->addWidget( m_Add );
  l1->addWidget( m_Delete );
  l1->addStretch(1);
  l1->setMargin(3);
  l1->setSpacing(3);

  QVBoxLayout * l2 = new QVBoxLayout(this);
  l2->addWidget( m_TreeView );
  l2->addLayout( l1 );
  l2->addWidget( m_EditorParent );
  l2->setMargin(3);
  l2->setSpacing(3);

  m_ReadOnly = true;
}

void VariableArrayElementEditor::setup( Object * obj, pointer_diff offset, ClassProperty * prop, bool readOnly )
{
  m_Object = obj;
  m_Property = (VariableArrayClassProperty*)prop;
  setOffset( offset );
  m_ReadOnly = readOnly;
  updateEditor();
}

void VariableArrayElementEditor::updateEditor()
{
  updateView();
}

void VariableArrayElementEditor::sltAdd()
{
  m_Property->setSize( m_Object, m_Property->getNumElements(m_Object) + 1 );
  updateView();
}

void VariableArrayElementEditor::sltDelete()
{
  QTreeWidgetItem * item = m_TreeView->currentItem();
  if( item != NULL )
  {
    int index = m_TreeView->indexOfTopLevelItem( item );
    if( index >= 0 )
    {
      if( QTHelpers::question( this, QString("Delete item #") + QString::number(index) + " '" +
        m_Property->getObjectName(m_Object,index).c_str() + "' ?" ) )
      {
        delete m_ActEditor;
        m_Property->deleteElement(m_Object, index );
        updateView();
      }
    }
  }
}

void VariableArrayElementEditor::sltItemActivated( QTreeWidgetItem * item, int column )
{
  STD_GLOBAL_EXCEPTION_HANDLER_START
  if( item != NULL )
  {
    int index = m_TreeView-> indexOfTopLevelItem( item );
    if( index >= 0 )
    {
      openEditor( (MetaClass*)m_Property->getElementClassProperty()->getDefaultPropertyEditor(), index );
    }
  }
  STD_GLOBAL_EXCEPTION_HANDLER_END
}

void VariableArrayElementEditor::sltModified()
{
  updateView();
}

void VariableArrayElementEditor::updateView()
{
  m_TreeView->clear();
  for( card32 i=0;i<m_Property->getNumElements( m_Object );i++ )
  {
    string name = m_Property->getObjectName( m_Object, i );
    QTreeWidgetItem * item = new QTreeWidgetItem(m_TreeView);
    item->setText(0, name.c_str() );
  }
}

void VariableArrayElementEditor::openEditor( MetaClass * mc, card32 index )
{
  delete m_ActEditor;
  m_ActPropertyEditor = (PropertyEditor *)mc->newInstance();
  pointer_diff offset = getOffset() + m_Property->getDiff( m_Object, index );
  m_ActPropertyEditor->setup( m_Object, offset, m_Property->getElementClassProperty() );
  m_ActEditor = m_ActPropertyEditor->makeEditor( m_EditorParent, m_ReadOnly );
  m_EditorParent->layout()->addWidget( m_ActEditor );
  connect( m_ActEditor, SIGNAL( destroyed(QObject*)), SLOT(sltEditorDestroyed(QObject *)));
  connect( m_ActEditor, SIGNAL( sigModified()), SLOT(sltModified()));
}

void VariableArrayElementEditor::destroy(bool dw, bool ds)
{
  delete m_ActEditor;
  m_ActEditor = NULL;
  delete m_ActPropertyEditor;
  m_ActPropertyEditor = NULL;
}

void VariableArrayElementEditor::sltEditorDestroyed(QObject *obj)
{
  m_ActEditor = NULL;
  delete m_ActPropertyEditor;
  m_ActPropertyEditor = NULL;
}


#ifndef CMAKE_INTDIR
#ifdef _WIN32
#include "moc/VariableArrayElementEditor_moc.cpp"
#else
#ifndef CMAKE_UNIX_INCLUDE_MOC
#include "moc_VariableArrayElementEditor.cpp"
#endif
#endif
#endif
