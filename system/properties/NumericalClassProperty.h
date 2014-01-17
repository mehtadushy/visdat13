//---------------------------------------------------------------------------
#ifndef NumericalClassPropertyH
#define NumericalClassPropertyH
//---------------------------------------------------------------------------
#include "SingleClassProperty.h"
//---------------------------------------------------------------------------
#include <string>

//#ifdef TARGET_PLATFORM_MSVC2005FF
//   #include "values_vs.h"
//#endif

#include "PTypes.h"
//---------------------------------------------------------------------------


using namespace std;


void  NumericalClassProperty__addDefaultEditor(ClassProperty *c);

class  NumericalClassPropertyBase : public SingleClassProperty
{
protected:
   NumericalClassPropertyBase() {}
public:
   typedef float64 FloatType;

   NumericalClassPropertyBase( const string CName, const Version introducedInVersion )
      :SingleClassProperty( CName, introducedInVersion ) {}

   virtual void setMinimumValueDouble( const FloatType &value ) = 0;
   virtual void setMaximumValueDouble( const FloatType &value ) = 0;
   virtual FloatType getMinimumValueDouble() const = 0;
   virtual FloatType getMaximumValueDouble() const = 0;
   virtual FloatType getIncrementValueDouble() const = 0;

   virtual FloatType getValueDouble(const Object *obj, pointer_diff offset) const  = 0;
   virtual void setValueDouble(Object *obj, pointer_diff offset, FloatType v) const = 0;

};

template <class NumericalType>
class  NumericalClassProperty : public NumericalClassPropertyBase {
 public:
   enum IncrementType {itAdd, itMult};

   typedef NumericalType (Object::* NumericalTypeGetPropertyMethod)() const;
   typedef void (Object::* NumericalTypeSetPropertyMethod)(NumericalType);
   typedef void (Object::* UpdateMethod)();

 protected:
   NumericalTypeGetPropertyMethod getMethod;
   NumericalTypeSetPropertyMethod setMethod;
   UpdateMethod updateAfterWrite;
   bool useGetSetMethods;

   IncrementType incrType;
   NumericalType minValue;
   NumericalType maxValue;
   NumericalType incrValue;

   NumericalClassProperty() {}
 public:
   NumericalClassProperty(const string CName, const Version introducedInVersion,
                          NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod);

   NumericalClassProperty(const string CName, const Version introducedInVersion,
                          pointer_diff offset, UpdateMethod updateAfterWrite = NULL);

   inline void setMinimumValue(NumericalType minValue);
   inline void setMaximumValue(NumericalType maxValue);
   inline void setIncrement(NumericalType incrValue, IncrementType incrType);


   inline NumericalType getMinimumValue() const;
   inline NumericalType getMaximumValue() const;
   inline NumericalType getIncrementValue() const;
   inline IncrementType getIncrementType() const {return incrType;}

   virtual void setMinimumValueDouble( const NumericalClassPropertyBase::FloatType &value );
   virtual void setMaximumValueDouble( const NumericalClassPropertyBase::FloatType &value );
   virtual NumericalClassPropertyBase::FloatType getMinimumValueDouble() const;
   virtual NumericalClassPropertyBase::FloatType getMaximumValueDouble() const;

   virtual NumericalClassPropertyBase::FloatType getIncrementValueDouble() const;

           NumericalType getValue(const Object *obj, pointer_diff offset) const;
           void setValue(Object *obj, pointer_diff offset, NumericalType v) const;

   virtual NumericalClassPropertyBase::FloatType getValueDouble(const Object *obj, pointer_diff offset) const;

   virtual void setValueDouble(Object *obj, pointer_diff offset, NumericalClassPropertyBase::FloatType v) const;

   virtual void writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const;
   virtual void readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const;
   virtual string toString(const Object* obj, pointer_diff offset) const;
   virtual void fromString(Object* obj, pointer_diff offset, const string str) const;
   virtual void assignProperty(const Object *from, Object *to, pointer_diff offset) const;
};


class  Card8ClassProperty : public NumericalClassProperty<card8> {
 private:
   Card8ClassProperty() {}
 public:
   Card8ClassProperty(const string CName, const Version introducedInVersion,
                      NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod);
   Card8ClassProperty(const string CName, const Version introducedInVersion,
                          pointer_diff offset, UpdateMethod updateAfterWrite = NULL);
   virtual const string getTypeName() const;
   virtual ClassProperty* copyClassProperty();
};


