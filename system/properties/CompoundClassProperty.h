//---------------------------------------------------------------------------
#ifndef CompoundClassPropertyH
#define CompoundClassPropertyH
//---------------------------------------------------------------------------
#include "ClassProperty.h"
//---------------------------------------------------------------------------


/**
	base class for compound class properties
*/
class  CompoundClassProperty : public ClassProperty
{
public:
	CompoundClassProperty(const string cName, const Version introducedInVersion);
	~CompoundClassProperty(void);

protected:
	CompoundClassProperty() {}

};



#endif
