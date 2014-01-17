//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "EmbeddedObjectClassProperty.h"
#include "MetaClass.h"
#include "Persistent.h"
#include "Tools.h"
//---------------------------------------------------------------------------
#include "EmbeddedObjectClassPropertyEditor.h"
//---------------------------------------------------------------------------



void EmbeddedObjectClassProperty_addDefaultEditor(ClassProperty *c) {
   c->addPropertyEditor(EmbeddedObjectClassPropertyEditor::getClass(), true);
}


EmbeddedObjectClassProperty::EmbeddedObjectClassProperty(const string CName, const Version introducedInVersion,
                               pointer_diff offset, const MetaClass * mc) 
										 : GeneralEmbeddedObjectClassProperty(CName, introducedInVersion) 
{
	m_MetaClass = (MetaClass *)mc;
	pAssert( !mc->isAbstract() );
	setOffset( offset );
   EmbeddedObjectClassProperty_addDefaultEditor(this);
}

void EmbeddedObjectClassProperty::writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const 
{
   Persistent * src = dynamic_cast<Persistent*>(getSubObjectAdress( (Object *)obj, offset ));
	pAssert( src != NULL );
	src->write(out);
}

void EmbeddedObjectClassProperty::readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const 
{
   Persistent * dest = dynamic_cast<Persistent*>(getSubObjectAdress( obj, offset ));
	pAssert( dest != NULL );
	dest->read(in);
}

string EmbeddedObjectClassProperty::toString(const Object* obj, pointer_diff offset) const {
   forget(obj);
   notImplemented();
   return "";
}

void EmbeddedObjectClassProperty::fromString(Object* obj, pointer_diff offset, const string str) const {
   forget(obj);
   forget(str);
   notImplemented();
}

const string EmbeddedObjectClassProperty::getTypeName() const 
{
   return m_MetaClass->getClassName();
}

ClassProperty* EmbeddedObjectClassProperty::copyClassProperty() {
   EmbeddedObjectClassProperty *result = new EmbeddedObjectClassProperty();
   *result = *this;
   return result;
}

void EmbeddedObjectClassProperty::assignProperty(const Object *from, Object *to, pointer_diff offset) const 
{
	Object * src = getSubObjectAdress( (Object*)from, offset );
	Object * dest = getSubObjectAdress( to, offset );
	dest->assign( src );
}


Object *EmbeddedObjectClassProperty::getSubObjectAdress(Object *obj, pointer_diff offset) const 
{
   return (Object*)((char*)obj + getOffset() + offset);
}