class  Card16ClassProperty : public NumericalClassProperty<card16> {
 private:
   Card16ClassProperty() {}
 public:
   Card16ClassProperty(const string CName, const Version introducedInVersion,
                       NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod);
   Card16ClassProperty(const string CName, const Version introducedInVersion,
                       pointer_diff offset, UpdateMethod updateAfterWrite = NULL);
   virtual const string getTypeName() const;
   virtual ClassProperty* copyClassProperty();
};


class  Card32ClassProperty : public NumericalClassProperty<card32> {
 public:
   typedef string (*GetEnumDescriptionMethod)(card32);

 private:
   Card32ClassProperty() {}
   GetEnumDescriptionMethod getEnumDescription;
   card32 numEnumItems;

 public:
   Card32ClassProperty(const string CName, const Version introducedInVersion,
                       NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod,
                       GetEnumDescriptionMethod getEnumDescription = NULL, card32 numEnumItems = 0);

   Card32ClassProperty(const string CName, const Version introducedInVersion,
                       pointer_diff offset, UpdateMethod updateAfterWrite = NULL,
                       GetEnumDescriptionMethod getEnumDescription = NULL, card32 numEnumItems = 0);

   virtual const string getTypeName() const;
   virtual ClassProperty* copyClassProperty();
   virtual string toString(const Object* obj, pointer_diff offset) const;
   virtual void fromString(Object* obj, pointer_diff offset, const string str) const;

   inline card32 getNumEnumItems() {return numEnumItems;}
   string publicGetEnumDescription(card32 i);
};


class  Int8ClassProperty : public NumericalClassProperty<int8> {
 private:
   Int8ClassProperty() {}
 public:
   Int8ClassProperty(const string CName, const Version introducedInVersion,
                     NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod);

   Int8ClassProperty(const string CName, const Version introducedInVersion,
                     pointer_diff offset, UpdateMethod updateAfterWrite = NULL);

   virtual const string getTypeName() const;
   virtual ClassProperty* copyClassProperty();
};

class  Int16ClassProperty : public NumericalClassProperty<int16> {
 private:
   Int16ClassProperty() {}
 public:
   Int16ClassProperty(const string CName, const Version introducedInVersion,
                      NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod);

   Int16ClassProperty(const string CName, const Version introducedInVersion,
                     pointer_diff offset, UpdateMethod updateAfterWrite = NULL);

   virtual const string getTypeName() const;
   virtual ClassProperty* copyClassProperty();
};

class  Int32ClassProperty : public NumericalClassProperty<int32> {
 private:
   Int32ClassProperty() {}
 public:
   Int32ClassProperty(const string CName, const Version introducedInVersion,
                      NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod);

   Int32ClassProperty(const string CName, const Version introducedInVersion,
                     pointer_diff offset, UpdateMethod updateAfterWrite = NULL);

   virtual const string getTypeName() const;
   virtual ClassProperty* copyClassProperty();
};

class  Int64ClassProperty : public NumericalClassProperty<int64> {
 private:
   Int64ClassProperty() {}
 public:
   Int64ClassProperty(const string CName, const Version introducedInVersion,
                      NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod);

   Int64ClassProperty(const string CName, const Version introducedInVersion,
                     pointer_diff offset, UpdateMethod updateAfterWrite = NULL);

   virtual const string getTypeName() const;
   virtual ClassProperty* copyClassProperty();
};

class  Card64ClassProperty : public NumericalClassProperty<card64> {
 private:
   Card64ClassProperty() {}
 public:
   Card64ClassProperty(const string CName, const Version introducedInVersion,
                      NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod);

   Card64ClassProperty(const string CName, const Version introducedInVersion,
                     pointer_diff offset, UpdateMethod updateAfterWrite = NULL);

   virtual const string getTypeName() const;
   virtual ClassProperty* copyClassProperty();
};

class  Float32ClassProperty : public NumericalClassProperty<float32> {
 private:
   Float32ClassProperty() {}
 public:
   Float32ClassProperty(const string CName, const Version introducedInVersion,
                        NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod);

