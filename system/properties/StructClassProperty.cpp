//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "StructClassProperty.h"
//---------------------------------------------------------------------------
#include "Tools.h"
#include "ObjectStreams.h"
//---------------------------------------------------------------------------



StructClassProperty::StructClassProperty(const string cName, const Version introducedInVersion )
:CompoundClassProperty( cName, introducedInVersion )
{
	m_Offset = NULL;
}

StructClassProperty::StructClassProperty(const string cName, const Version introducedInVersion,
			pointer_diff offset, vector<ClassProperty*> &properties)
:CompoundClassProperty( cName, introducedInVersion )
{
	// check for non-compundable property
	for( unsigned i=0;i<properties.size();i++ )
		pAssert( properties[i]->getCompoundAble() == true );

	m_Properties = properties;
	m_Offset = offset;
}

StructClassProperty::~StructClassProperty(void)
{
	for( unsigned i=0;i<m_Properties.size();i++ )
		delete m_Properties[i];
}

void StructClassProperty::writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const
{
	for( unsigned i=0;i<m_Properties.size();i++ )
		m_Properties[i]->writeProperty( obj,  m_Offset, out );
}

void StructClassProperty::readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const
{
	for( unsigned i=0;i<m_Properties.size();i++ )
		m_Properties[i]->readProperty( obj, m_Offset, in );
}

string StructClassProperty::toString(const Object* obj, pointer_diff offset) const
{
	forget(obj);
	return "Struct";
}

void StructClassProperty::fromString(Object* obj, pointer_diff offset, const string str) const
{
	forget(obj);
	forget(str);
}

void StructClassProperty::assignProperty(const Object *from, Object *to, pointer_diff offset) const
{
	for( unsigned i=0;i<m_Properties.size();i++ )
		m_Properties[i]->assignProperty( from, to, m_Offset );

}

ClassProperty* StructClassProperty::copyClassProperty()
{
	StructClassProperty * ret = new StructClassProperty;
	*ret = *this;
	ret->m_Properties.clear();
	for( unsigned i=0;i<m_Properties.size();i++ )
	{
		ret->m_Properties.push_back( m_Properties[i]->copyClassProperty() );
	}
	return ret;
}
