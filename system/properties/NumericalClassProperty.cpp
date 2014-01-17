//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "NumericalClassProperty.h"
//---------------------------------------------------------------------------
#include "StringPropertyEditor.h"
#include "EnumerationPropertyEditor.h"
#include "StringHelper.h"
#include "ObjectStreams.h"
//---------------------------------------------------------------------------
#ifdef _WIN32
   #pragma warning(disable:4996)
#endif
//---------------------------------------------------------------------------
#ifdef _WIN32
   #include <values_vs.h>
#else
static void itoa(int value, std::string& buf, int base)
{
        int i = 30;
        buf = "";
        for(; value && i ; --i, value /= base)
		buf = "0123456789abcdef"[value % base] + buf;
}

static char* itoa(int val, char *buf, int base)
{
//	static char buf[32] = {0};
	int i = 30;

	buf[i+1]=0;
	for(; val && i ; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];

	return &buf[i+1];
}

/* WORKAROUND FOR MAC OS X - MISSING VALUES.H (DEPRECATED) */
#ifdef __APPLE__
#include <float.h>
#define MINFLOAT FLT_MIN
#define MAXFLOAT FLT_MAX
#define MINDOUBLE DBL_MIN
#define MAXDOUBLE DBL_MAX
#else
#include <values.h>
#endif
#endif
//---------------------------------------------------------------------------



void NumericalClassProperty__addDefaultEditor(ClassProperty *c) {
   c->addPropertyEditor(StringPropertyEditor::getClass(), true);
}


void MaxPlatformIntClassProperty::writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const {
   mpint buf = getValue(obj,offset);
   out->writeMaxPlatformInt(buf);
}
void MaxPlatformIntClassProperty::readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const {
   mpint buf;
   in->readMaxPlatformInt(buf);
   setValue(obj, offset, buf);
}
void MaxPlatformCardClassProperty::writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const {
   mpcard buf = getValue(obj, offset);
   out->writeMaxPlatformCard(buf);
}
void MaxPlatformCardClassProperty::readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const {
   mpcard buf;
   in->readMaxPlatformCard(buf);
   setValue(obj, offset, buf);
}

const string MaxPlatformCardClassProperty::getTypeName() const {return "max_platform_card";}

ClassProperty* MaxPlatformCardClassProperty::copyClassProperty() {
   MaxPlatformCardClassProperty *result = new MaxPlatformCardClassProperty();
   *result = *this;
   return result;
}

const string MaxPlatformIntClassProperty::getTypeName() const {return "max_platform_int";}

ClassProperty* MaxPlatformIntClassProperty::copyClassProperty() {
   MaxPlatformIntClassProperty *result = new MaxPlatformIntClassProperty();
   *result = *this;
   return result;
}



Card8ClassProperty::Card8ClassProperty(const string CName, const Version introducedInVersion,
                   NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod)
 : NumericalClassProperty<card8>(CName, introducedInVersion, getMethod, setMethod)
{
   minValue = 0;
   maxValue = 255;
}
Card8ClassProperty::Card8ClassProperty(const string CName, const Version introducedInVersion,
                       pointer_diff offset, UpdateMethod updateAfterWrite)
 : NumericalClassProperty<card8>(CName, introducedInVersion, offset, updateAfterWrite)
{
   minValue = 0;
   maxValue = 255;
}


const string Card8ClassProperty::getTypeName() const {
   return "card8";
}

ClassProperty* Card8ClassProperty::copyClassProperty() {
   Card8ClassProperty *result = new Card8ClassProperty();
   *result = *this;
   return result;
}


Card16ClassProperty::Card16ClassProperty(const string CName, const Version introducedInVersion,
                    NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod)
 : NumericalClassProperty<card16>(CName, introducedInVersion, getMethod, setMethod) {
   minValue = 0; maxValue = 65535;
}

Card16ClassProperty::Card16ClassProperty(const string CName, const Version introducedInVersion,
                    pointer_diff offset, UpdateMethod updateAfterWrite)
 : NumericalClassProperty<card16>(CName, introducedInVersion, offset, updateAfterWrite) {
   minValue = 0; maxValue = 65535;
}