   Float32ClassProperty(const string CName, const Version introducedInVersion,
                        pointer_diff offset, UpdateMethod updateAfterWrite = NULL);

   virtual const string getTypeName() const;
   virtual string toString(const Object* obj, pointer_diff offset) const;
   virtual void fromString(Object* obj, pointer_diff offset, const string str) const;
   virtual ClassProperty* copyClassProperty();
};

class  Float64ClassProperty : public NumericalClassProperty<float64> {
 private:
   Float64ClassProperty() {}
 public:
   Float64ClassProperty(const string CName, const Version introducedInVersion,
                        NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod);

   Float64ClassProperty(const string CName, const Version introducedInVersion,
                        pointer_diff offset, UpdateMethod updateAfterWrite = NULL);

   virtual const string getTypeName() const;
   virtual string toString(const Object* obj, pointer_diff offset) const;
   virtual ClassProperty* copyClassProperty();
};

class  MaxPlatformIntClassProperty : public NumericalClassProperty<mpint> {
 private:
   MaxPlatformIntClassProperty() {}
 public:
   MaxPlatformIntClassProperty(const string CName, const Version introducedInVersion,
                       NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod)
   : NumericalClassProperty<mpint>(CName, introducedInVersion, getMethod, setMethod) {}

   MaxPlatformIntClassProperty(const string CName, const Version introducedInVersion,
                       pointer_diff offset, UpdateMethod updateAfterWrite = NULL)
   : NumericalClassProperty<mpint>(CName, introducedInVersion, offset, updateAfterWrite) {}

   virtual void writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const;
   virtual void readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const;

   virtual const string getTypeName() const;
   virtual ClassProperty* copyClassProperty();
};

class  MaxPlatformCardClassProperty : public NumericalClassProperty<mpcard> {
private:
   MaxPlatformCardClassProperty() {}
 public:
   MaxPlatformCardClassProperty(const string CName, const Version introducedInVersion,
                       NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod)
   : NumericalClassProperty<mpcard>(CName, introducedInVersion, getMethod, setMethod) {}

   MaxPlatformCardClassProperty(const string CName, const Version introducedInVersion,
                       pointer_diff offset, UpdateMethod updateAfterWrite = NULL)
   : NumericalClassProperty<mpcard>(CName, introducedInVersion, offset, updateAfterWrite) {}

   virtual void writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const;
   virtual void readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const;

   virtual const string getTypeName() const;
   virtual ClassProperty* copyClassProperty();
};


typedef Card8ClassProperty::NumericalTypeSetPropertyMethod SetCard8PropMethod;
typedef Card8ClassProperty::NumericalTypeGetPropertyMethod GetCard8PropMethod;
typedef Int8ClassProperty::NumericalTypeSetPropertyMethod SetInt8PropMethod;
typedef Int8ClassProperty::NumericalTypeGetPropertyMethod GetInt8PropMethod;
typedef Card16ClassProperty::NumericalTypeSetPropertyMethod SetCard16PropMethod;
typedef Card16ClassProperty::NumericalTypeGetPropertyMethod GetCard16PropMethod;
typedef Int16ClassProperty::NumericalTypeSetPropertyMethod SetInt16PropMethod;
typedef Int16ClassProperty::NumericalTypeGetPropertyMethod GetInt16PropMethod;
typedef Card32ClassProperty::NumericalTypeSetPropertyMethod SetCard32PropMethod;
typedef Card32ClassProperty::NumericalTypeGetPropertyMethod GetCard32PropMethod;
typedef Int32ClassProperty::NumericalTypeSetPropertyMethod SetInt32PropMethod;
typedef Int32ClassProperty::NumericalTypeGetPropertyMethod GetInt32PropMethod;
typedef Card64ClassProperty::NumericalTypeSetPropertyMethod SetCard64PropMethod;
typedef Card64ClassProperty::NumericalTypeGetPropertyMethod GetCard64PropMethod;
typedef Int64ClassProperty::NumericalTypeSetPropertyMethod SetInt64PropMethod;
typedef Int64ClassProperty::NumericalTypeGetPropertyMethod GetInt64PropMethod;
typedef Float32ClassProperty::NumericalTypeSetPropertyMethod SetFloat32PropMethod;
typedef Float32ClassProperty::NumericalTypeGetPropertyMethod GetFloat32PropMethod;
typedef Float64ClassProperty::NumericalTypeSetPropertyMethod SetFloat64PropMethod;
typedef Float64ClassProperty::NumericalTypeGetPropertyMethod GetFloat64PropMethod;
typedef MaxPlatformCardClassProperty::NumericalTypeGetPropertyMethod GetMaxPlatformCardPropMethod;
typedef MaxPlatformCardClassProperty::NumericalTypeSetPropertyMethod SetMaxPlatformCardPropMethod;
typedef MaxPlatformIntClassProperty::NumericalTypeGetPropertyMethod GetMaxPlatformIntPropMethod;
typedef MaxPlatformIntClassProperty::NumericalTypeSetPropertyMethod SetMaxPlatformIntPropMethod;

