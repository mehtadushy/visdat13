//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ClassEditor.h"
//---------------------------------------------------------------------------


MetaClass* ClassEditor::ClassEditorMetaClass = NULL;

const MetaClass* ClassEditor::getInstanceClass() const {
   return getClass();
}

const MetaClass* ClassEditor::getClass() {
   if (ClassEditorMetaClass==NULL) {
      ClassEditorMetaClass = new MetaClass("ClassEditor", "Base Class for all class editor controller classes.",
                                          NULL, sizeof(ClassEditor));
   }
   return ClassEditorMetaClass;
}

void ClassEditor::init(const MetaClass *parentClass) {
   ((MetaClass*)getClass())->init(parentClass);
   ClassEditorMetaClass->copyParentProperties();
}

void ClassEditor::shutdown() {
   delete ClassEditorMetaClass;
}

ClassEditor::ClassEditor() {
}

