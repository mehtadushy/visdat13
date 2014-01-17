//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "StringClassProperty.h"
//---------------------------------------------------------------------------
#include "StringPropertyEditor.h"
//---------------------------------------------------------------------------
#include "Tools.h"
//---------------------------------------------------------------------------



void StringClassProperty::addDefaultEditor(ClassProperty *c) {
   c->addPropertyEditor(StringPropertyEditor::getClass(), true);
}

StringClassProperty::StringClassProperty(const string CName, const Version introducedInVersion,
                       StringGetPropertyMethod getMethod, StringSetPropertyMethod setMethod)
: SingleClassProperty(CName, introducedInVersion) {
   this->getMethod = getMethod;
   this->setMethod = setMethod;
   useGetSetMethods = true;
	setCompoundAble( false );
   addDefaultEditor(this);
}

StringClassProperty::StringClassProperty(const string CName, const Version introducedInVersion,
                                         pointer_diff offset)
: SingleClassProperty(CName, introducedInVersion) {
   this->getMethod = NULL;
   this->setMethod = NULL;
	setOffset( offset );
   useGetSetMethods = false;
   addDefaultEditor(this);
}

string StringClassProperty::getValue(const Object *obj, pointer_diff offset) const {
   if (useGetSetMethods) 
	{
      return (obj->*getMethod)();
   } 
	else 
	{
      return *((string *)((char *)obj + getOffset() + offset));
   }
}

void   StringClassProperty::setValue(Object *obj, pointer_diff offset, string v) const {
   if (useGetSetMethods) 
	{
		if( setMethod != NULL )
			(obj->*setMethod)(v);
   } 
	else 
	{
      *((string *)((char *)obj + getOffset() + offset)) = v;
   }
}

void StringClassProperty::writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const {
	out->writeString( getValue( obj,offset ) );
}

void StringClassProperty::readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const {
   string s;
   in->readString(s);
	setValue( obj,offset, s );
}

string StringClassProperty::toString(const Object* obj, pointer_diff offset) const {
	return getValue(obj,offset);
}

void StringClassProperty::fromString(Object* obj, pointer_diff offset, const string str) const {
	setValue( obj,offset, str );
}

const string StringClassProperty::getTypeName() const {
   return "string";
}

void StringClassProperty::assignProperty(const Object *from, Object *to, pointer_diff offset) const {
   setValue(to,offset, getValue(from,offset));
}


ClassProperty* StringClassProperty::copyClassProperty() {
   StringClassProperty *result = new StringClassProperty();
   *result = *this;
   return result;
}

