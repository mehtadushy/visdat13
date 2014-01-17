//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "VariableArrayClassPropertyEditor.h"
//---------------------------------------------------------------------------
#include "VariableArrayElementEditor.h"
//---------------------------------------------------------------------------



IMPLEMENT_GEOX_CLASS( VariableArrayClassPropertyEditor, 0 )
{
}

VariableArrayClassPropertyEditor::VariableArrayClassPropertyEditor(void)
{
	m_ElementEditor = NULL;
}

QWidget * VariableArrayClassPropertyEditor::makeEditor(QWidget *parent, bool readOnly)
{
	pAssert( m_Object != NULL );
	pAssert( m_Property != NULL );
	pAssert( m_ElementEditor == NULL );

	m_ElementEditor = new VariableArrayElementEditor( parent );
	m_ElementEditor->setup( m_Object, m_Offset, m_Property, readOnly );

	return m_ElementEditor;
}

void VariableArrayClassPropertyEditor::update()
{
	pAssert( m_ElementEditor != NULL );
	m_ElementEditor->sltUpdate();
}

card32 VariableArrayClassPropertyEditor::getPreferredHeight()
{
	return m_ElementEditor->height();
}