//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ClassProperty.h"
//---------------------------------------------------------------------------
#include "Tools.h"
//---------------------------------------------------------------------------


ClassProperty::ClassProperty() {
   ownerClass = NULL;
}

ClassProperty::ClassProperty(const string cName, const Version introducedInVersion) {
   this->cName = cName;
   this->introducedInVersion = introducedInVersion;
   this->description = "";
   this->userEdit = true;
   this->editReadonly = false;
   this->streamProperty = true;
	this->compoundAble = true;
}

void ClassProperty::setDescription(const string description) {
   this->description = description;
}

void ClassProperty::addPropertyEditor(const MetaClass *edClass, bool defaultEditor) {
   if (defaultEditor) {
      editors.push_front(edClass);
   } else {
      editors.push_back(edClass);
   }
}

const string ClassProperty::getCName() const {
   return cName;
}

const string ClassProperty::getDescription() const {
   return description;
}

const Version ClassProperty::getIntroducedInVersion() const {
   return introducedInVersion;
}

const MetaClass *ClassProperty::getPropertyEditor(card32 i) const {
   if (i >= editors.size()) {
      throw ERangeCheck("ClassProperty::getPropertyEditor: index out of range.");
   }
   return editors[i];
}

card32 ClassProperty::getNumPropertyEditors() const {
   return (card32)editors.size();
}

const MetaClass *ClassProperty::getDefaultPropertyEditor() const {
   if (getNumPropertyEditors() == 0) {
      return NULL;
   } else {
      return getPropertyEditor(0);
   }
}

void ClassProperty::setUserEdit(bool userEdit) {
   this->userEdit = userEdit;
}

void ClassProperty::setEditReadonly(bool editReadonly) {
   this->editReadonly = editReadonly;
}

void ClassProperty::setStreamProperty(bool streamProperty) {
   this->streamProperty = streamProperty;
}

void ClassProperty::assign(const Object* obj, CopyContext *context) {
   forget(obj);
   forget(context);
   throw PException("not implemented");
}

