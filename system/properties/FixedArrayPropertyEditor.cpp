//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "FixedArrayPropertyEditor.h"
//---------------------------------------------------------------------------
#include "FixedArrayElementEditor.h"
//---------------------------------------------------------------------------



IMPLEMENT_GEOX_CLASS( FixedArrayPropertyEditor, 0 )
{
}


FixedArrayPropertyEditor::FixedArrayPropertyEditor()
{
	m_ElementEditor = NULL;
}

FixedArrayPropertyEditor::~FixedArrayPropertyEditor(void)
{
}

QWidget * FixedArrayPropertyEditor::makeEditor(QWidget *parent, bool readOnly)
{
	pAssert( m_Object != NULL );
	pAssert( m_Property != NULL );
	pAssert( m_ElementEditor == NULL );

	m_ElementEditor = new FixedArrayElementEditor( parent );
	m_ElementEditor->setup( m_Object, m_Offset, m_Property, readOnly );

	return m_ElementEditor;
}

void FixedArrayPropertyEditor::update()
{
	pAssert( m_ElementEditor != NULL );
	m_ElementEditor->sltUpdate();
}

card32 FixedArrayPropertyEditor::getPreferredHeight()
{
	pAssert( m_ElementEditor != NULL );
	return m_ElementEditor->height();
}