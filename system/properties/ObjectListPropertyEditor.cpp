//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ObjectListPropertyEditor.h"
//---------------------------------------------------------------------------
#include "ObjectListElementEditor.h"
//---------------------------------------------------------------------------



IMPLEMENT_GEOX_CLASS( ObjectListPropertyEditor ,0)
{
}


ObjectListPropertyEditor::ObjectListPropertyEditor(void)
{
	m_ElementEditor = NULL;
}

QWidget * ObjectListPropertyEditor::makeEditor(QWidget *parent, bool readOnly)
{
	pAssert( m_Object != NULL );
	pAssert( m_Property != NULL );
	pAssert( m_ElementEditor == NULL );

	m_ElementEditor = new ObjectListElementEditor( parent );
	m_ElementEditor->setup( m_Object, m_Offset, m_Property, readOnly );

	return m_ElementEditor;
}

void ObjectListPropertyEditor::update()
{
	pAssert( m_ElementEditor != NULL );
	m_ElementEditor->sltUpdate();
}

card32 ObjectListPropertyEditor::getPreferredHeight()
{
	pAssert( m_ElementEditor != NULL );
	return m_ElementEditor->height();
}