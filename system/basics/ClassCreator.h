//---------------------------------------------------------------------------
#ifndef ClassCreatorH
#define ClassCreatorH
//---------------------------------------------------------------------------
#include "Object.h"
//---------------------------------------------------------------------------



/// Object factory: creates new objects according to interal parameters. Typically used for interactive setup of new object instances.
/// Use this class' default class editor to edit internal setup parameters interactively prior to creating a new object.
class  ClassCreator : public Object {
 private:
   static MetaClass *ClassCreatorMetaClass;
 public:
   virtual const MetaClass *getInstanceClass() const;
   static const MetaClass *getClass();
   static void init(const MetaClass *parentClass);
   static void shutdown();

   /// create new object according to current creation settings (use this class' default class editor for interactive setup)
   virtual Object *makeObject() = 0;
   /// macro: show display creation dialog and return new object if OK was selected (NULL otherwise)
   static Object *interactivelyCreateObject(ClassCreator *cc);
};



#endif
