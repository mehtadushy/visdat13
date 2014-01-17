//---------------------------------------------------------------------------
#ifndef FixedArrayClassPropertyH
#define FixedArrayClassPropertyH
//---------------------------------------------------------------------------
#include "CompoundClassProperty.h"
#include "NumericalClassProperty.h"
//---------------------------------------------------------------------------



class  FixedArrayClassProperty : public CompoundClassProperty
{
private:

	pointer_diff calculateElementDiff( unsigned i ) const;
	
	ClassProperty	*	m_ElementProperty;
	pointer_diff		m_Offset;
	card32				m_EntriesPerRow;
	card32				m_ArraySize;
	card32				m_Stride;

protected:
	FixedArrayClassProperty() {}

public:
	/// contructs an array property, becomes owner of prop
	FixedArrayClassProperty(const string cName, const Version introducedInVersion,
			pointer_diff offset, ClassProperty * prop, card32 arraySize, card32 stride, card32 entriesPerRow = 1);
	~FixedArrayClassProperty(void);

	pointer_diff getOffset() const {return m_Offset;}
	card32 getEntriesPerRow() const {return m_EntriesPerRow;}
	card32 getArraySize() const {return m_ArraySize;}
	card32 getStride() const {return m_Stride;}
	ClassProperty * getElementProperty() const {return m_ElementProperty;}
	
	virtual const string getTypeName() const;
   virtual void writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const;
   virtual void readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const;
   virtual string toString(const Object* obj, pointer_diff offset) const;
   virtual void fromString(Object* obj, pointer_diff offset, const string str) const;

	virtual void assignProperty(const Object *from, Object *to, pointer_diff offset) const;

   virtual ClassProperty* copyClassProperty();

};

#define ADD_FLOAT32_FIXED_ARRAY_PROP( propName, version, count, entriesPerRow ) \
	((MetaClass*)getClass())->addProperty( new FixedArrayClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
	(pointer_diff)(&( obj_null_pointer->propName ) - NULL) , new Float32ClassProperty("", version , NULL) \
	,count, sizeof(float32), entriesPerRow ));

#define ADD_INT32_FIXED_ARRAY_PROP( propName, version, count, entriesPerRow ) \
	((MetaClass*)getClass())->addProperty( new FixedArrayClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
	(pointer_diff)(&( obj_null_pointer->propName ) - NULL) , new Int32ClassProperty("", version , NULL) \
	,count, sizeof(int32), entriesPerRow ));

#define ADD_CARD32_FIXED_ARRAY_PROP( propName, version, count, entriesPerRow ) \
	((MetaClass*)getClass())->addProperty( new FixedArrayClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
	(pointer_diff)(&( obj_null_pointer->propName ) - NULL) , new Card32ClassProperty("", version , NULL) \
	,count, sizeof(card32), entriesPerRow ));

#define ADD_CARD8_FIXED_ARRAY_PROP( propName, version, count, entriesPerRow ) \
	((MetaClass*)getClass())->addProperty( new FixedArrayClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
	(pointer_diff)(&( obj_null_pointer->propName ) - NULL) , new Card8ClassProperty("", version , NULL) \
	,count, sizeof(card8), entriesPerRow ));

#define ADD_BOOLEAN_FIXED_ARRAY_PROP( propName, version, count, entriesPerRow ) \
	((MetaClass*)getClass())->addProperty( new FixedArrayClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
	(pointer_diff)(&( obj_null_pointer->propName ) - NULL) , new BooleanClassProperty("", version , NULL) \
	,count, sizeof(bool), entriesPerRow )); 


#define ADD_OBJECT_FIXED_ARRAY_PROP( propName, version, count, baseClass, is_owner ) \
	((MetaClass*)getClass())->addProperty( new FixedArrayClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
	(pointer_diff)(&( obj_null_pointer->propName ) - NULL) , new ObjectClassProperty("", version , NULL, \
	baseClass , is_owner ) ,count, sizeof(card32), 1 ));

#define ADD_VECTOR2F_PROP_UPDATE( propName, version, updateMethodPtr ) \
	((MetaClass*)getClass())->addProperty( new FixedArrayClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
	(pointer_diff)(&( obj_null_pointer->propName ) - NULL) , new Float32ClassProperty("", version , NULL, (Float32UpdateMethod)updateMethodPtr) \
	,2, sizeof(float32), 2 ));

