//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "EnumerationElementEditor.h"
//---------------------------------------------------------------------------
#include <QtGui/QComboBox>
#include <QtGui/QBoxLayout>
#include "ClassProperty.h"
#include "StringHelper.h"
#include "NumericalClassProperty.h"
//---------------------------------------------------------------------------



EnumerationElementEditor::EnumerationElementEditor(QWidget * parent, Qt::WFlags f)
:ElementEditor( parent, f)
{
  m_Edit = new QComboBox(this);
  connect( m_Edit, SIGNAL(currentIndexChanged(int)), SLOT(sltIndexChanged(int)) );
  QHBoxLayout * layout = new QHBoxLayout( this );
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_Edit);
  setMinimumHeight(30);
  setMaximumHeight(30);
  m_Edit->setMinimumWidth(50);
  m_internalChange = false;
}

void EnumerationElementEditor::setup(Object *obj, pointer_diff offset, ClassProperty *prop, bool readOnly)
{
  m_Object = obj;
  m_Property = dynamic_cast<Card32ClassProperty*>(prop);
  pAssert(m_Property);
  setOffset( offset );
  m_Edit->setEnabled( !readOnly );
  card32 n = m_Property->getNumEnumItems();
  m_internalChange = true;
  for (card32 i=0; i<n; i++) {
    m_Edit->addItem(m_Property->publicGetEnumDescription(i).c_str());
  }
  m_internalChange = false;
  updateEditor();
}

void EnumerationElementEditor::updateEditor()
{
  card32 i = m_Property->getValue(m_Object, getOffset());
  m_Edit->setCurrentIndex(i);
}

void EnumerationElementEditor::sltIndexChanged(int i)
{
  if (!m_internalChange) {
    m_Property->setValue(m_Object, getOffset(), i);
  }
}

#ifndef CMAKE_INTDIR
#ifdef _WIN32
#include "moc/EnumerationElementEditor_moc.cpp"
#else
#ifndef CMAKE_UNIX_INCLUDE_MOC
#include "moc_EnumerationElementEditor.cpp"
#endif
#endif
#endif
