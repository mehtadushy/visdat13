//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "MemberBrowser.h"
//---------------------------------------------------------------------------
#include "PropertyBrowser.h"
#include "MethodBrowser.h"
#include "ClassMethods.h"
#include "MetaClass.h"
//---------------------------------------------------------------------------


MetaClass* MemberBrowser::MemberBrowserMetaClass = NULL;

const MetaClass* MemberBrowser::getInstanceClass() const {
   return getClass();
}

Object* MemberBrowser::newInstance() {
   return new MemberBrowser;
}

const MetaClass* MemberBrowser::getClass() {
   if (MemberBrowserMetaClass==NULL) {
  MemberBrowserMetaClass = new MetaClass( "MemberBrowser" , "no description.",
                                            newInstance, sizeof( MemberBrowser ));
   }
   return MemberBrowserMetaClass;
}

void MemberBrowser::init(const MetaClass *parentClass) {
   ((MetaClass*)getClass())->init(parentClass, MetaClass::FIRST_VERSION + 0);
   MemberBrowserMetaClass->copyParentProperties();
}

void MemberBrowser::shutdown() {
   delete MemberBrowserMetaClass;
}

void MemberBrowser::update() {
  propBrowser->update();
  methodBrowser->update();
  m_Widget->updateEditor();
}

card32 MemberBrowser::getPreferredWidth() {return m_Widget->width();}
card32 MemberBrowser::getPreferredHeight()  {return m_Widget->height();}

QWidget * MemberBrowser::makeEditor( QWidget * parent, Object * obj ) {
  m_Widget = new MemberBrowserWidget( parent );
  m_Object = obj;

  propBrowser = new PropertyBrowser();
  methodBrowser = new MethodBrowser();

  m_Widget->setup( m_Object, propBrowser, methodBrowser );
  m_Widget->updateEditor();
  ObjectViewsTable::bindView(m_Object, this);
  return m_Widget;
}

Object * MemberBrowser::getObject() {return m_Object;}

bool MemberBrowser::standAloneEditorApplication() {return false;}

MemberBrowser::~MemberBrowser() {
  ObjectViewsTable::releaseView(m_Object, this);
}


// ---


MemberBrowserWidget::MemberBrowserWidget(QWidget * parent) : QSplitter(Qt::Vertical, parent)
{
  m_Object = NULL;
  setFrameStyle(0);
  setHandleWidth(6);
  //resize(200, 500);

}

MemberBrowserWidget::~MemberBrowserWidget(void) {}

void MemberBrowserWidget::setup(Object* obj, PropertyBrowser *propBrowser, MethodBrowser *methodBrowser)
{
  m_Object = obj;
  if (m_Object != NULL) {
    QWidget *propWidget = propBrowser->makeEditor(this, obj);
    propWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    addWidget(propWidget);
    if (m_Object->getInstanceClass()->getNumMethods() > 0) {
      QWidget *methodWidget = methodBrowser->makeEditor(this, obj);
      methodWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
      addWidget(methodWidget);
      setStretchFactor(0,1);
      setCollapsible(0,false);
      setStretchFactor(1,0);
      setCollapsible(1,false);
       //if (methodWidget->height() > 200) {
       //   QList<int> sizes;
       //   sizes.append(height()-200);
       //   sizes.append(200);
       //   setSizes(sizes);
       //}
    }
  }
}

void MemberBrowserWidget::updateEditor() {}  // done within subeditors

#ifndef CMAKE_INTDIR
#ifdef _WIN32
#include "moc/MemberBrowser_moc.cpp"
#else
#ifndef CMAKE_UNIX_INCLUDE_MOC
#include "moc_MemberBrowser.cpp"
#endif
#endif
#endif
