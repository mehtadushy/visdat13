//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "VariableArrayClassProperty.h"
//---------------------------------------------------------------------------
#include "ClassProperty.h"
#include "ObjectStreams.h"
#include "StringHelper.h"
#include "VariableArrayClassPropertyEditor.h"
//---------------------------------------------------------------------------



VariableArrayClassProperty::VariableArrayClassProperty(const string cName, const Version introducedInVersion, ClassProperty * cp,
			SetSizeMethod setSize, DeleteMethod del, GetMethod get, GetNumMethod getNum, GetObjectNameMethod getName)
:CompoundClassProperty(cName, introducedInVersion)
{
	setSizeMethod = setSize;
	delMethod = del;
	getMethod = get;
	getNumMethod = getNum;
	getNameMethod = getName;
	m_ElementClassProperty = cp;
	addPropertyEditor(VariableArrayClassPropertyEditor::getClass(), true);
}

VariableArrayClassProperty::~VariableArrayClassProperty(void)
{
}

const string VariableArrayClassProperty::getTypeName() const
{
	string ret = "Variable Array of ";
	ret += m_ElementClassProperty->getCName();
	return ret;
}

void VariableArrayClassProperty::writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const
{
	mpcard numElements = getNumElements( (Object*)obj );
	out->writeMaxPlatformCard( numElements );

	for( max_platform_card i=0;i<numElements;i++ )
	{
		pointer_diff diff = getDiff((Object*)obj,i);
		m_ElementClassProperty->writeProperty( (Object*)obj, diff, out );
	}
}

void VariableArrayClassProperty::readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const
{
	max_platform_card num_elements;
	in->readMaxPlatformCard(num_elements);
	setSize( obj, num_elements );
	for( max_platform_card i=0;i<num_elements;i++ )
	{
		pointer_diff diff = getDiff(obj,i);
		m_ElementClassProperty->readProperty( obj, diff, in );
	}
}

string VariableArrayClassProperty::toString(const Object* obj, pointer_diff offset) const
{
	return "not implemented.";
}

void VariableArrayClassProperty::fromString(Object* obj, pointer_diff offset, const string str) const
{
}

void VariableArrayClassProperty::assignProperty(const Object *from, Object *to, pointer_diff offset) const
{
	setSize( to, getNumElements( (Object*)from ) );
	for( mpcard i=0;i<getNumElements(to);i++ )
	{
		pointer_diff diff = getDiff(to,i);
		m_ElementClassProperty->assignProperty( from, to, diff );
	}
}


pointer_diff VariableArrayClassProperty::getDiff( Object * obj, mpcard index ) const
{
	return getElement(obj,index) - (char*)obj;
}

string VariableArrayClassProperty::getObjectName( Object * obj, mpcard index ) const
{
	if( getNameMethod == NULL )
	{
		return intToStr( index );
	}
	else
	{
		return (obj->*getNameMethod)(index);
	}
}

ClassProperty*   VariableArrayClassProperty::copyClassProperty()
{
	VariableArrayClassProperty * ret = new VariableArrayClassProperty( getCName(), 
		getIntroducedInVersion(), m_ElementClassProperty, setSizeMethod, delMethod,
		getMethod, getNumMethod, getNameMethod );
	return ret;
}
