//---------------------------------------------------------------------------
#ifndef VariableArrayClassPropertyH
#define VariableArrayClassPropertyH
//---------------------------------------------------------------------------
#include "CompoundClassProperty.h"
//---------------------------------------------------------------------------
#include "StringClassProperty.h"
#include "NumericalClassProperty.h"
#include "BooleanClassProperty.h"
//---------------------------------------------------------------------------


class ClassProperty;
class VariableArrayElementEditor;

class  VariableArrayClassProperty : public CompoundClassProperty
{
	friend class VariableArrayElementEditor;

public:
   // property-access-methods
   typedef void (Object::* SetSizeMethod)(mpcard);
   typedef void (Object::* DeleteMethod)(mpcard);
   typedef char* (Object::* GetMethod)(mpcard);
   typedef mpcard (Object::* GetNumMethod)();
   typedef string (Object::* GetObjectNameMethod)(mpcard );

// methods for copy&paste
/*
	void setNumXXs(mpcard size);
	void deleteXX( mpcard index );
	char * getXX( mpcard index );
	mpcard getNumXXs();
	string getXXName(mpcard index );
)*/

public:
	VariableArrayClassProperty(const string cName, const Version introducedInVersion, ClassProperty * elementProperty,
			SetSizeMethod setSize, DeleteMethod del, GetMethod get, GetNumMethod getNum, GetObjectNameMethod getName );
	~VariableArrayClassProperty(void);

	virtual const string     getTypeName() const;
   virtual void             writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const;
   virtual void             readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const;
   virtual string           toString(const Object* obj, pointer_diff offset) const;
   virtual void             fromString(Object* obj, pointer_diff offset, const string str) const;
   virtual void             assignProperty(const Object *from, Object *to, pointer_diff offset) const;
   virtual ClassProperty*   copyClassProperty();

	ClassProperty * getElementClassProperty() const {return m_ElementClassProperty;}

private:

	ClassProperty		*		m_ElementClassProperty;
	SetSizeMethod				setSizeMethod;
	DeleteMethod				delMethod;
	GetMethod					getMethod;
	GetNumMethod				getNumMethod;
	GetObjectNameMethod		getNameMethod;

public:

	void setSize( Object * obj, mpcard size) const
	{
		(obj->*setSizeMethod)(size);
	}

	void deleteElement( Object * obj, mpcard index ) const
	{
		(obj->*delMethod)(index);
	}

	char * getElement( Object * obj, mpcard index ) const
	{
		return (obj->*getMethod)(index);
	}

	mpcard getNumElements( Object * obj ) const
	{
		return (obj->*getNumMethod)();
	}

	string getObjectName( Object * obj, mpcard index ) const;

	pointer_diff getDiff( Object * obj, mpcard index ) const;

};

#define ADD_VAR_ARRAY_PROP( propName, version, elementProperty ) \
	((MetaClass*)getClass())->addProperty( new VariableArrayClassProperty( string(#propName) + "s" , MetaClass::FIRST_VERSION + version, \
	 elementProperty,\
	 (VariableArrayClassProperty::SetSizeMethod)&setNum##propName##s, \
	 (VariableArrayClassProperty::DeleteMethod)&delete##propName, \
	 (VariableArrayClassProperty::GetMethod) &get##propName, \
	 (VariableArrayClassProperty::GetNumMethod) &getNum##propName##s, \
	 (VariableArrayClassProperty::GetObjectNameMethod) &get##propName##Name ));

#define ADD_STRING_VAR_ARRAY_PROP( propName, version ) \
	ADD_VAR_ARRAY_PROP( propName, version, new StringClassProperty("element", MetaClass::FIRST_VERSION + version,NULL))

#define ADD_INT32_VAR_ARRAY_PROP( propName, version ) \
	ADD_VAR_ARRAY_PROP( propName, version, new Int32ClassProperty("element", MetaClass::FIRST_VERSION + version,NULL))
#define ADD_CARD32_VAR_ARRAY_PROP( propName, version ) \
	ADD_VAR_ARRAY_PROP( propName, version, new Card32ClassProperty("element", MetaClass::FIRST_VERSION + version,NULL))
#define ADD_FLOAT32_VAR_ARRAY_PROP( propName, version ) \
	ADD_VAR_ARRAY_PROP( propName, version, new Float32ClassProperty("element", MetaClass::FIRST_VERSION + version,NULL))
#define ADD_VECTOR3F_VAR_ARRAY_PROP( propName, version ) \
	ADD_VAR_ARRAY_PROP( \
		propName, \
		version, \
		new FixedArrayClassProperty( \
			#propName, \
			MetaClass::FIRST_VERSION + version, \
			(pointer_diff)(&( obj_null_pointer->propName ) - NULL), \
			new Float32ClassProperty( "", version, (pointer_diff)NULL, NULL ), \
			3, \
			sizeof(float32), \
			3 \
		) \
	)

#define IMPLEMENT_VAR_ARRAY_METHODS_STL( propName ) \
	void setNum##propName##s(mpcard size) \
		{ propName.resize(size);} \
	void delete##propName( mpcard index ) \
		{pAssert( index < propName.size()); propName.erase( propName.begin()+index );}\
	char * get##propName( mpcard index ) \
		{pAssert( index < propName##.size()); return (char*)&propName[index];} \
	mpcard getNum##propName##s() \
		{return propName.size();} \
	string get##propName##Name(mpcard index ) \
		{pAssert( index < propName.size()); return "Object";}

#define IMPLEMENT_VAR_ARRAY_METHODS_STL_UPDATE( propName, callOnChange ) \
	void setNum##propName##s(mpcard size) \
		{ propName.resize(size); callOnChange();} \
	void delete##propName( mpcard index ) \
		{pAssert( index < propName.size()); propName.erase( propName.begin()+index ); callOnChange();}\
	char * get##propName( mpcard index ) \
		{pAssert( index < propName.size()); return (char*)&propName[index];} \
	mpcard getNum##propName##s() \
		{return propName.size();} \
	string get##propName##Name(mpcard index ) \
		{pAssert( index < propName.size()); return "Object";}

#define IMPLEMENT_VAR_STRING_ARRAY_METHODS_STL_UPDATE( propName, callOnChange ) \
	void setNum##propName##s(mpcard size) \
		{ propName.resize(size); callOnChange();} \
	void delete##propName( mpcard index ) \
		{pAssert( index < propName.size()); propName.erase( propName.begin()+index ); callOnChange();}\
	char * get##propName( mpcard index ) \
		{pAssert( index < propName.size()); return (char*)&propName[index];} \
	mpcard getNum##propName##s() \
		{return propName.size();} \
	string get##propName##Name(mpcard index ) \
		{pAssert( index < propName.size()); return propName[index];}



#endif
