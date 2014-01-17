//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ObjectListElementEditor.h"
//---------------------------------------------------------------------------
#include "ObjectListProperty.h"
#include <QtGui/QPushButton>
#include <QtGui/QTreeWidget>
#include <QtGui/QLabel>
#include <QtGui/QMessageBox>
#include "qtHelpers.h"
#include "ClassSelectionDialog.h"
#include "ClassEditor.h"
#include "ObjectViewsTable.h"
#include "ClassCreator.h"
#include "MemberBrowser.h"
//---------------------------------------------------------------------------
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QInputDialog>
//---------------------------------------------------------------------------


void olee_setButtonIcon( QPushButton * button, QString filename )
{
  QPixmap * pixmap = QTHelpers::createPixmap( filename );
  if( pixmap )
  {
    QLabel * iconLabel = new QLabel(button);
    iconLabel->move(3,3);
    iconLabel->setPixmap( *pixmap );
  }
}

ObjectListElementEditor::ObjectListElementEditor(QWidget * parent,Qt::WFlags f)
:ElementEditor(parent,f)
{
  resize( 200, 150 );
  m_Object = NULL;
  m_ObjectList = NULL;
  m_SubObjectWidget = NULL;

  m_TreeView = new QTreeWidget( this );
  m_TreeView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  m_Add = new QPushButton( "Add", this );
  m_AddAddr = new QPushButton( "Add Addr.", this );
  m_Delete = new QPushButton( "Del", this );
  m_ToTop = new QPushButton( "",this);
  m_ToBottom = new QPushButton( "",this);
  m_Up = new QPushButton( "",this);
  m_Down = new QPushButton( "",this);
  m_EditMember = new QPushButton( "Edit", this );
  m_Edit = new QPushButton( "...", this );

  m_NumElementsLabel = new QLabel(this);

  m_TreeView->resize(100,100);
  m_TreeView->setColumnCount(2);
  m_TreeView->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
  QStringList labels;
  labels << "Name" << "Type";
  m_TreeView->setHeaderLabels( labels );

  m_Add->setMaximumWidth( 35 );
  m_Delete->setMaximumWidth( 35 );
  m_ToTop->setMaximumWidth( 20 );
  m_ToBottom->setMaximumWidth( 20 );
  m_Up->setMaximumWidth( 20 );
  m_Down->setMaximumWidth( 20 );
  m_Edit->setMaximumWidth( 35 );
  m_EditMember->setMaximumWidth( 40 );

  olee_setButtonIcon( m_ToTop, ":/foundation/Images/Buttons/up_top_arrow.bmp" );
  olee_setButtonIcon( m_ToBottom, ":/foundation/Images/Buttons/down_bottom_arrow.bmp" );
  olee_setButtonIcon( m_Up, ":/foundation/Images/Buttons/up_arrow.bmp" );
  olee_setButtonIcon( m_Down, ":/foundation/Images/Buttons/down_arrow.bmp" );

  QVBoxLayout * l1 = new QVBoxLayout();
  l1->addWidget( m_ToTop );
  l1->addWidget( m_Up );
  l1->addWidget( m_Down );
  l1->addWidget( m_ToBottom );
  l1->addStretch(1);
  l1->setSpacing(3);
  l1->setMargin(0);

  QHBoxLayout * l2 = new QHBoxLayout();
  l2->addWidget( m_Add );
  l2->addWidget( m_AddAddr );
  l2->addWidget( m_Delete );
  l2->addWidget( m_EditMember );
  l2->addWidget( m_Edit );
  l2->addStretch(1);
  l2->setSpacing(3);
  l2->setMargin(0);
  l2->addWidget( m_NumElementsLabel );

  QHBoxLayout * l3 = new QHBoxLayout();
  l3->addWidget( m_TreeView );
  l3->addLayout( l1 );
  l3->setSpacing(3);
  l3->setMargin(0);

  QVBoxLayout * l4 = new QVBoxLayout(this);
  l4->addLayout( l3 );
  l4->addLayout( l2 );

  l4->setMargin(3);
  l4->setSpacing(3);

  connect( m_Add, SIGNAL(clicked()), SLOT( sltAdd()));
  connect( m_AddAddr, SIGNAL(clicked()), SLOT( sltAddAddr()));
  connect( m_Delete, SIGNAL(clicked()), SLOT( sltDelete()));
  connect( m_Edit, SIGNAL(clicked()), SLOT( sltEdit()));
  connect( m_EditMember, SIGNAL(clicked()), SLOT( sltEditMember()));
  connect( m_ToTop, SIGNAL(clicked()), SLOT( sltMoveToTop()));
  connect( m_ToBottom, SIGNAL(clicked()), SLOT( sltMoveToBottom()));
  connect( m_Up, SIGNAL(clicked()), SLOT( sltMoveUp()));
  connect( m_Down, SIGNAL(clicked()), SLOT( sltMoveDown()));
  connect( m_TreeView, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), SLOT(sltItemDoubleClicked(QTreeWidgetItem *, int)));

  m_ClassSelectionDialog = new ClassSelectionDialog(this);
}