typedef Card8ClassProperty::UpdateMethod Card8UpdateMethod;
typedef Card16ClassProperty::UpdateMethod Card16UpdateMethod;
typedef Card32ClassProperty::UpdateMethod Card32UpdateMethod;
typedef Card64ClassProperty::UpdateMethod Card64UpdateMethod;
typedef Int8ClassProperty::UpdateMethod Int8UpdateMethod;
typedef Int16ClassProperty::UpdateMethod Int16UpdateMethod;
typedef Int32ClassProperty::UpdateMethod Int32UpdateMethod;
typedef Int64ClassProperty::UpdateMethod Int64UpdateMethod;
typedef Float32ClassProperty::UpdateMethod Float32UpdateMethod;
typedef Float64ClassProperty::UpdateMethod Float64UpdateMethod;
typedef MaxPlatformCardClassProperty::UpdateMethod MaxPlatformCardUpdateMethod;
typedef MaxPlatformIntClassProperty::UpdateMethod MaxPlatformIntUpdateMethod;

#define ADD_CARD8_PROP_UPDATE( propName, version, updateMethodPtr ) \
  ((MetaClass*)getClass())->addProperty( new Card8ClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
  (pointer_diff)(&( obj_null_pointer->propName ) - NULL), (Card8UpdateMethod) updateMethodPtr ) );

#define ADD_CARD16_PROP_UPDATE( propName, version, updateMethodPtr) \
  ((MetaClass*)getClass())->addProperty( new Card16ClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
  (pointer_diff)(&( obj_null_pointer->propName ) - NULL), (Card16UpdateMethod) updateMethodPtr ) );

#define ADD_CARD32_PROP_UPDATE( propName, version, updateMethodPtr ) \
  ((MetaClass*)getClass())->addProperty( new Card32ClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
  (pointer_diff)(&( obj_null_pointer->propName ) - NULL), (Card32UpdateMethod) updateMethodPtr ) );

#define ADD_CARD64_PROP_UPDATE( propName, version, updateMethodPtr ) \
  ((MetaClass*)getClass())->addProperty( new Card64ClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
  (pointer_diff)(&( obj_null_pointer->propName ) - NULL), (Card64UpdateMethod) updateMethodPtr ) );

#define ADD_INT8_PROP_UPDATE( propName, version, updateMethodPtr ) \
  ((MetaClass*)getClass())->addProperty( new Int8ClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
  (pointer_diff)(&( obj_null_pointer->propName ) - NULL), (Int8UpdateMethod) updateMethodPtr ) );

#define ADD_INT16_PROP_UPDATE( propName, version, updateMethod ) \
  ((MetaClass*)getClass())->addProperty( new Int16ClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
  (pointer_diff)(&( obj_null_pointer->propName ) - NULL), (Int16UpdateMethod) updateMethodPtr ) );

#define ADD_INT32_PROP_UPDATE( propName, version, updateMethodPtr ) \
  ((MetaClass*)getClass())->addProperty( new Int32ClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
  (pointer_diff)(&( obj_null_pointer->propName ) - NULL), (Int32UpdateMethod) updateMethodPtr ) );

#define ADD_INT64_PROP_UPDATE( propName, version, updateMethod ) \
  ((MetaClass*)getClass())->addProperty( new Int64ClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
  (pointer_diff)(&( obj_null_pointer->propName ) - NULL), (Int64UpdateMethod) updateMethodPtr ) );

