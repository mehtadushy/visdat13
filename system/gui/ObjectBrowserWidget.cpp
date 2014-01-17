//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ObjectBrowserWidget.h"
//---------------------------------------------------------------------------
#include "Object.h"
#include "Tools.h"
#include "BinaryObjectStreams.h"
#include "ClassEditor.h"
#include "ObjectViewsTable.h"
#include "ClassEditor.h"
#include "FileDialogs.h"
//---------------------------------------------------------------------------
#include <QtGui/QHBoxLayout>
#include <QtGui/QMenu>
#include <QtGui/QKeyEvent>
//---------------------------------------------------------------------------



ObjectBrowserWidget::ObjectBrowserWidget(Object *obj, ClassEditor *clEd, QWidget *parent, Qt::WFlags f) : QDialog(parent, f) {
   this->obj = obj;
   this->clEd = clEd;
   ui.setupUi(this);
   edLayout = new QHBoxLayout(ui.editorFrame);
   edLayout->setMargin(0);
   edLayout->setSpacing(0);
   ui.classDescriptionLabel->setText(makeObjectDescription(obj).c_str());
  setAttribute( Qt::WA_DeleteOnClose );

  m_Menu = new QMenu(this);

  if (obj == NULL) return;

  m_Menu->hide();
  connect( m_Menu, SIGNAL(triggered(QAction*)), SLOT(sltHandleMenu(QAction*)));
  m_EditorsMenu = m_Menu->addMenu( "Change Editor" );
  m_Menu->addSeparator();
  m_SaveObjectAction = m_Menu->addAction( "Save Object...");
  m_Menu->addSeparator();
  m_UpdateAction = m_Menu->addAction("Update");
  m_UpdateAction->setShortcut(QKeySequence("F5"));
  QObject::connect(m_UpdateAction, SIGNAL(triggered(bool)), this, SLOT(sltUpdate(bool)));

   const MetaClass *mc = obj->getInstanceClass();

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
   setWindowTitle("Object Browser");
   setWindowFlags(Qt::Window);
}

QWidget *ObjectBrowserWidget::getEditorContainer() {
   return ui.editorFrame;
}

void ObjectBrowserWidget::setupEditor(QWidget *ed) {
   edLayout->addWidget(ed);
   edWidget = ed;
}

void ObjectBrowserWidget::sltHandleMenu(QAction *action) {
  if( action == m_SaveObjectAction )
  {
    if( obj == NULL )
      return;

    string filename = FileDialogs::getSaveFileName(this, "save object", "*.object" );
    if( filename != "" )
    {
      BinaryOutputObjectStream out( filename.c_str() );
      out.writeObject( (Persistent *)obj );
    }
  }

  if( m_EditorClasses.find( action ) != m_EditorClasses.end() )
  {
      if (obj == NULL) return;
      delete edWidget;
      edWidget = NULL;

    const MetaClass *mc = m_EditorClasses[ action ].mc;
     clEd = (ClassEditor*)mc->newInstance();
    edWidget = clEd->makeEditor( ui.editorFrame, obj );
      edLayout->addWidget(edWidget);
     //connect( edWidget, SIGNAL( destroyed( QObject* ) ), SLOT( sltDestroyEditorWidget( QObject* ) ) );

     ObjectViewsTable::bindView( obj, clEd );
  }

}

void ObjectBrowserWidget::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_F5) sltUpdate(false);
}

void ObjectBrowserWidget::sltUpdate(bool x) {
  clEd->update();
}


void ObjectBrowserWidget::on_editButton_clicked()
{
  m_Menu->exec(ui.editButton->mapToGlobal(QPoint(0, ui.editButton->height())));
}


#ifndef CMAKE_INTDIR
#ifdef _WIN32
#include "moc/ObjectBrowserWidget_moc.cpp"
#else
#ifndef CMAKE_UNIX_INCLUDE_MOC
#include "moc_ObjectBrowserWidget.cpp"
#endif
#endif
#endif