const string Card16ClassProperty::getTypeName() const {return "card16";}

ClassProperty* Card16ClassProperty::copyClassProperty() {
   Card16ClassProperty *result = new Card16ClassProperty();
   *result = *this;
   return result;
}


Card32ClassProperty::Card32ClassProperty(const string CName, const Version introducedInVersion,
                    NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod,
                    GetEnumDescriptionMethod getEnumDescription, card32 numEnumItems)
 : NumericalClassProperty<card32>(CName, introducedInVersion, getMethod, setMethod) {
   minValue = 0; maxValue = 0xFFFFFFFF;
   this->getEnumDescription = getEnumDescription;
   this->numEnumItems = numEnumItems;
   if (numEnumItems != 0) {
		addPropertyEditor(EnumerationPropertyEditor::getClass(), true);
	}
}

Card32ClassProperty::Card32ClassProperty(const string CName, const Version introducedInVersion,
                    pointer_diff offset, UpdateMethod updateAfterWrite,
                    GetEnumDescriptionMethod getEnumDescription, card32 numEnumItems)
 : NumericalClassProperty<card32>(CName, introducedInVersion, offset, updateAfterWrite) {
   minValue = 0; maxValue = 0xFFFFFFFF;
   this->getEnumDescription = getEnumDescription;
   this->numEnumItems = numEnumItems;
   if (numEnumItems != 0) {
		addPropertyEditor(EnumerationPropertyEditor::getClass(), true);
	}
}

const string Card32ClassProperty::getTypeName() const {return "card32";}

ClassProperty* Card32ClassProperty::copyClassProperty() {
   Card32ClassProperty *result = new Card32ClassProperty();
   *result = *this;
   return result;
}

string Card32ClassProperty::toString(const Object* obj, pointer_diff offset) const {
   char buf[34];
   if (getEnumDescription != NULL) {
      string descr;
      descr = getEnumDescription(getValue(obj,offset));
      return string(itoa(getValue(obj,offset), buf, 10)) + string(" /* ") + descr + string(" */");
   } else {
      return string(itoa(getValue(obj,offset), buf, 10));
   }
}

void Card32ClassProperty::fromString(Object* obj, pointer_diff offset, const string str) const {
   card32 descrPos = 0;
   card32 l = (card32)str.length() - 1;
   string str2;
   if (l != 0) {
      while (descrPos <= l) {
         if (str[descrPos] == '/') break;
         ++descrPos;
      }
      str2 = str.substr(0, descrPos);
   } else {
      str2 = str;
   }

   card32 v = (card32)(atoi(str2.c_str()));
   if (v > maxValue) v = maxValue;
   if (v < minValue) v = minValue;
   setValue(obj,offset, v);
}

string Card32ClassProperty::publicGetEnumDescription(card32 i) {
   if (getEnumDescription != NULL) {
      return getEnumDescription(i);
	} else {
		return intToStr(i) + string(" - unnamed");
	}	
}




Int8ClassProperty::Int8ClassProperty(const string CName, const Version introducedInVersion,
                  NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod)
 : NumericalClassProperty<int8>(CName, introducedInVersion, getMethod, setMethod) {
   minValue = -128; maxValue = 127;
}

Int8ClassProperty::Int8ClassProperty(const string CName, const Version introducedInVersion,
                  pointer_diff offset, UpdateMethod updateAfterWrite)
 : NumericalClassProperty<int8>(CName, introducedInVersion, offset, updateAfterWrite) {
   minValue = -128; maxValue = 127;
}

const string Int8ClassProperty::getTypeName() const {return "int8";}

ClassProperty* Int8ClassProperty::copyClassProperty() {
   Int8ClassProperty *result = new Int8ClassProperty();
   *result = *this;
   return result;
}


Int16ClassProperty::Int16ClassProperty(const string CName, const Version introducedInVersion,
                   NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod)
 : NumericalClassProperty<int16>(CName, introducedInVersion, getMethod, setMethod) {
   minValue = -0x8000; maxValue = 0x7FFF;
}

