#include "stdafx.h"
//---------------------------------------------------------------------------
#include "PropertyBrowser.h"
//---------------------------------------------------------------------------
#include "ClassProperty.h"
#include <QtGui/QSplitter>
#include <QtGui/QLabel>
#include <QtGui/QBoxLayout>
#include <QtGui/QSpacerItem>
#include <QtGui/QPushButton>
#include <QtGui/QPalette>
#include "PropertyEditor.h"
#include "ClassProperty.h"
#include "SeparatorClassProperty.h"
//---------------------------------------------------------------------------
#include <QtGui/QApplication>
//---------------------------------------------------------------------------



IMPLEMENT_GEOX_CLASS( PropertyBrowser, 0 )
{
}

PropertyBrowser::PropertyBrowser()
{
  m_Widget = NULL;
}

void PropertyBrowser::update()
{
  m_Widget->update();
}

QWidget* PropertyBrowser::makeEditor(QWidget * parent, Object * obj )
{
  m_Widget = new PropertyBrowserWidget( parent );
  m_Widget->resize( 240, 200 );
  m_Widget->setup( obj);
  return m_Widget;
}

card32 PropertyBrowser::getPreferredWidth()
{
  return m_Widget->width();
}

card32 PropertyBrowser::getPreferredHeight()
{
  return m_Widget->height();
}

PropertyBrowserWidget::PropertyBrowserWidget( QWidget * parent )
:QScrollArea( parent )
{
  //setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  //setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  setWidgetResizable(true);
  setFrameStyle(0);
  m_Height = 100;
  resize( 240, 200 );

  // add empty layout to fix funny qt resize bug
  QVBoxLayout* tl=new QVBoxLayout;
  tl->setMargin(0);
  tl->setSpacing(0);
  setLayout(tl);
}

static int alt(int x) {
   //x = (int)(1.06*x) + 5;
   x = int(0.5f*float(x) + 0.5f*255.0f);
   if (x<0) x = 0;
   if (x>255) x = 255;
   return x;
}

static int alt2(int x) {
   //x = (int)(0.94*x) - 5;
   x = int(0.1f*float(x) + 0.9f*255.0f);
   if (x<0) x = 0;
   if (x>255) x = 255;
   return x;
}

static int dark(int x) {
   x = (int)(0.3*x);
   if (x>255) x = 255;
   return x;
}

