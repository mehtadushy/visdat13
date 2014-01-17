//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ObjectPropertyEditor.h"
//---------------------------------------------------------------------------
#include "ObjectElementEditor.h"
//---------------------------------------------------------------------------



IMPLEMENT_GEOX_CLASS( ObjectPropertyEditor ,0)
{
}


ObjectPropertyEditor::ObjectPropertyEditor(void)
{
	m_ElementEditor = NULL;
}

ObjectPropertyEditor::~ObjectPropertyEditor(void)
{
}

QWidget * ObjectPropertyEditor::makeEditor(QWidget *parent, bool readOnly)
{
	pAssert( m_Object != NULL );
	pAssert( m_Property != NULL );
	pAssert( m_ElementEditor == NULL );

	m_ElementEditor = new ObjectElementEditor( parent );
	m_ElementEditor->setup( m_Object, m_Offset, m_Property, readOnly );

	return m_ElementEditor;
}

void ObjectPropertyEditor::update()
{
	pAssert( m_ElementEditor != NULL );
	m_ElementEditor->sltUpdate();
}

card32 ObjectPropertyEditor::getPreferredHeight()
{
	pAssert( m_ElementEditor != NULL );
	return m_ElementEditor->height();
}