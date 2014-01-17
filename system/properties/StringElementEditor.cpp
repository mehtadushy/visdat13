//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "StringElementEditor.h"
//---------------------------------------------------------------------------
#include <QtGui/QLineEdit>
#include <QtGui/QBoxLayout>
#include "ClassProperty.h"
#include "StringHelper.h"
//---------------------------------------------------------------------------



StringElementEditor::StringElementEditor(QWidget * parent, Qt::WFlags f)
:ElementEditor( parent, f)
{
  m_Edit = new QLineEdit( "",this );
  connect( m_Edit, SIGNAL(textChanged(const QString&)), SLOT(sltTextChanged(const QString&)) );
  connect( m_Edit, SIGNAL(returnPressed()), SLOT(sltReturnPressed()) );
  QHBoxLayout * layout = new QHBoxLayout( this );
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_Edit);
  setMinimumHeight(30);
  setMaximumHeight(30);
}

StringElementEditor::~StringElementEditor(void)
{
}

void StringElementEditor::setup(Object *obj, pointer_diff offset, ClassProperty *prop, bool readOnly)
{
  m_Object = obj;
  m_Property = prop;
  setOffset( offset );
  m_Edit->setEnabled( !readOnly );
  updateEditor();
}

void StringElementEditor::updateEditor()
{
  string newStr = m_Property->toString(m_Object,getOffset());
  if (newStr != qString2STLString(m_Edit->text())) {
    m_Edit->setText( QString( newStr.c_str() ) );
  }
}

void StringElementEditor::sltTextChanged(const QString &text)
{
  if( ! text.isEmpty() )
  {
    m_Property->fromString( m_Object, getOffset(), text.toLatin1().constData() );
    modified();
  }
}

void StringElementEditor::sltReturnPressed()
{
  updateEditor();
}

#ifndef CMAKE_INTDIR
#ifdef _WIN32
#include "moc/StringElementEditor_moc.cpp"
#else
#ifndef CMAKE_UNIX_INCLUDE_MOC
#include "moc_StringElementEditor.cpp"
#endif
#endif
#endif
