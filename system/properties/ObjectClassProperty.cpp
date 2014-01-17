//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ObjectClassProperty.h"
//---------------------------------------------------------------------------
#include "ObjectPropertyEditor.h"
#include "Tools.h"
#include "ObjectStreams.h"
#include "MetaClass.h"
#include "Persistent.h"
//---------------------------------------------------------------------------



void ObjectClassProperty::addDefaultEditor(ClassProperty *c) {
	c->addPropertyEditor(ObjectPropertyEditor::getClass(), true);
}

ObjectClassProperty::ObjectClassProperty(const string CName, const Version introducedInVersion,
                       ObjectGetPropertyMethod getMethod, ObjectSetPropertyMethod setMethod,
                       const MetaClass *baseClass, bool isOwner)
: SingleClassProperty(CName, introducedInVersion) {
   this->getMethod = getMethod;
   this->setMethod = setMethod;
   this->updateAfterWrite = NULL;
   this->baseClass = baseClass;
   this->owner = isOwner;
   useGetSetMethods = true;
	setCompoundAble( false );
   addDefaultEditor(this);
}
ObjectClassProperty::ObjectClassProperty(const string CName, const Version introducedInVersion,
                       pointer_diff offset, const MetaClass *baseClass, bool isOwner,UpdateMethod updateAfterWrite)
: SingleClassProperty(CName, introducedInVersion) {
   this->getMethod = NULL;
   this->setMethod = NULL;
   this->updateAfterWrite = updateAfterWrite;
   this->baseClass = baseClass;
   this->owner = isOwner;
	setOffset( offset );
   useGetSetMethods = false;
   addDefaultEditor(this);
}

const MetaClass *ObjectClassProperty::getBaseClass() const {
   return baseClass;
}

bool ObjectClassProperty::getObjectIsOwnerOfSubobject() const {
   return owner;
}

void ObjectClassProperty::setObjectIsOwnerOfSubobject(bool isOwner) {
   owner = isOwner;
}

void ObjectClassProperty::setBaseClass(const MetaClass *baseClass) {
   this->baseClass = baseClass;
}

Object *ObjectClassProperty::getValue(const Object *obj, pointer_diff offset) const {
   if (useGetSetMethods) {
      return (obj->*getMethod)();
   } else {
      return *((Object **)((char *)obj + getOffset() + offset));
   }
}

void   ObjectClassProperty::setValue(Object *obj, pointer_diff offset, Object *v) const {
   if (useGetSetMethods) {
      (obj->*setMethod)(v);
   } else {
      *((Object **)((char *)obj + getOffset() + offset)) = v;
      if (updateAfterWrite != NULL) {
         (obj->*updateAfterWrite)();
      }
   }
}

void ObjectClassProperty::writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const {
   out->writeObject((Persistent*)getValue(obj,offset) );
}

void ObjectClassProperty::readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const {
   Persistent* inobj;
   in->readObject(inobj);
	setValue( obj, offset, inobj );
}

string ObjectClassProperty::toString(const Object* obj, pointer_diff offset) const {
	return makeObjectDescription( getValue( obj,offset ) );
}

void ObjectClassProperty::fromString(Object* obj, pointer_diff offset, const string str) const {
   forget(obj);
   forget(str);
	forget(offset);
}

const string ObjectClassProperty::getTypeName() const {
   return baseClass->getClassName() + string("*");
}

ClassProperty* ObjectClassProperty::copyClassProperty() {
   ObjectClassProperty *result = new ObjectClassProperty();
   *result = *this;
   return result;
}

void ObjectClassProperty::assignProperty(const Object *from, Object *to, pointer_diff offset) const {
   if (owner && !useGetSetMethods ) {
      delete getValue(to,offset);
      setValue(to, offset, NULL);
   }
   if (getObjectIsOwnerOfSubobject()) {
		if (getValue(from, offset) != NULL) {
	      setValue(to, offset, getValue(from, offset)->copy());
	   } else {
	      setValue(to, offset, NULL);
	   }
   } else {
      setValue(to, offset, getValue(from ,offset));
   }
}

