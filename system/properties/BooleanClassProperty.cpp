//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "BooleanClassProperty.h"
//---------------------------------------------------------------------------
#include "BooleanPropertyEditor.h"
//---------------------------------------------------------------------------
#include "Tools.h"
//---------------------------------------------------------------------------



void BooleanClassProperty::addDefaultEditor(ClassProperty *c) {
	c->addPropertyEditor(BooleanPropertyEditor::getClass(), true);
}

BooleanClassProperty::BooleanClassProperty(const string CName, const Version introducedInVersion,
                       BooleanGetPropertyMethod getMethod, BooleanSetPropertyMethod setMethod)
: SingleClassProperty(CName, introducedInVersion) {
   this->getMethod = getMethod;
   this->setMethod = setMethod;
   useGetSetMethods = true;
	setCompoundAble( false );
   addDefaultEditor(this);
}

BooleanClassProperty::BooleanClassProperty(const string CName, const Version introducedInVersion,
                        pointer_diff offset)
: SingleClassProperty(CName, introducedInVersion) {
   this->getMethod = NULL;
   this->setMethod = NULL;
	setOffset( offset );
   useGetSetMethods = false;
   addDefaultEditor(this);
}

bool BooleanClassProperty::getValue(const Object *obj, pointer_diff offset) const {
   if (useGetSetMethods) {
      return (obj->*getMethod)();
   } else {
      return *((bool *)((char *)obj + getOffset() + offset));
   }
}

void   BooleanClassProperty::setValue(Object *obj, pointer_diff offset, bool v) const {
   if (useGetSetMethods) 
	{
		if( setMethod != NULL )
			(obj->*setMethod)(v);
   } else {
      *((bool *)((char *)obj + getOffset() + offset)) = v;
   }
}

void BooleanClassProperty::writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const {
	out->writeBool( getValue( obj,offset ) );

}

void BooleanClassProperty::readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const {
   bool v;
   in->readBool(v);
	setValue( obj, offset, v );
}

string BooleanClassProperty::toString(const Object* obj, pointer_diff offset) const {
   bool b = getValue( obj, offset );
   if (b) {
      return "true";
   } else {
      return "false";
   }
}

void BooleanClassProperty::fromString(Object* obj, pointer_diff offset, const string str) const {
   bool b;
   if (str == "true" || str == "True" || str == "1" || str == "TRUE") {
      b = true;
   } else {
      b = false;
   }
	setValue( obj, offset, b );
}

const string BooleanClassProperty::getTypeName() const {
   return "bool";
}

void BooleanClassProperty::assignProperty(const Object *from, Object *to, pointer_diff offset) const {
   setValue(to, offset, getValue(from,offset));
}

ClassProperty* BooleanClassProperty::copyClassProperty() {
   BooleanClassProperty *result = new BooleanClassProperty();
   *result = *this;
   return result;
}

