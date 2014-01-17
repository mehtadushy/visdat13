//---------------------------------------------------------------------------
#ifndef SingleClassPropertyH
#define SingleClassPropertyH
//---------------------------------------------------------------------------
#include "ClassProperty.h"
//---------------------------------------------------------------------------



class  SingleClassProperty : public ClassProperty
{
private:
	pointer_diff	m_Offset;	// offset to Object pointer

protected:
	SingleClassProperty() {m_Offset = NULL;}

public:
	SingleClassProperty(const string cName, const Version introducedInVersion);
	~SingleClassProperty(void);

	void setOffset( pointer_diff offset ) {m_Offset = offset;}
	pointer_diff getOffset() const {return m_Offset;}
};



#endif
