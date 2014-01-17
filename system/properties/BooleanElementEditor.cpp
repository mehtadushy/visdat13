//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "BooleanElementEditor.h"
//---------------------------------------------------------------------------
#include "BooleanClassProperty.h"
#include <QtGui/QCheckBox>
#include <QtGui/QBoxLayout>
//---------------------------------------------------------------------------



BooleanElementEditor::BooleanElementEditor(QWidget * parent, Qt::WFlags f)
:ElementEditor( parent, f )
{
	m_CheckBox = new QCheckBox( "", this );
	connect( m_CheckBox, SIGNAL( stateChanged(int) ), SLOT(sltStateChanged(int)));
	m_Property = NULL;
	m_Object = NULL;
	m_Pointer = NULL;
}

BooleanElementEditor::~BooleanElementEditor(void)
{
}

void BooleanElementEditor::setup(bool &b,  bool readOnly )
{
	pAssert( m_Object == NULL );
	pAssert( m_Property == NULL );
	pAssert( m_Pointer == NULL );

	m_CheckBox->setEnabled( !readOnly );
	m_Pointer = &b;

	updateEditor();
	resize( 100, 30 );
}

void BooleanElementEditor::setup( Object * obj, pointer_diff offset, ClassProperty * prop, bool readOnly )
{
	pAssert( m_Object == NULL );
	pAssert( m_Property == NULL );
	pAssert( m_Pointer == NULL );

	m_CheckBox->setEnabled( !readOnly );
	m_CheckBox->setText(prop->getCName().c_str());
	m_Object = obj;
	m_Property = prop;
	setOffset( offset );

	updateEditor();
	resize( 100, 30 );
}

void BooleanElementEditor::updateEditor()
{
	m_CheckBox->setCheckState( getElement() ? Qt::Checked : Qt::Unchecked );
}

void BooleanElementEditor::sltStateChanged( int state )
{
	setElement( m_CheckBox->checkState() == Qt::Checked );
	modified();
}


bool BooleanElementEditor::getElement()
{
	if( m_Pointer == NULL )
	{
		pAssert( m_Property != NULL );
		pAssert( m_Object != NULL );
		return ((BooleanClassProperty*)m_Property)->getValue(m_Object,getOffset());
	}
	else
		return *m_Pointer;
}

void BooleanElementEditor::setElement( bool b )
{
	if( m_Pointer == NULL )
	{
		pAssert( m_Property != NULL );
		pAssert( m_Object != NULL );
		((BooleanClassProperty*)m_Property)->setValue( m_Object,getOffset(), b );
	}
	else
		*m_Pointer = b;
}

void BooleanElementEditor::resizeEvent(QResizeEvent *e)
{
	int h = m_CheckBox->height();
	m_CheckBox->move( 5, height()/2 - h/2 );
}

#ifndef CMAKE_INTDIR
#ifdef _WIN32
#include "moc/BooleanElementEditor_moc.cpp"
#else
//#include "moc_BooleanElementEditor.cpp"
#endif
#endif