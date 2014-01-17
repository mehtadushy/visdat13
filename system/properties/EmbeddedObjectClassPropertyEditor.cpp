//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "EmbeddedObjectClassPropertyEditor.h"
//---------------------------------------------------------------------------
#include "EmbeddedObjectElementEditor.h"
//---------------------------------------------------------------------------



IMPLEMENT_GEOX_CLASS( EmbeddedObjectClassPropertyEditor ,0)
{
}
IMPLEMENT_GEOX_CLASS( EmbeddedObjectClassPropertyInlineEditor ,0)
{
}


// ---


EmbeddedObjectClassPropertyEditor::EmbeddedObjectClassPropertyEditor(void)
{
	m_ElementEditor = NULL;
}

QWidget * EmbeddedObjectClassPropertyEditor::makeEditor(QWidget *parent, bool readOnly)
{
	pAssert( m_Object != NULL );
	pAssert( m_Property != NULL );
	pAssert( m_ElementEditor == NULL );

	m_ElementEditor = new EmbeddedObjectElementEditor( parent );
	m_ElementEditor->setup( m_Object, m_Offset, m_Property, readOnly );

	return m_ElementEditor;
}

void EmbeddedObjectClassPropertyEditor::update()
{
	pAssert( m_ElementEditor != NULL );
	m_ElementEditor->sltUpdate();
}

card32 EmbeddedObjectClassPropertyEditor::getPreferredHeight()
{
	pAssert( m_ElementEditor != NULL );
	return m_ElementEditor->height();
}

// --

EmbeddedObjectClassPropertyInlineEditor::EmbeddedObjectClassPropertyInlineEditor(void)
{
	m_ElementEditor = NULL;
}

QWidget * EmbeddedObjectClassPropertyInlineEditor::makeEditor(QWidget *parent, bool readOnly)
{
	pAssert( m_Object != NULL );
	pAssert( m_Property != NULL );
	pAssert( m_ElementEditor == NULL );

	m_ElementEditor = new EmbeddedObjectInlineElementEditor( parent );
	m_ElementEditor->setup( m_Object, m_Offset, m_Property, readOnly );

	return m_ElementEditor;
}

void EmbeddedObjectClassPropertyInlineEditor::update()
{
	pAssert( m_ElementEditor != NULL );
	m_ElementEditor->sltUpdate();
}

card32 EmbeddedObjectClassPropertyInlineEditor::getPreferredHeight()
{
	pAssert( m_ElementEditor != NULL );
	return m_ElementEditor->height();
}