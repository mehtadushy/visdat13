//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ClassTypePropertyEditor.h"
//---------------------------------------------------------------------------
#include "ClassTypeClassPropertyEditorWidget.h"
#include "Delegate.h"
#include "ClassTypeClassProperty.h"
//---------------------------------------------------------------------------



IMPLEMENT_GEOX_CLASS( ClassTypePropertyEditor ,0)
{
}


ClassTypePropertyEditor::ClassTypePropertyEditor(void)
{
	m_ElementEditor = NULL;
}

ClassTypePropertyEditor::~ClassTypePropertyEditor(void)
{
}

QWidget * ClassTypePropertyEditor::makeEditor(QWidget *parent, bool readOnly)
{
	pAssert( m_Object != NULL );
	pAssert( m_Property != NULL );
	pAssert( m_ElementEditor == NULL );

	m_ElementEditor = new ClassTypeClassPropertyEditorWidget( parent );

	ClassTypeClassProperty *prop = dynamic_cast<ClassTypeClassProperty*>(m_Property);
	pAssert(prop);
	m_ElementEditor->setup( prop );
	m_ElementEditor->updateEditor();
	m_ElementEditor->setSelection(prop->getValue(m_Object, 0));
	m_ElementEditor->setNotify(Delegate(this, (Delegate::MethodPtr)&ClassTypePropertyEditor::writeBackSelection));
	return m_ElementEditor;
}

void ClassTypePropertyEditor::update()
{
	pAssert( m_ElementEditor != NULL );
	const ClassTypeClassProperty *prop = dynamic_cast<const ClassTypeClassProperty*>(m_Property);
	pAssert(prop);
	m_ElementEditor->setSelection(prop->getValue(m_Object, 0));
}

card32 ClassTypePropertyEditor::getPreferredHeight()
{
	pAssert( m_ElementEditor != NULL );
	return m_ElementEditor->height();
}

void ClassTypePropertyEditor::writeBackSelection() 
{
	const ClassTypeClassProperty *prop = dynamic_cast<const ClassTypeClassProperty*>(m_Property);
	pAssert(prop);
	prop->setValue(m_Object, 0, m_ElementEditor->getSelected());
}
