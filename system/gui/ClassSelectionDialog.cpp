//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ClassSelectionDialog.h"
//---------------------------------------------------------------------------
#include <QtGui/QTreeWidget>
#include <QtGui/QBoxLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QIcon>
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
//---------------------------------------------------------------------------
#include "MetaClass.h"
//---------------------------------------------------------------------------



ClassSelectionDialog::ClassSelectionDialog(QWidget * parent, Qt::WFlags f)
:QDialog(parent, f )
{
  setWindowTitle( "class selection" );

  // init tree
  m_Tree = new QTreeWidget( this );
  m_Tree->setColumnCount(1);
  m_Tree->setRootIsDecorated(true);
  m_Tree->setHeaderLabels( QStringList("classname") );
  connect( m_Tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), SLOT(sltDoubleClicked(QTreeWidgetItem *, int)));

  // init type in
  m_DirectTypeIn = new QLineEdit(this);
  connect( m_DirectTypeIn, SIGNAL( textChanged(const QString&) ), SLOT(sltDirectTypeInChanged(const QString &)));
  connect( m_DirectTypeIn, SIGNAL( returnPressed()), SLOT(sltEnterPressed()));

  // setup layout
  QVBoxLayout * layout = new QVBoxLayout;
  layout->addWidget( m_Tree );
  layout->addWidget( m_DirectTypeIn );
  setLayout( layout );

  // load icons
  m_ClassIcon = new QIcon( ":/foundation/Images/Classes/small_class.bmp" );
  m_AbstractClassIcon = new QIcon( ":/foundation/Images/Classes/small_class_abstract.bmp" );

  m_AllowAbstractClasses = false;
  m_DirectTypeIn->setFocus();

  resize(220, 250);
}

ClassSelectionDialog::~ClassSelectionDialog(void)
{
  delete m_ClassIcon;
  delete m_AbstractClassIcon;
}

void ClassSelectionDialog::init(const MetaClass * baseClass, bool allowAbstractClasses)
{
  m_AllowAbstractClasses = allowAbstractClasses;

  const MetaClass * mc = baseClass;
  pAssert( mc != NULL );

  QTreeWidgetItem * root = new QTreeWidgetItem( m_Tree, QStringList( baseClass->getClassName().c_str() ) );
  root->setIcon( 0, mc->isAbstract() ? *m_AbstractClassIcon : *m_ClassIcon );
  collectClasses( baseClass->getClassName(), root );
}

void ClassSelectionDialog::collectClasses( string baseClass, QTreeWidgetItem * parent )
{
  const MetaClass * mc = MetaClass::getClassByName( baseClass );
  parent->setIcon( 0, mc->isAbstract() ? *m_AbstractClassIcon : *m_ClassIcon );
  for( card32 i=0;i<mc->getNumDescendants();i++ )
  {
    string className = mc->getDescendant(i)->getClassName();
    QTreeWidgetItem * item = new QTreeWidgetItem( parent, QStringList( className.c_str() ) );
    collectClasses( className, item );
  }
}

void ClassSelectionDialog::sltDirectTypeInChanged( const QString &s)
{
  QList<QTreeWidgetItem *> items = m_Tree->findItems( s, Qt::MatchContains | Qt::MatchRecursive );
  if( items.size() > 0 )
    m_Tree->setCurrentItem( items.first() );
}

void ClassSelectionDialog::sltDoubleClicked(QTreeWidgetItem *item, int column)
{
  tryToFinishDialog( item->text(0).toLatin1().constData() );
}

void ClassSelectionDialog::sltEnterPressed()
{
  QTreeWidgetItem * item = m_Tree->currentItem();
  if( item != NULL )
  {
    tryToFinishDialog( item->text(0).toLatin1().constData() );
  }
}

void ClassSelectionDialog::showEvent(QShowEvent *e)
{
  m_SelectedClass = NULL;
}

void ClassSelectionDialog::tryToFinishDialog( string selectedClass )
{
  const MetaClass * mc = MetaClass::getClassByName( selectedClass );
  pAssert( mc != NULL );
  if( mc->isAbstract() && !m_AllowAbstractClasses )
  {
    QMessageBox::information( this, "Error", "Abstract classes are not allowed!" );
    return;
  }
  m_SelectedClass = (MetaClass*)mc;
  accept();
}

void ClassSelectionDialog::autoMove(QWidget *belowThis) {
   QPoint p = belowThis->mapToGlobal(QPoint(0, belowThis->height()));
   moveOnScreen(p.x(), p.y());
}

void ClassSelectionDialog::moveOnScreen(int x, int y) {
   QRect screen = QApplication::desktop()->availableGeometry(this);
   int w = width();
   int h = height();
   if (x>screen.width()-w) x = screen.width()-w;
   if (y>screen.height()-h) y = screen.height()-h;
   if (x<0) x=0;
   if (y<0) y=0;
  move(x,y);
}

#ifndef CMAKE_INTDIR
#ifdef _WIN32
#include "moc/ClassSelectionDialog_moc.cpp"
#else
#ifndef CMAKE_UNIX_INCLUDE_MOC
#include "moc_ClassSelectionDialog.cpp"
#endif
#endif
#endif
