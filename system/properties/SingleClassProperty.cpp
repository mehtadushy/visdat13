//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "SingleClassProperty.h"
//---------------------------------------------------------------------------



SingleClassProperty::SingleClassProperty(const string cName, const Version introducedInVersion)
:ClassProperty( cName, introducedInVersion )
{
	m_Offset = NULL;
}

SingleClassProperty::~SingleClassProperty(void)
{
}
