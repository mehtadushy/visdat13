//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ObjectViewsTable.h"
//---------------------------------------------------------------------------
#include "Editor.h"
#include "Exceptions.h"
#include "Tools.h"
#include "HashTables.hpp"
//---------------------------------------------------------------------------



ObjectViewsTable::ObjHashTable ObjectViewsTable::objectsInViews;
bool ObjectViewsTable::hasBeenShutdown = false;

void ObjectViewsTable::update(Object* changedObj) {
   if (hasBeenShutdown) return;
   ViewList *vl = NULL;
   objectsInViews.getData(changedObj, vl);
   if (vl == NULL) return;
   ViewList::iterator pos = vl->begin();
   ViewList::iterator end = vl->end();
   while (pos != end) {
      (*pos)->update();
      ++pos;
   }
}

void ObjectViewsTable::bindView(Object* obj, Editor* objView) {
   if (hasBeenShutdown) return;
   ViewList *vl = NULL;
   objectsInViews.getData(obj, vl);
   if (vl == NULL) {
      vl = new ViewList;
      objectsInViews.addElement(obj, vl);
   }
   vl->push_back(objView);
}

void ObjectViewsTable::releaseView(Object* obj, Editor* objView) {
   if (hasBeenShutdown) return;
   ViewList *vl = NULL;
   objectsInViews.getData(obj, vl);
   if (vl == NULL) {
      throw EInvalidState("ObjectViewsTable::releaseView - obj not in hash table.");
   }
   bool found = false;
   ViewList::iterator pos = vl->begin();
   ViewList::iterator end = vl->end();
   while (pos != end) {
      if (*pos == objView) {
         ViewList::iterator npos = pos;
         ++npos;
         vl->erase(pos);
         pos = npos;
         found = true;
      } else {
         ++pos;
      }
   }
   if (!found)
      throw EInvalidState("ObjectViewsTable::releaseView - objView not in list.");
}

void ObjectViewsTable::deleteEntries(Object* obj, ViewList* vl) {
   delete vl;
   forget(obj);
}

void ObjectViewsTable::shutdown() {
   if (hasBeenShutdown) return;
   objectsInViews.forEachEntry(deleteEntries);
   hasBeenShutdown = true;
}

