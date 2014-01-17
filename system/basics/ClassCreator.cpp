//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ClassCreator.h"
#include "ObjectCreationDialog.h"
//---------------------------------------------------------------------------

MetaClass* ClassCreator::ClassCreatorMetaClass = NULL;

const MetaClass* ClassCreator::getInstanceClass() const {
   return getClass();
}

const MetaClass* ClassCreator::getClass() {
   if (ClassCreatorMetaClass==NULL) {
      ClassCreatorMetaClass = new MetaClass("ClassCreator", "Base Class for interactive class creation dialog controllers.",
                                          NULL, sizeof(ClassCreator));
   }
   return ClassCreatorMetaClass;
}

void ClassCreator::init(const MetaClass *parentClass) {
   ((MetaClass*)getClass())->init(parentClass);
   ClassCreatorMetaClass->copyParentProperties();
}

void ClassCreator::shutdown() {
   delete ClassCreatorMetaClass;
}


// ----

Object *ClassCreator::interactivelyCreateObject(ClassCreator *cc) {
   return ObjectCreationDialog::execute(qApp->topLevelWidgets()[0], cc);
}
