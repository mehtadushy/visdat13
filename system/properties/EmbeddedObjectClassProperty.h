//---------------------------------------------------------------------------
#ifndef EmbeddedObjectClassPropertyH
#define EmbeddedObjectClassPropertyH
//---------------------------------------------------------------------------
#include "SingleClassProperty.h"
//---------------------------------------------------------------------------


using namespace std;

class MetaClass;

void  EmbeddedObjectClassProperty_addDefaultEditor(ClassProperty *c);



class  GeneralEmbeddedObjectClassProperty : public SingleClassProperty {
 protected:
   GeneralEmbeddedObjectClassProperty() : SingleClassProperty("", 1) {}
 public:
   GeneralEmbeddedObjectClassProperty(const string CName, const Version introducedInVersion)
    : SingleClassProperty(CName, introducedInVersion) {}
   virtual Object *getSubObjectAdress(Object *obj, pointer_diff offset) const = 0;
};



/*template <class ObjectType>
class  EmbeddedObjectClassProperty : public GeneralEmbeddedObjectClassProperty {
 private:
   EmbeddedObjectClassProperty() {}

 public:
   EmbeddedObjectClassProperty(const string CName, const Version introducedInVersion,
                               pointer_diff offset);

   ObjectType getValue(const Object *obj, pointer_diff offset) const;
   void       setValue(Object *obj, pointer_diff offset, ObjectType v) const;

   virtual void writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const;
   virtual void readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const;
   virtual string toString(const Object* obj, pointer_diff offset) const;
   virtual void fromString(Object* obj, pointer_diff offset, const string str) const;
   virtual const string getTypeName() const;
   ClassProperty* copyClassProperty();
   virtual void assignProperty(const Object *from, Object *to, pointer_diff offset) const;


   virtual Object *getSubObjectAdress(Object *obj) const;
};
*/

class  EmbeddedObjectClassProperty : public GeneralEmbeddedObjectClassProperty {
 private:
   EmbeddedObjectClassProperty() {}

	MetaClass	*	m_MetaClass;

 public:
   EmbeddedObjectClassProperty(const string CName, const Version introducedInVersion,
                               pointer_diff offset, const MetaClass * mc);

   virtual void writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const;
   virtual void readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const;
   virtual string toString(const Object* obj, pointer_diff offset) const;
   virtual void fromString(Object* obj, pointer_diff offset, const string str) const;
   virtual const string getTypeName() const;
   ClassProperty* copyClassProperty();
   virtual void assignProperty(const Object *from, Object *to, pointer_diff offset) const;


   virtual Object *getSubObjectAdress(Object *obj, pointer_diff offset) const;
};

#define ADD_EMBEDDED_OBJECT_PROP( propName, version, objectClass ) \
	((MetaClass*)getClass())->addProperty( new EmbeddedObjectClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
	(pointer_diff)(&( obj_null_pointer->propName ) - NULL), objectClass) );





#endif
