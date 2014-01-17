//---------------------------------------------------------------------------
#ifndef ClassTypeClassPropertyH
#define ClassTypeClassPropertyH
//---------------------------------------------------------------------------
#include "SingleClassProperty.h"
//---------------------------------------------------------------------------

using namespace std;


class  ClassTypeClassProperty : public SingleClassProperty {
 private:
   ClassTypeClassProperty() {}
   const MetaClass *baseClass;

 public:
   ClassTypeClassProperty(const string CName, const Version introducedInVersion,
                          pointer_diff offset, const MetaClass *baseClass);

   const MetaClass* getValue(const Object *obj, pointer_diff offset) const;
   void setValue(Object *obj, pointer_diff offset, const MetaClass *v) const;

   const MetaClass *getBaseClass() const;
   void setBaseClass(const MetaClass *baseClass);

   virtual void writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const;
   virtual void readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const;
   virtual string toString(const Object* obj, pointer_diff offset) const;
   virtual void fromString(Object* obj, pointer_diff offset, const string str) const;
   virtual const string getTypeName() const;

   virtual void assignProperty(const Object *from, Object *to, pointer_diff offset) const;

   ClassProperty* copyClassProperty();
};

#define ADD_METACLASS_PROP( propName, version, baseClass ) \
	((MetaClass*)getClass())->addProperty( new ClassTypeClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
	(pointer_diff)(&( obj_null_pointer->propName ) - NULL) , baseClass ) );




#endif
