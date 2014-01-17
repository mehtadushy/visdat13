//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "PropertyEditor.h"
//---------------------------------------------------------------------------


MetaClass* PropertyEditor::PropertyEditorMetaClass = NULL;


PropertyEditor::PropertyEditor() {
	m_Object = NULL;
	m_Property = NULL;
}                        

const MetaClass* PropertyEditor::getInstanceClass() const {
   return getClass();
}

const MetaClass* PropertyEditor::getClass() {
   if (PropertyEditorMetaClass==NULL) {
      PropertyEditorMetaClass = new MetaClass("PropertyEditor", "Base Class for all property editors.",
                                          NULL, sizeof(PropertyEditor));
   }
   return PropertyEditorMetaClass;
}

void PropertyEditor::init(const MetaClass *parentClass) {
   ((MetaClass*)getClass())->init(parentClass);
   PropertyEditorMetaClass->copyParentProperties();
}

void PropertyEditor::shutdown() {
   delete PropertyEditorMetaClass;
}
