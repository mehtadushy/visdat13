//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ClassInstanceSet.h"
//---------------------------------------------------------------------------
#include "PAssert.h"
//---------------------------------------------------------------------------



void ClassInstanceSet::insertObject(Object *obj) {
   pAssert(obj != NULL);
   const MetaClass *mc = obj->getInstanceClass();
   for (card32 i=0; i<entries.size(); i++) {
      if (entries[i].mc == mc) {
         if (obj != entries[i].obj) {
            delete entries[i].obj;
         }
         entries[i].obj = obj;
         return;
      }
   }
   Entry e;
   e.mc = mc;
   e.obj = obj;
   entries.push_back(e);
}

bool ClassInstanceSet::containsObjectOfClass(const MetaClass *mc) {
   for (card32 i=0; i<entries.size(); i++) {
      if (entries[i].mc == mc) return true;
   }
   return false;
}

Object *ClassInstanceSet::getInstance(const MetaClass *mc) {
   for (card32 i=0; i<entries.size(); i++) {
      if (entries[i].mc == mc) {
         return entries[i].obj;
      }
   }
   return NULL;
}

ClassInstanceSet::~ClassInstanceSet() {
   for (card32 i=0; i<entries.size(); i++) {
      delete entries[i].obj;
   }
}
