//---------------------------------------------------------------------------
#ifndef StringClassPropertyH
#define StringClassPropertyH
//---------------------------------------------------------------------------
#include "SingleClassProperty.h"
#include "ObjectStreams.h"
//---------------------------------------------------------------------------



class  StringClassProperty : public SingleClassProperty {
 private:
   StringClassProperty() {}
   void addDefaultEditor(ClassProperty *c);

 public:
   typedef string (Object::* StringGetPropertyMethod)() const;
   typedef void (Object::* StringSetPropertyMethod)(string);
//   typedef string Object::* ElementPointer;

 protected:
   StringGetPropertyMethod getMethod;
   StringSetPropertyMethod setMethod;
//   ElementPointer elementPtr;
   bool useGetSetMethods;

 public:
   StringClassProperty(const string CName, const Version introducedInVersion,
                       StringGetPropertyMethod getMethod, StringSetPropertyMethod setMethod);
   StringClassProperty(const string CName, const Version introducedInVersion,
                       pointer_diff offset);

   string getValue(const Object *obj, pointer_diff offset) const;
   void   setValue(Object *obj, pointer_diff offset, string v) const;

   virtual void writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const;
   virtual void readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const;
   virtual string toString(const Object* obj, pointer_diff offset) const;
   virtual void fromString(Object* obj, pointer_diff offset, const string str) const;
   virtual const string getTypeName() const;
   virtual ClassProperty* copyClassProperty();

   void assignProperty(const Object *from, Object *to, pointer_diff offset) const;
};

typedef StringClassProperty::StringSetPropertyMethod SetStringPropMethod;
typedef StringClassProperty::StringGetPropertyMethod GetStringPropMethod;


#define ADD_STRING_PROP( propName, version ) \
	((MetaClass*)getClass())->addProperty( new StringClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
	(pointer_diff)(&( obj_null_pointer->propName ) - NULL) ) );

#define ADD_STRING_PROP_METHOD( propName, version, get, set ) \
	((MetaClass*)getClass())->addProperty( new StringClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
	(GetStringPropMethod) get, (SetStringPropMethod) set ) );





#endif