#define ADD_VECTOR2I_PROP_UPDATE( propName, version, updateMethodPtr ) \
	((MetaClass*)getClass())->addProperty( new FixedArrayClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
	(pointer_diff)(&( obj_null_pointer->propName ) - NULL) , new Int32ClassProperty("", version , NULL, (Int32UpdateMethod)updateMethodPtr) \
	,2, sizeof(int32), 2 ));

#define ADD_VECTOR3F_PROP_UPDATE( propName, version, updateMethodPtr ) \
	((MetaClass*)getClass())->addProperty( new FixedArrayClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
	(pointer_diff)(&( obj_null_pointer->propName ) - NULL) , new Float32ClassProperty("",version, NULL, (Float32UpdateMethod)updateMethodPtr) \
	,3, sizeof(float32), 3 ));

#define ADD_VECTOR3I_PROP_UPDATE( propName, version, updateMethodPtr ) \
	((MetaClass*)getClass())->addProperty( new FixedArrayClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
	(pointer_diff)(&( obj_null_pointer->propName ) - NULL) , new Int32ClassProperty("",version, NULL, (Int32UpdateMethod)updateMethodPtr) \
	,3, sizeof(float32), 3 ));

#define ADD_VECTOR4F_PROP_UPDATE( propName, version, updateMethodPtr ) \
	((MetaClass*)getClass())->addProperty( new FixedArrayClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
	(pointer_diff)(&( obj_null_pointer->propName ) - NULL) , new Float32ClassProperty("",version, NULL, (Float32UpdateMethod)updateMethodPtr) \
	,4, sizeof(float32), 4 ));

#define ADD_VECTOR6F_PROP_UPDATE( propName, version, updateMethodPtr ) \
	((MetaClass*)getClass())->addProperty( new FixedArrayClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
	(pointer_diff)(&( obj_null_pointer->propName ) - NULL) , new Float32ClassProperty("",version, NULL, (Float32UpdateMethod)updateMethodPtr) \
	,6, sizeof(float32), 6 ));

#define ADD_MATRIX3F_PROP_UPDATE( propName, version, updateMethodPtr ) \
	((MetaClass*)getClass())->addProperty( new FixedArrayClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
	(pointer_diff)(&( obj_null_pointer->propName ) - NULL) , new Float32ClassProperty("",version, NULL, (Float32UpdateMethod)updateMethodPtr) \
	,9, sizeof(float32), 3 ));

#define ADD_MATRIX4F_PROP_UPDATE( propName, version, updateMethodPtr ) \
	((MetaClass*)getClass())->addProperty( new FixedArrayClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
	(pointer_diff)(&( obj_null_pointer->propName ) - NULL) , new Float32ClassProperty("",version, NULL, (Float32UpdateMethod)updateMethodPtr) \
	,16, sizeof(float32), 4 ));

#define ADD_MATRIX6F_PROP_UPDATE( propName, version, updateMethodPtr ) \
	((MetaClass*)getClass())->addProperty( new FixedArrayClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
	(pointer_diff)(&( obj_null_pointer->propName ) - NULL) , new Float32ClassProperty("",version, NULL, (Float32UpdateMethod)updateMethodPtr) \
	,36, sizeof(float32), 6 ));

#define ADD_VECTOR2F_PROP( propName, version ) ADD_VECTOR2F_PROP_UPDATE( propName, version, NULL )
#define ADD_VECTOR2I_PROP( propName, version ) ADD_VECTOR2I_PROP_UPDATE( propName, version, NULL )
#define ADD_VECTOR3F_PROP( propName, version ) ADD_VECTOR3F_PROP_UPDATE( propName, version, NULL )
#define ADD_VECTOR6F_PROP( propName, version ) ADD_VECTOR6F_PROP_UPDATE( propName, version, NULL )
#define ADD_VECTOR3I_PROP( propName, version ) ADD_VECTOR3I_PROP_UPDATE( propName, version, NULL )
#define ADD_VECTOR4F_PROP( propName, version ) ADD_VECTOR4F_PROP_UPDATE( propName, version, NULL )
#define ADD_MATRIX3F_PROP( propName, version ) ADD_MATRIX3F_PROP_UPDATE( propName, version, NULL )
#define ADD_MATRIX4F_PROP( propName, version ) ADD_MATRIX4F_PROP_UPDATE( propName, version, NULL )
#define ADD_MATRIX6F_PROP( propName, version ) ADD_MATRIX6F_PROP_UPDATE( propName, version, NULL )



#endif
