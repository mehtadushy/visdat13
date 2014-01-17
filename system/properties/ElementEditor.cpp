//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ElementEditor.h"
//---------------------------------------------------------------------------



ElementEditor::ElementEditor(QWidget * parent, Qt::WFlags f)
:QWidget( parent, f )
{
  m_Offset = NULL;
}

ElementEditor::~ElementEditor(void)
{
}

void ElementEditor::modified()
{
  emit sigModified();
}

void ElementEditor::sltUpdate()
{
  updateEditor();
}

#ifndef CMAKE_INTDIR
#ifdef _WIN32
#include "moc/ELementEditor_moc.cpp"
#else
#ifndef CMAKE_UNIX_INCLUDE_MOC
#include "moc_ElementEditor.cpp"
#endif
#endif
#endif