Int16ClassProperty::Int16ClassProperty(const string CName, const Version introducedInVersion,
                  pointer_diff offset, UpdateMethod updateAfterWrite)
 : NumericalClassProperty<int16>(CName, introducedInVersion, offset, updateAfterWrite) {
   minValue = -0x8000; maxValue = 0x7FFF;
}

const string Int16ClassProperty::getTypeName() const {return "int16";}
ClassProperty* Int16ClassProperty::copyClassProperty() {
   Int16ClassProperty *result = new Int16ClassProperty();
   *result = *this;
   return result;
}



Int32ClassProperty::Int32ClassProperty(const string CName, const Version introducedInVersion,
                   NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod)
 : NumericalClassProperty<int32>(CName, introducedInVersion, getMethod, setMethod) {
   minValue = -2147483647; maxValue = 0x7FFFFFFF;
}

Int32ClassProperty::Int32ClassProperty(const string CName, const Version introducedInVersion,
                  pointer_diff offset, UpdateMethod updateAfterWrite)
 : NumericalClassProperty<int32>(CName, introducedInVersion, offset, updateAfterWrite) {
   minValue = -2147483647; maxValue = 0x7FFFFFFF;
}

const string Int32ClassProperty::getTypeName() const {return "int32";}

ClassProperty* Int32ClassProperty::copyClassProperty() {
   Int32ClassProperty *result = new Int32ClassProperty();
   *result = *this;
   return result;
}

Int64ClassProperty::Int64ClassProperty(const string CName, const Version introducedInVersion,
                      NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod)
 : NumericalClassProperty<int64>(CName, introducedInVersion, getMethod, setMethod) {
   minValue = -0x7FFFFFFFFFFFFFFFLL; maxValue = 0x7FFFFFFFFFFFFFFFLL;
}

Int64ClassProperty::Int64ClassProperty(const string CName, const Version introducedInVersion,
                  pointer_diff offset, UpdateMethod updateAfterWrite)
 : NumericalClassProperty<int64>(CName, introducedInVersion, offset, updateAfterWrite) {
   minValue = -0x7FFFFFFFFFFFFFFFLL; maxValue = 0x7FFFFFFFFFFFFFFFLL;
}

const string Int64ClassProperty::getTypeName() const {
   return "int64";
}

ClassProperty* Int64ClassProperty::copyClassProperty() {
   Int64ClassProperty *result = new Int64ClassProperty();
   *result = *this;
   return result;
}



Card64ClassProperty::Card64ClassProperty(const string CName, const Version introducedInVersion,
                   NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod)
 : NumericalClassProperty<card64>(CName, introducedInVersion, getMethod, setMethod)
{
   minValue = 0;
   maxValue = 0x7FFFFFFFFFFFFFFFLL;
}

Card64ClassProperty::Card64ClassProperty(const string CName, const Version introducedInVersion,
                  pointer_diff offset, UpdateMethod updateAfterWrite)
 : NumericalClassProperty<card64>(CName, introducedInVersion, offset, updateAfterWrite)
{
   minValue = 0;
   maxValue = 0x7FFFFFFFFFFFFFFFLL;
}

const string Card64ClassProperty::getTypeName() const {
   return "card64";
}

ClassProperty* Card64ClassProperty::copyClassProperty() {
   Card64ClassProperty *result = new Card64ClassProperty();
   *result = *this;
   return result;
}


Float32ClassProperty::Float32ClassProperty(const string CName, const Version introducedInVersion,
                     NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod)
 : NumericalClassProperty<float32>(CName, introducedInVersion, getMethod, setMethod)
{
   minValue = MINFLOAT;
   maxValue = MAXFLOAT;
}

Float32ClassProperty::Float32ClassProperty(const string CName, const Version introducedInVersion,
                     pointer_diff offset, UpdateMethod updateAfterWrite)
 : NumericalClassProperty<float32>(CName, introducedInVersion, offset, updateAfterWrite)
{
   minValue = MINFLOAT;
   maxValue = MAXFLOAT;
}

