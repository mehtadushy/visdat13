//---------------------------------------------------------------------------
#ifndef StructClassPropertyH
#define StructClassPropertyH
//---------------------------------------------------------------------------
#include "CompoundClassProperty.h"
//---------------------------------------------------------------------------
#include <vector>
//---------------------------------------------------------------------------


using namespace std;

class FixedArrayClassProperty;

class StructClassProperty : public CompoundClassProperty
{
private:

	pointer_diff	m_Offset;
	vector<ClassProperty*> m_Properties;

protected:
	StructClassProperty() {m_Offset = NULL;}
	StructClassProperty(const string cName, const Version introducedInVersion);

public:
	/// constructs a property holding n properties, becomes owner of properties
	StructClassProperty(const string cName, const Version introducedInVersion,
			pointer_diff offset, vector<ClassProperty*> &properties);
	~StructClassProperty(void);

	void setOffset( pointer_diff offset ) {m_Offset = offset;}
	pointer_diff getOffset() const {return m_Offset;}

	card32 getNumProperties() const {return (card32)m_Properties.size();}
	void addProperty( ClassProperty * prop ) {m_Properties.push_back( prop );}
	void clearProperties() {m_Properties.clear();}
	ClassProperty * getProperty( card32 i ) const {pAssert( i < getNumProperties() );return m_Properties[i];}


   virtual void writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const;
   virtual void readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const;
   virtual string toString(const Object* obj, pointer_diff offset) const;
   virtual void fromString(Object* obj, pointer_diff offset, const string str) const;
	virtual const string getTypeName() const {return "Struct";}

	virtual void assignProperty(const Object *from, Object *to, pointer_diff offset) const;
   virtual ClassProperty* copyClassProperty();

	friend class FixedArrayClassProperty;
};



#endif