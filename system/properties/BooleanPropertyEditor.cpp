//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "BooleanPropertyEditor.h"
//---------------------------------------------------------------------------
#include <QtGui/QCheckBox>
#include "Object.h"
#include "BooleanElementEditor.h"
//---------------------------------------------------------------------------



IMPLEMENT_GEOX_CLASS( BooleanPropertyEditor, 0 )
{
}


BooleanPropertyEditor::BooleanPropertyEditor()
{
	m_ElementEditor = NULL;
}

BooleanPropertyEditor::~BooleanPropertyEditor(void)
{
}

QWidget * BooleanPropertyEditor::makeEditor(QWidget *parent, bool readOnly)
{
	pAssert( m_Object != NULL );
	pAssert( m_Property != NULL );
	pAssert( m_ElementEditor == NULL );

	m_ElementEditor = new BooleanElementEditor( parent );
	m_ElementEditor->setup( m_Object, m_Offset, m_Property, readOnly );

	return m_ElementEditor;
}

void BooleanPropertyEditor::update()
{
	pAssert( m_ElementEditor != NULL );
	m_ElementEditor->sltUpdate();
}

card32 BooleanPropertyEditor::getPreferredHeight()
{
	return 30;
}