const string Float32ClassProperty::getTypeName() const {
   return "float32";
}

string Float32ClassProperty::toString(const Object* obj, pointer_diff offset) const {
   char buf[100];
   sprintf(buf, "%g", getValue(obj,offset));
   return string(buf);
}

void Float32ClassProperty::fromString(Object* obj, pointer_diff offset, const string str) const
{
   setValue(obj, offset, (float32)(atof(str.c_str())));
}

ClassProperty* Float32ClassProperty::copyClassProperty()
{
   Float32ClassProperty *result = new Float32ClassProperty();
   *result = *this;
   return result;
}

Float64ClassProperty::Float64ClassProperty(const string CName, const Version introducedInVersion,
                     NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod)
 : NumericalClassProperty<float64>(CName, introducedInVersion, getMethod, setMethod) {
   minValue = MINDOUBLE; maxValue = MAXDOUBLE;
}

Float64ClassProperty::Float64ClassProperty(const string CName, const Version introducedInVersion,
                     pointer_diff offset, UpdateMethod updateAfterWrite)
 : NumericalClassProperty<float64>(CName, introducedInVersion, offset, updateAfterWrite) {
   minValue = MINDOUBLE; maxValue = MAXDOUBLE;
}

const string Float64ClassProperty::getTypeName() const {
   return "float64";
}

string Float64ClassProperty::toString(const Object* obj, pointer_diff offset) const {
   char buf[100];
   sprintf(buf, "%g", getValue(obj,offset));
   return string(buf);
}

ClassProperty* Float64ClassProperty::copyClassProperty() {
   Float64ClassProperty *result = new Float64ClassProperty();
   *result = *this;
   return result;
}


template <class NumericalType>
NumericalClassProperty<NumericalType>::NumericalClassProperty(const string CName, const Version introducedInVersion,
                       NumericalTypeGetPropertyMethod getMethod, NumericalTypeSetPropertyMethod setMethod)
: NumericalClassPropertyBase(CName, introducedInVersion)
{
   this->getMethod = getMethod;
   this->setMethod = setMethod;
   this->updateAfterWrite = NULL;
   useGetSetMethods = true;
	setCompoundAble( false );
   incrType = itMult;
   incrValue = 2;
   NumericalClassProperty__addDefaultEditor(this);
   if (setMethod == NULL) setEditReadonly(true);
}

template <class NumericalType>
NumericalClassProperty<NumericalType>::NumericalClassProperty(const string CName, const Version introducedInVersion,
                       pointer_diff offset, UpdateMethod updateAfterWrite)
: NumericalClassPropertyBase(CName, introducedInVersion)
{
   this->getMethod = NULL;
   this->setMethod = NULL;
   this->updateAfterWrite = updateAfterWrite;
   useGetSetMethods = false;
	setOffset( offset );
   incrType = itMult;
   incrValue = 2;
   NumericalClassProperty__addDefaultEditor(this);
   setEditReadonly(false);
}

template <class NumericalType>
inline void NumericalClassProperty<NumericalType>::setMinimumValue(NumericalType minValue) {
   this->minValue = minValue;
}

template <class NumericalType>
inline void NumericalClassProperty<NumericalType>::setMaximumValue(NumericalType maxValue) {
   this->maxValue = maxValue;
}

template <class NumericalType>
inline void NumericalClassProperty<NumericalType>::setIncrement(NumericalType incrValue, IncrementType incrType) {
   this->incrValue = incrValue;
   this->incrType = incrType;
}


template <class NumericalType>
inline NumericalType NumericalClassProperty<NumericalType>::getMinimumValue() const {
   return minValue;
}

template <class NumericalType>
inline NumericalType NumericalClassProperty<NumericalType>::getMaximumValue() const {
   return maxValue;
}

template <class NumericalType>
inline NumericalType NumericalClassProperty<NumericalType>::getIncrementValue() const {
   return incrValue;
}

/*template <class NumericalType>
inline NumericalClassProperty<NumericalType>::IncrementType NumericalClassProperty<NumericalType>::getIncrementType() const {
   return incrType;
}*/

