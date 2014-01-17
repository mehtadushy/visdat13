//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "FixedArrayClassProperty.h"
//---------------------------------------------------------------------------
#include "Tools.h"
#include "ObjectStreams.h"
#include "FixedArrayPropertyEditor.h"
//---------------------------------------------------------------------------



FixedArrayClassProperty::FixedArrayClassProperty(const string cName, const Version introducedInVersion,
			pointer_diff offset, ClassProperty * prop, card32 arraySize, card32 stride, card32 entriesPerRow)
:CompoundClassProperty(cName,introducedInVersion)
{
	m_ElementProperty = prop;
	m_Offset = offset;
	m_EntriesPerRow = entriesPerRow;
	m_ArraySize = arraySize;
	m_Stride = stride;
   addPropertyEditor(FixedArrayPropertyEditor::getClass(), true);
}

FixedArrayClassProperty::~FixedArrayClassProperty(void)
{
	delete m_ElementProperty;
}

const string FixedArrayClassProperty::getTypeName() const
{
	return "FixedArray of " + m_ElementProperty->getTypeName();
}

void FixedArrayClassProperty::writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const
{
	for( unsigned i=0;i<m_ArraySize;i++ )
	{
		m_ElementProperty->writeProperty( obj, offset + calculateElementDiff(i), out );
	}
}

void FixedArrayClassProperty::readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const
{
	for( unsigned i=0;i<m_ArraySize;i++ )
	{
		m_ElementProperty->readProperty( obj, offset + calculateElementDiff(i), in );
	}
}

string FixedArrayClassProperty::toString(const Object* obj, pointer_diff offset) const
{
	forget(obj);
	return "FixedArray::toString() not implemented.";
}

void FixedArrayClassProperty::fromString(Object* obj, pointer_diff offset, const string str) const
{
	forget(obj);
	forget( str );
}

void FixedArrayClassProperty::assignProperty(const Object *from, Object *to, pointer_diff offset) const
{
	for( unsigned i=0;i<m_ArraySize;i++ )
	{
		pointer_diff diff = calculateElementDiff(i);
		m_ElementProperty->assignProperty( from, to , diff + offset);
	}
}


ClassProperty* FixedArrayClassProperty::copyClassProperty()
{
	FixedArrayClassProperty * ret = new FixedArrayClassProperty;
	*ret = *this;
	ret->m_ElementProperty = m_ElementProperty->copyClassProperty();
	return ret;

}

pointer_diff FixedArrayClassProperty::calculateElementDiff( unsigned i ) const
{
	return m_Offset + (pointer_diff)i*m_Stride;
}

