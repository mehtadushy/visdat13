//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "FixedArrayElementEditor.h"
//---------------------------------------------------------------------------
#include "FixedArrayClassProperty.h"
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include "PropertyEditor.h"
//---------------------------------------------------------------------------



FixedArrayElementEditor::FixedArrayElementEditor(QWidget * parent, Qt::WFlags f)
:ElementEditor( parent, f )
{
	m_Property = NULL;
	m_Object = NULL;
}

FixedArrayElementEditor::~FixedArrayElementEditor(void)
{
}


void FixedArrayElementEditor::setup( Object * obj, pointer_diff offset, ClassProperty * prop, bool readOnly )
{
	m_Object = obj;
	m_Property = dynamic_cast<FixedArrayClassProperty*>(prop);
	setOffset( offset );
	pAssert( m_Property != NULL );

	QGridLayout * layout = new QGridLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);

	ClassProperty * cp = m_Property->getElementProperty();
//	cp->setEditReadonly( readOnly );
	if( cp->getNumPropertyEditors() == 0 )
	{
		QLabel * label = new QLabel( QString("No editor found for ") + cp->getCName().c_str() );
		layout->addWidget( label,0,0 );
		return;
	}

	card32 entriesPerRow = m_Property->getEntriesPerRow();
	card32 row=0, column=0;
	card32 minHeight = 0;
	for( unsigned i=0;i<m_Property->getArraySize();i++ )
	{
		PropertyEditor * pe = (PropertyEditor*)cp->getDefaultPropertyEditor()->newInstance();
		pe->setup( obj, offset + m_Property->getOffset() + i*m_Property->getStride(), cp );
		QWidget * widget = pe->makeEditor( this, readOnly );
		layout->addWidget( widget, row, column );

		m_Editors.push_back( pe );

		if( (column % entriesPerRow) == 0 )
		{
			minHeight += widget->height();
		}
		
		column++;
		if( column >= entriesPerRow )
		{
			column = 0;
			row++;
		}
	}
	setMinimumHeight( minHeight );
	setMaximumHeight( minHeight );
}

void FixedArrayElementEditor::updateEditor()
{
	for( unsigned i=0;i<m_Editors.size();i++ )
		m_Editors[i]->update();
}
