//---------------------------------------------------------------------------
#ifndef ObjectViewsTableH
#define ObjectViewsTableH
//---------------------------------------------------------------------------
#include "HashTables.h"
//---------------------------------------------------------------------------



class Editor;
class Object;

class  ObjectViewsTable {
 private:
   class Comperator {
     public:
       static bool equals(Object* obj1, Object* obj2) {return obj1 == obj2;}
   };
   typedef list<Editor*> ViewList;

   class ObjHashTable : public HashTable<Object*, ViewList*, Comperator> {
    public:
      virtual max_platform_card hashFunction(Object* obj) const {
         return (max_platform_card)obj;
      }
   };

   static ObjHashTable objectsInViews;
   static bool hasBeenShutdown;

   static void deleteEntries(Object* obj, ViewList* vl);

 public:
   static void update(Object* changedObj);
   static void bindView(Object* obj, Editor* objView);
   static void releaseView(Object* obj, Editor* objView);
   static void shutdown();
};



#endif
