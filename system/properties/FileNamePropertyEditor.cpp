//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "FileNamePropertyEditor.h"
//---------------------------------------------------------------------------
#include "FileNameElementEditor.h"
//---------------------------------------------------------------------------



IMPLEMENT_GEOX_CLASS( FileNamePropertyEditor, 0 )
{
}


FileNamePropertyEditor::FileNamePropertyEditor(void)
{
	m_ElementEditor = NULL;
}

FileNamePropertyEditor::~FileNamePropertyEditor(void)
{
}

QWidget * FileNamePropertyEditor::makeEditor(QWidget *parent, bool readOnly)
{
	pAssert( m_Object != NULL );
	pAssert( m_Property != NULL );
	pAssert( m_ElementEditor == NULL );

	m_ElementEditor = new FileNameElementEditor( parent );
	m_ElementEditor->setup( m_Object, m_Offset, m_Property, readOnly );

	return m_ElementEditor;

}

void FileNamePropertyEditor::update()
{
	pAssert( m_ElementEditor != NULL );
	m_ElementEditor->sltUpdate();
}

card32 FileNamePropertyEditor::getPreferredHeight()
{
	pAssert( m_ElementEditor != NULL );
	return m_ElementEditor->height();
}