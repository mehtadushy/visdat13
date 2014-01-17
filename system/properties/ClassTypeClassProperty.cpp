//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ClassTypeClassProperty.h"
//---------------------------------------------------------------------------
#include "Tools.h"
#include "ObjectStreams.h"
#include "MetaClass.h"
#include "ClassTypePropertyEditor.h"
//---------------------------------------------------------------------------


static void ClassTypeClassProperty_addDefaultEditor(ClassProperty *c) {
	c->addPropertyEditor(ClassTypePropertyEditor::getClass(), true);
}


ClassTypeClassProperty::ClassTypeClassProperty(const string CName, const Version introducedInVersion,
                       pointer_diff offset, const MetaClass *baseClass) : 
SingleClassProperty(CName, introducedInVersion)
{
   this->baseClass = baseClass;
	setOffset(offset);
	ClassTypeClassProperty_addDefaultEditor(this);
}

const MetaClass* ClassTypeClassProperty::getValue(const Object *obj, pointer_diff offset) const {
   return *((MetaClass **)((char *)obj + getOffset() + offset));
}

void ClassTypeClassProperty::setValue(Object *obj, pointer_diff offset, const MetaClass *v) const {
   *((MetaClass **)((char *)obj + getOffset() + offset)) = (MetaClass *)v;
}

const MetaClass *ClassTypeClassProperty::getBaseClass() const {
   return baseClass;
}

void ClassTypeClassProperty::setBaseClass(const MetaClass *baseClass) {
   this->baseClass = baseClass;
}

void ClassTypeClassProperty::writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const {
   out->writeMetaClass(getValue(obj,offset));
}

void ClassTypeClassProperty::readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const {
   const MetaClass *c;
   in->readMetaClass(c);
   setValue( obj,offset, c );
}

string ClassTypeClassProperty::toString(const Object* obj, pointer_diff offset) const {
   return (getValue(obj,offset))->getClassName();
}

void ClassTypeClassProperty::fromString(Object* obj, pointer_diff offset, const string str) const {
   notImplemented();
   forget(obj);
   forget(str);
}

const string ClassTypeClassProperty::getTypeName() const {
   return string("class of ") + baseClass->getClassName();
}

ClassProperty* ClassTypeClassProperty::copyClassProperty() {
   ClassTypeClassProperty *result = new ClassTypeClassProperty();
   *result = *this;
   return result;
}

void ClassTypeClassProperty::assignProperty(const Object *from, Object *to, pointer_diff offset) const {
   setValue(to,offset, getValue(from,offset));
}

