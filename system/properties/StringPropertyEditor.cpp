//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "StringPropertyEditor.h"
//---------------------------------------------------------------------------
#include "StringElementEditor.h"
//---------------------------------------------------------------------------



IMPLEMENT_GEOX_CLASS( StringPropertyEditor, 0 )
{
}


StringPropertyEditor::StringPropertyEditor(void)
{
	m_ElementEditor = NULL;
}

StringPropertyEditor::~StringPropertyEditor(void)
{
}

QWidget * StringPropertyEditor::makeEditor(QWidget *parent, bool readOnly)
{
	pAssert( m_Object != NULL );
	pAssert( m_Property != NULL );
	pAssert( m_ElementEditor == NULL );

	m_ElementEditor = new StringElementEditor( parent );
	m_ElementEditor->setup( m_Object, m_Offset, m_Property, readOnly );

	return m_ElementEditor;

}

void StringPropertyEditor::update()
{
	pAssert( m_ElementEditor != NULL );
	m_ElementEditor->sltUpdate();
}

card32 StringPropertyEditor::getPreferredHeight()
{
	pAssert( m_ElementEditor != NULL );
	return m_ElementEditor->height();
}