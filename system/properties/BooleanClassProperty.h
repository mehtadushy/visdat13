//---------------------------------------------------------------------------
#ifndef BooleanClassPropertyH
#define BooleanClassPropertyH
//---------------------------------------------------------------------------
#include "SingleClassProperty.h"
#include "ObjectStreams.h"
//---------------------------------------------------------------------------

using namespace std;


class  BooleanClassProperty : public SingleClassProperty {
 private:
   BooleanClassProperty() {}
   void addDefaultEditor(ClassProperty *c);

 public:
   typedef bool (Object::* BooleanGetPropertyMethod)() const;
   typedef void (Object::* BooleanSetPropertyMethod)(bool);

 protected:
   BooleanGetPropertyMethod getMethod;
   BooleanSetPropertyMethod setMethod;
   bool useGetSetMethods;

 public:
   BooleanClassProperty(const string CName, const Version introducedInVersion,
                        BooleanGetPropertyMethod getMethod, BooleanSetPropertyMethod setMethod);
   BooleanClassProperty(const string CName, const Version introducedInVersion,
                        pointer_diff offset);

   bool getValue(const Object *obj, pointer_diff offset) const;
   void setValue(Object *obj, pointer_diff offset, bool v) const;

   virtual void writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const;
   virtual void readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const;
   virtual string toString(const Object* obj, pointer_diff offset) const;
   virtual void fromString(Object* obj, pointer_diff offset, const string str) const;
   virtual const string getTypeName() const;

   virtual void assignProperty(const Object *from, Object *to, pointer_diff offset) const;

   ClassProperty* copyClassProperty();
};

typedef BooleanClassProperty::BooleanSetPropertyMethod SetBooleanPropMethod;
typedef BooleanClassProperty::BooleanGetPropertyMethod GetBooleanPropMethod;

#define ADD_BOOLEAN_PROP( propName, version ) \
	((MetaClass*)getClass())->addProperty( new BooleanClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
	(pointer_diff)(&( obj_null_pointer->propName ) - NULL) ) );

#define ADD_BOOLEAN_PROP_METHOD( propName, version, get, set ) \
	((MetaClass*)getClass())->addProperty( new BooleanClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
	(GetBooleanPropMethod) get, (SetBooleanPropMethod) set) );



#endif