void ObjectListElementEditor::setup(Object *obj, pointer_diff offset, ClassProperty *prop, bool readOnly)
{
  m_Object = obj;
  m_ObjectList = (ObjectListProperty *)prop;
  m_ClassSelectionDialog->init( m_ObjectList->getObjectBaseClass());
  setOffset( offset );
  updateTreeView();
}

void ObjectListElementEditor::sltAdd()
{
  QPoint pos = m_Add->mapToGlobal(QPoint(0,m_Add->height()));
  m_ClassSelectionDialog->move(pos);
  if( m_ClassSelectionDialog->exec() == QDialog::Accepted )
  {
    const MetaClass * mc = m_ClassSelectionDialog->getSelectedClass();
    const MetaClass * ccmc = mc->getGUIDescriptor()->getDefaultClassCreator();
    Object * obj = NULL;
    if (ccmc == NULL) {
      obj = mc->newInstance();
    } else {
      ClassCreator *cc = (ClassCreator*)ccmc->newInstance();
      obj = ClassCreator::interactivelyCreateObject(cc); // result may be NULL
      delete cc;
    }
    if (obj != NULL) {
      m_ObjectList->addObject( m_Object, obj );
      updateTreeView();
    }
  }
}

void ObjectListElementEditor::sltAddAddr() {
  bool ok = false;
  QString text = QInputDialog::getText (this, "Enter Address of an Object to be added...",
    "Address (0x values for hex): [please use this function with care, the input is not validated]", QLineEdit::Normal, "0x00000000", &ok);
  if (ok) {
    mpcard addr = text.toInt(&ok, 0);
    if (!ok) {
      QMessageBox::warning(this, QString("cannot convert"), QString("Cannot parse address string - abortet."));
    } else {
      m_ObjectList->addObject( m_Object, (Object*)addr );
      updateTreeView();
    }
  }
}
void ObjectListElementEditor::sltDelete()
{
  if( m_TreeView->currentItem() != NULL )
  {
    ObjectListItem * item = (ObjectListItem*)m_TreeView->currentItem();
    if( QMessageBox::question(this, "", QString("Do you want to delete \'") + item->text(0) + "\' ?",
      QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes )
    {
      delete m_SubObjectWidget;
      m_SubObjectWidget = NULL;
      m_ObjectList->deleteObjects( m_Object, item->getIndex(),item->getIndex()+1 );
      updateTreeView();
    }
  }
}

void ObjectListElementEditor::sltEdit()
{
  if( m_TreeView->currentItem() != NULL )
  {
    ObjectListItem * item = (ObjectListItem*)m_TreeView->currentItem();
    openEditor( item->getObject(), item->getObject()->getInstanceClass()->getGUIDescriptor()->getDefaultClassEditor() );
  }
}


void ObjectListElementEditor::sltEditMember()
{
  if( m_TreeView->currentItem() != NULL )
  {
    ObjectListItem * item = (ObjectListItem*)m_TreeView->currentItem();
    const GUIDescriptor * guiDesc = item->getObject()->getInstanceClass()->getGUIDescriptor();
    for( card32 i=0;i<guiDesc->getNumClassEditors();i++ )
      if( guiDesc->getClassEditor(i) == MemberBrowser::getClass() )
      {
        openEditor( item->getObject(), guiDesc->getClassEditor(i));
        return;
      }
    openEditor( item->getObject(), guiDesc->getDefaultClassEditor() );
  }
}


void ObjectListElementEditor::openEditor( Object * obj, const MetaClass * mc )
{
  delete m_SubObjectWidget;
  ClassEditor * ce = (ClassEditor*)mc->newInstance();
  QWidget * w = ce->makeEditor( this, obj );
  w->setWindowFlags( Qt::Window );
  w->setAttribute( Qt::WA_DeleteOnClose );
  connect( w, SIGNAL( destroyed( QObject* ) ), SLOT( sltEditorDestroyed( QObject* ) ) );
  connect( w, SIGNAL( sigModified() ), SLOT(sltSubObjectModified()));
  w->show();
  w->move( QCursor::pos() );
  m_SubObjectWidget = w;
  ObjectViewsTable::bindView( obj, ce );

}

void ObjectListElementEditor::sltEditorDestroyed(QObject *obj)
{
  m_SubObjectWidget = NULL;
}

void ObjectListElementEditor::sltMoveDown()
{
  ObjectListItem * item = dynamic_cast<ObjectListItem*>( m_TreeView->currentItem() );
  if( item != NULL )
  {
    card32 index = item->getIndex();
    if( index + 1 < m_ObjectList->getNumObjects(m_Object) )
    {
      m_ObjectList->moveObject(m_Object,index, index+1);
      updateTreeView();
      QTreeWidgetItem * newItem = m_TreeView->topLevelItem( index+1 );
      if( newItem != NULL )
        m_TreeView->setCurrentItem( newItem );
    }
  }
}

void ObjectListElementEditor::sltMoveUp()
{
  ObjectListItem * item = dynamic_cast<ObjectListItem*>( m_TreeView->currentItem() );
  if( item != NULL )
  {
    int index = item->getIndex();
    if( index  > 0 )
    {
      m_ObjectList->moveObject(m_Object,index, index-1);
      updateTreeView();
      QTreeWidgetItem * newItem = m_TreeView->topLevelItem( index-1 );
      if( newItem != NULL )
        m_TreeView->setCurrentItem( newItem );
    }
  }
  updateTreeView();
}

void ObjectListElementEditor::sltMoveToTop()
{
  ObjectListItem * item = dynamic_cast<ObjectListItem*>( m_TreeView->currentItem() );
  if( item != NULL )
  {
    int index = item->getIndex();
    m_ObjectList->moveObject(m_Object,index, m_ObjectList->getNumObjects(m_Object)-1);
    updateTreeView();
    QTreeWidgetItem * newItem = m_TreeView->topLevelItem( m_ObjectList->getNumObjects(m_Object)-1 );
    if( newItem != NULL )
      m_TreeView->setCurrentItem( newItem );
  }
  updateTreeView();
}

void ObjectListElementEditor::sltMoveToBottom()
{
  ObjectListItem * item = dynamic_cast<ObjectListItem*>( m_TreeView->currentItem() );
  if( item != NULL )
  {
    int index = item->getIndex();
    m_ObjectList->moveObject(m_Object,index, 0);
    updateTreeView();
    QTreeWidgetItem * newItem = m_TreeView->topLevelItem( 0 );
    if( newItem != NULL )
      m_TreeView->setCurrentItem( newItem );
  }
  updateTreeView();
}

void ObjectListElementEditor::destroy(bool dw, bool ds)
{
  delete m_SubObjectWidget;
  m_SubObjectWidget = NULL;
}

void ObjectListElementEditor::updateTreeView()
{
  m_TreeView->clear();
  for( card32 i=0;i<m_ObjectList->getNumObjects(m_Object);i++ )
  {
    string objectName = m_ObjectList->getObjectName(m_Object,i);
    Object * obj = m_ObjectList->getObject(m_Object,i);
    ObjectListItem * item = new ObjectListItem(m_TreeView, obj,i );
    item->setText(0, objectName.c_str() );
    item->setText(1, obj->getInstanceClass()->getClassName().c_str());
  }
  m_NumElementsLabel->setText( QString::number(m_ObjectList->getNumObjects(m_Object)) + " Elements" );
  update();
}

void ObjectListElementEditor::sltItemDoubleClicked(QTreeWidgetItem *item, int column)
{
  if( item != NULL )
  {
    ObjectListItem * tmpItem = (ObjectListItem*)item;
    openEditor( tmpItem->getObject(), tmpItem->getObject()->getInstanceClass()->getGUIDescriptor()->getDefaultClassEditor() );
  }
}

void ObjectListElementEditor::sltSubObjectModified()
{
  updateTreeView();
}

#ifndef CMAKE_INTDIR
#ifdef _WIN32
#include "moc/ObjectListElementEditor_moc.cpp"
#else
#ifndef CMAKE_UNIX_INCLUDE_MOC
#include "moc_ObjectListElementEditor.cpp"
#endif
#endif
#endif