void PropertyBrowserWidget::setup(Object * obj)
{
  pAssert( m_Entries.size() == 0 );

  m_Height = 0;
  const MetaClass * mc = obj->getInstanceClass();

  QPalette pal[3];
  pal[0] = palette();
  pal[1] = palette();
  pal[2] = palette();

  QPalette stdPal = qApp->palette();
  QColor col = stdPal.button().color();
  QColor col1 = QColor(alt(col.red()), alt(col.green()), alt(col.blue()), col.alpha());
  QColor col2 = QColor(alt2(col.red()), alt2(col.green()), alt2(col.blue()), col.alpha());
  QColor col3 = QColor(dark(col.red()), dark(col.green()), dark(col.blue()), col.alpha());
  pal[0].setColor(QPalette::Window, col1 );
  pal[1].setColor(QPalette::Window, col2 );
  pal[2].setColor(QPalette::Window, col3 );
  pal[2].setColor(QPalette::WindowText, QColor(255,255,255,255) );

  //pal[0].setColor(QPalette::Window, QColor( 212,208,200 ) );
  //pal[1].setColor(QPalette::Window, QColor( 232,232,232 ) );

  m_MainWidget = new QWidget( viewport() );
  QPalette main_pal;
  QColor bg = Qt::darkGray;
  main_pal.setColor( QPalette::Window, bg );
  //m_MainWidget->setPalette( main_pal );
  //m_MainWidget->setAutoFillBackground(true);

  QPalette splitter_pal;
//	splitter_pal.setColor( QPalette::Light, bg );
//	splitter_pal.setColor( QPalette::Dark, bg );

  QSplitter * splitter = new QSplitter( m_MainWidget );
//	splitter->setPalette( splitter_pal );
  splitter->setHandleWidth( 6 );
  setWidget( m_MainWidget );
  splitter->setOrientation( Qt::Horizontal );
  QVBoxLayout * layout = new QVBoxLayout;
  layout->addWidget( splitter );
  layout->setMargin(0);
  m_MainWidget->setLayout( layout );

  QWidget * labelWidget = new QWidget( splitter );
  QVBoxLayout * lwLayout = new QVBoxLayout;
  lwLayout->setMargin(2);
  lwLayout->setSpacing(0);
  labelWidget->setLayout( lwLayout );
  QWidget * editorWidget = new QWidget( splitter );
  QVBoxLayout * ewLayout = new QVBoxLayout;
  ewLayout->setMargin(2);
  ewLayout->setSpacing(0);
  editorWidget->setLayout( ewLayout );

  splitter->setStretchFactor(0,0);
  splitter->setStretchFactor(1,1);

  int labelWidth = 20;
  int colorCycle = 0;
  for( unsigned i=0;i<mc->getNumProperties();i++ )
  {
    ClassProperty * prop = (ClassProperty *)mc->getProperty(i);
    if( !prop->getUserEdit() )
      continue;

    QWidget * ew = new QWidget( editorWidget );
    card32 height = 20;
    const MetaClass *edClass = prop->getDefaultPropertyEditor();
    PropertyEditor *propEd = NULL;
    if( edClass )
    {
      propEd = (PropertyEditor *)edClass->newInstance();
      propEd->setup( obj, NULL, prop );
      QWidget * edWidget = propEd->makeEditor( ew, prop->getEditReadonly() );
      connect( edWidget, SIGNAL( sigModified() ), SLOT(sltModified()));
      QHBoxLayout * layout = new QHBoxLayout( ew );
      layout->addWidget( edWidget );
      layout->setSpacing(0);
      layout->setMargin(3);

      height = propEd->getPreferredHeight() + layout->margin();
    }

    QWidget * lw = new QWidget( labelWidget );
    lw->setMinimumHeight( height );
    lw->setMaximumHeight( height );
    lw->setMinimumWidth( 5 );
    lw->resize( labelWidget->width(), height );
    lw->setAutoFillBackground(true);
    lw->setPalette( pal[colorCycle%2] );
    lwLayout->addWidget( lw );
    QLabel * label = new QLabel( lw );
    label->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
    label->setText( QString(" ") + QString(prop->getCName().c_str()) );
    QFontMetrics fm(label->font());
    labelWidth = max( labelWidth, fm.width(prop->getCName().c_str()));
    label->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    label->setToolTip( QString("Type: ") + QString(prop->getTypeName().c_str()) );
    label->setMinimumWidth( 5 );
    QHBoxLayout * lw_layout = new QHBoxLayout( lw );
    lw_layout->setMargin(0);
    lw_layout->setSpacing(0);
    lw_layout->addWidget( label );

    ew->setMinimumHeight( height );
    ew->setMaximumHeight( height );
    ew->resize( labelWidget->width(), height );
    ew->setAutoFillBackground(true);
    ew->setPalette( pal[(colorCycle++)%2] );
    ewLayout->addWidget( ew );

    if (dynamic_cast<const SeparatorClassProperty*>(prop)) {
      QFont f = label->font();
      //f.setBold(true);
      label->setFont(f);
      lw->setPalette(pal[2]);
      ew->setPalette(palette());
      colorCycle = 1;
    }

    m_Entries.push_back( TEditEntry( lw, ew, propEd ) );

    m_Height += height;
  }

  lwLayout->addItem( new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding) );
  ewLayout->addItem( new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding) );

  labelWidth += 10;
  labelWidth = 100;
  labelWidth = min( width() - 10, labelWidth );


  QList<int> sizes;
  sizes << labelWidth << width() - labelWidth;
  splitter->setSizes( sizes );

//	resize( 240, m_Height + 10 );
}

void PropertyBrowserWidget::resizeEvent(QResizeEvent *e)
{
//	if( (int)m_Height > height()+3 )
//		m_MainWidget->resize( width() - 20, max((int)m_Height,height()-5) );
//	else
//		m_MainWidget->resize( width(), max((int)m_Height,height()-5) );
  QScrollArea::resizeEvent( e );
}

void PropertyBrowserWidget::sltModified()
{
  emit sigModified();
}

void PropertyBrowserWidget::update()
{
  for( unsigned i=0;i<m_Entries.size();i++ )
  {
    if (m_Entries[i].edit != NULL) {
      m_Entries[i].edit->update();
    }
  }
}

#ifndef CMAKE_INTDIR
#ifdef _WIN32
#include "moc/PropertyBrowser_moc.cpp"
#else
#ifndef CMAKE_UNIX_INCLUDE_MOC
#include "moc_PropertyBrowser.cpp"
#endif
#endif
#endif
