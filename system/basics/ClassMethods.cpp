//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ClassMethods.h"
//---------------------------------------------------------------------------


ClassMethod::ClassMethod(string name, string description, string icon, bool visible) {
	this->name = name;
   this->description = description;
	this->icon = icon;
	this->visible = visible;
}
string ClassMethod::getDescription() const {
   return description;
}

string ClassMethod::getName() const {
	return name;
}

string ClassMethod::getIcon() const {
	return icon;
}

bool ClassMethod::getVisible() const {
	return visible;
}

// ---

NoArgsClassMethod::NoArgsClassMethod(MethodPointer method, string name, string description, string icon, bool visible) : ClassMethod(name, description, icon, visible) {
   this->method = method;
}

void NoArgsClassMethod::call(Object *obj) const {
   (obj->*method)();
}

ClassMethod *NoArgsClassMethod::copyMethod() const {
   ClassMethod *result = new NoArgsClassMethod(method, getName(), getDescription(), getIcon());
   return result;
}