template <class NumericalType>
void NumericalClassProperty<NumericalType>::setMinimumValueDouble( const NumericalClassPropertyBase::FloatType &value )
{
   setMinimumValue( (NumericalType)value );
}

template <class NumericalType>
void NumericalClassProperty<NumericalType>::setMaximumValueDouble( const NumericalClassPropertyBase::FloatType &value )
{
   setMaximumValue( (NumericalType)value );
}

template <class NumericalType>
NumericalClassPropertyBase::FloatType NumericalClassProperty<NumericalType>::getMinimumValueDouble() const
{
   return (NumericalClassPropertyBase::FloatType)getMinimumValue();
}

template <class NumericalType>
NumericalClassPropertyBase::FloatType NumericalClassProperty<NumericalType>::getMaximumValueDouble() const
{
   return (NumericalClassPropertyBase::FloatType)getMaximumValue();
}

template <class NumericalType>
NumericalClassPropertyBase::FloatType NumericalClassProperty<NumericalType>::getIncrementValueDouble() const
{
   return (NumericalClassPropertyBase::FloatType)getIncrementValue();
}

template <class NumericalType>
inline NumericalType NumericalClassProperty<NumericalType>::getValue(const Object *obj, pointer_diff offset) const {
   if (useGetSetMethods) {
      return (obj->*getMethod)();
   } else {
      return *((NumericalType *)((char*)obj + getOffset() + offset));
   }
}

template <class NumericalType>
inline void NumericalClassProperty<NumericalType>::setValue(Object *obj, pointer_diff offset, NumericalType v) const {
   if (useGetSetMethods) {
      if (setMethod != NULL) {
         (obj->*setMethod)(v);
      }
   } else {
      *((NumericalType *)((char*)obj + getOffset() + offset)) = v;
      if (updateAfterWrite != NULL)
         (obj->*updateAfterWrite)();
   }
}

template <class NumericalType>
NumericalClassPropertyBase::FloatType NumericalClassProperty<NumericalType>::getValueDouble(const Object *obj, pointer_diff offset) const
{
   return (NumericalClassPropertyBase::FloatType)getValue(obj,offset);
}

template <class NumericalType>
void NumericalClassProperty<NumericalType>::setValueDouble(Object *obj, pointer_diff offset, NumericalClassPropertyBase::FloatType v) const
{
   setValue( obj, offset, (NumericalType)v );
}

template <class NumericalType>
void NumericalClassProperty<NumericalType>::writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const {
   NumericalType buf = getValue(obj,offset);
   out->write<NumericalType>(buf);
}

template <class NumericalType>
void NumericalClassProperty<NumericalType>::readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const {
   NumericalType buf;
   in->read<NumericalType>(buf);
   setValue(obj, offset, buf);
}

template <class NumericalType>
string NumericalClassProperty<NumericalType>::toString(const Object* obj, pointer_diff offset) const {
   char buf[34];
   return string(itoa((int)getValue(obj,offset), buf, 10));
}

template <class NumericalType>
void NumericalClassProperty<NumericalType>::fromString(Object* obj, pointer_diff offset, const string str) const {
   NumericalType v = (NumericalType)(atoi(str.c_str()));
   if (v > maxValue) v = maxValue;
   if (v < minValue) v = minValue;
   setValue(obj, offset, v);
}

template <class NumericalType>
void NumericalClassProperty<NumericalType>::assignProperty(const Object *from, Object *to, pointer_diff offset) const {
   setValue(to, offset, getValue(from,offset));
}

template class  NumericalClassProperty<card8>;
template class  NumericalClassProperty<card16>;
template class  NumericalClassProperty<card32>;
template class  NumericalClassProperty<card64>;
template class  NumericalClassProperty<int8>;
template class  NumericalClassProperty<int16>;
template class  NumericalClassProperty<int32>;
template class  NumericalClassProperty<int64>;
template class  NumericalClassProperty<float32>;
template class  NumericalClassProperty<float64>;

