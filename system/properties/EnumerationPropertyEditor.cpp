//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "EnumerationPropertyEditor.h"
//---------------------------------------------------------------------------
#include "EnumerationElementEditor.h"
//---------------------------------------------------------------------------



IMPLEMENT_GEOX_CLASS( EnumerationPropertyEditor, 0 )
{
}


EnumerationPropertyEditor::EnumerationPropertyEditor(void)
{
	m_ElementEditor = NULL;
}

QWidget * EnumerationPropertyEditor::makeEditor(QWidget *parent, bool readOnly)
{
	pAssert( m_Object != NULL );
	pAssert( m_Property != NULL );
	pAssert( m_ElementEditor == NULL );

	m_ElementEditor = new EnumerationElementEditor( parent );
	m_ElementEditor->setup( m_Object, m_Offset, m_Property, readOnly );

	return m_ElementEditor;
}

void EnumerationPropertyEditor::update()
{
	pAssert( m_ElementEditor != NULL );
	m_ElementEditor->sltUpdate();
}

card32 EnumerationPropertyEditor::getPreferredHeight()
{
	pAssert( m_ElementEditor != NULL );
	return m_ElementEditor->height();
}