#define ADD_FLOAT32_PROP_UPDATE( propName, version, updateMethodPtr ) \
  ((MetaClass*)getClass())->addProperty( new Float32ClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
  (pointer_diff)(&( obj_null_pointer-> propName ) - NULL), (Float32UpdateMethod) updateMethodPtr ) );

#define ADD_FLOAT64_PROP_UPDATE( propName, version, updateMethodPtr ) \
  ((MetaClass*)getClass())->addProperty( new Float64ClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
  (pointer_diff)(&( obj_null_pointer->propName ) - NULL), (Float64UpdateMethod)updateMethodPtr ) );

#define ADD_MAX_PLATFORM_CARD_PROP_UPDATE( propName, version, updateMethodPtr ) \
  ((MetaClass*)getClass())->addProperty( new MaxPlatformCardClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
  (pointer_diff)(&( obj_null_pointer->propName ) - NULL), (MaxPlatformCardUpdateMethod) updateMethodPtr ) );

#define ADD_MAX_PLATFORM_INT_PROP_UPDATE( propName, version, updateMethodPtr ) \
  ((MetaClass*)getClass())->addProperty( new MaxPlatformIntClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
  (pointer_diff)(&( obj_null_pointer->propName ) - NULL), (MaxPlatformIntUpdateMethod)updateMethodPtr ) );


#define ADD_CARD8_PROP( propName, version ) \
  ADD_CARD8_PROP_UPDATE( propName, version, NULL )

#define ADD_CARD16_PROP( propName, version ) \
  ADD_CARD16_PROP_UPDATE( propName, version, NULL )

#define ADD_CARD32_PROP( propName, version ) \
  ADD_CARD32_PROP_UPDATE( propName, version, NULL )

#define ADD_CARD64_PROP( propName, version ) \
  ADD_CARD64_PROP_UPDATE( propName, version, NULL )

#define ADD_INT8_PROP( propName, version ) \
  ADD_INT8_PROP_UPDATE( propName, version, NULL )

#define ADD_INT16_PROP( propName, version ) \
  ADD_INT16_PROP_UPDATE( propName, version, NULL )

#define ADD_INT32_PROP( propName, version ) \
  ADD_INT32_PROP_UPDATE( propName, version, NULL )

#define ADD_INT64_PROP( propName, version ) \
  ADD_INT64_PROP_UPDATE( propName, version, NULL )

#define ADD_FLOAT32_PROP( propName, version ) \
  ADD_FLOAT32_PROP_UPDATE( propName, version, NULL )

#define ADD_FLOAT64_PROP( propName, version ) \
  ADD_FLOAT64_PROP_UPDATE( propName, version, NULL )

#define ADD_MAX_PLATFORM_CARD_PROP( propName, version ) \
  ADD_MAX_PLATFORM_CARD_PROP_UPDATE( propName, version, NULL )

#define ADD_MAX_PLATFORM_INT_PROP( propName, version ) \
  ADD_MAX_PLATFORM_INT_PROP_UPDATE( propName, version, NULL )

#define ADD_CARD32_PROP_METHOD( propName, version, get, set ) \
  ((MetaClass*)getClass())->addProperty( new Card32ClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
  (GetCard32PropMethod) get, (SetCard32PropMethod) set) );

#define ADD_FLOAT32_PROP_METHOD( propName, version, get, set ) \
  ((MetaClass*)getClass())->addProperty( new Float32ClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
  (GetFloat32PropMethod) get, (SetFloat32PropMethod) set) );



typedef Card32ClassProperty::GetEnumDescriptionMethod GetCard32EnumDescriptionMethod;

#define ADD_CARD32_PROP_METHOD_DESCR( propName, version, get, set, descr, numDescr ) \
  ((MetaClass*)getClass())->addProperty( new Card32ClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
  (GetCard32PropMethod) get, (SetCard32PropMethod) set, (GetCard32EnumDescriptionMethod) descr, numDescr) );

#define ADD_CARD32_PROP_UPDATE_DESCR( propName, version, updateMethodPtr, descr, numDescr ) \
  ((MetaClass*)getClass())->addProperty( new Card32ClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
  (pointer_diff)(&( obj_null_pointer->propName ) - NULL), (Card32UpdateMethod) updateMethodPtr, (GetCard32EnumDescriptionMethod) descr, numDescr) );

//#include "NumericalClassProperty.hpp"



#endif
