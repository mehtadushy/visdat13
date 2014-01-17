//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "Object.h"
//---------------------------------------------------------------------------
#include "Tools.h"
#include "PAssert.h"
//---------------------------------------------------------------------------
#include "MemberBrowser.h"
//---------------------------------------------------------------------------


MetaClass* Object::objectMetaClass = NULL;

const MetaClass* Object::getInstanceClass() const {
   return getClass();
}

const MetaClass* Object::getClass() {
   if (objectMetaClass==NULL) {
      objectMetaClass = new MetaClass("Object", "Base Class for all reflective classes.",
                                      NULL, sizeof(Object));
   }
   return objectMetaClass;
}

void Object::init(const MetaClass *parentClass) {
   ((MetaClass*)getClass())->init( parentClass);
   objectMetaClass->getGUIDescriptor()->addClassEditor(MemberBrowser::getClass());

}

void Object::shutdown() {
   delete objectMetaClass;
}


// ----


Object *CopyContext::getData(string name) {
   const size_t n = tagStack.size();
   if (n == 0) return NULL;
   for (size_t i=n-1; i>0; i--) {
      if (tagStack[i].name == name) return tagStack[i].data;
   }
   if (tagStack[0].name == name) return tagStack[0].data;
   return NULL;
}

void CopyContext::pushData(string name, Object *obj) {
   Entry e;
   e.name = name;
   e.data = obj;
   tagStack.push_back(e);
}

void CopyContext::popData() {
   pAssert(tagStack.size() > 0);
   tagStack.pop_back();
}



void Object::assign(const Object* obj, CopyContext *context) {
   if (obj->getInstanceClass() != this->getInstanceClass()) {
      throw ECannotAssign("Object::assign does not implement assignment between different types.");
   } else {
      memcpy(this, obj, getInstanceClass()->getInstanceSize());
   }
   forget(context);
}

Object* Object::copy(CopyContext *context) const {
   Object* result = getInstanceClass()->newInstance();
   result->assign(this, context);
   return result;
}

bool Object::equal(const Object* obj) const {
   if (obj == NULL) return false;

   const MetaClass *thisMetaClass = getInstanceClass();
   const MetaClass *objMetaClass = obj->getInstanceClass();
   if (thisMetaClass != objMetaClass) return false;

   card32 instanceSize = thisMetaClass->getInstanceSize();
   char *thisPos = (char*)this;
   char *objPos = (char*)obj;

   while (instanceSize > 0) {
      if (*thisPos != *objPos) return false;
      ++thisPos;
      ++objPos;
      --instanceSize;
   }
   return true;
}

card32 Object::getFieldsHashValue() const{
   const MetaClass *thisMetaClass = getInstanceClass();

   card32 hashValue = 0;
   card32 DWordInstanceSize = thisMetaClass->getInstanceSize() / 4;
   card32 *thisPos = (card32*)this;

   while (DWordInstanceSize > 0) {
      hashValue = (hashValue << 1) ^ (*thisPos);
      ++thisPos;
      --DWordInstanceSize;
   }
   return hashValue;
}


