//---------------------------------------------------------------------------
#ifndef ObjectClassPropertyH
#define ObjectClassPropertyH
//---------------------------------------------------------------------------
#include "SingleClassProperty.h"
//---------------------------------------------------------------------------


using namespace std;


class  ObjectClassProperty : public SingleClassProperty {
 private:
   ObjectClassProperty() {}
   void addDefaultEditor(ClassProperty *c);
   const MetaClass *baseClass;
   bool owner;

 public:
   typedef Object* (Object::* ObjectGetPropertyMethod)() const;
   typedef void (Object::* ObjectSetPropertyMethod)(Object*);
   typedef void (Object::* UpdateMethod)();

 protected:
   ObjectGetPropertyMethod getMethod;
   ObjectSetPropertyMethod setMethod;
   UpdateMethod updateAfterWrite;
   bool useGetSetMethods;

 public:
   ObjectClassProperty(const string CName, const Version introducedInVersion,
                       ObjectGetPropertyMethod getMethod, ObjectSetPropertyMethod setMethod,
                       const MetaClass *baseClass, bool isOwner = false);

   ObjectClassProperty(const string CName, const Version introducedInVersion,
                       pointer_diff offset, const MetaClass *baseClass, bool isOwner = false,
                       UpdateMethod updateAfterWrite = NULL);

   Object* getValue(const Object *obj, pointer_diff offset) const;
   void    setValue(Object *obj, pointer_diff offset, Object *v) const;

   const MetaClass *getBaseClass() const;
   void setBaseClass(const MetaClass *baseClass);
   bool getObjectIsOwnerOfSubobject() const;
   void setObjectIsOwnerOfSubobject(bool isOwner);

	bool getUseGetSetMethods() const {return useGetSetMethods;}

   virtual void writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const;
   virtual void readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const;
   virtual string toString(const Object* obj, pointer_diff offset) const;
   virtual void fromString(Object* obj, pointer_diff offset, const string str) const;
   virtual const string getTypeName() const;

   void assignProperty(const Object *from, Object *to, pointer_diff offset) const;

   ClassProperty* copyClassProperty();
};

typedef ObjectClassProperty::ObjectSetPropertyMethod SetObjectPropMethod;
typedef ObjectClassProperty::ObjectGetPropertyMethod GetObjectPropMethod;
typedef ObjectClassProperty::UpdateMethod ObjectUpdateMethod;

#define ADD_OBJECT_PROP_UPDATE( propName, version, baseClass, is_owner, updateMethodPtr ) \
	((MetaClass*)getClass())->addProperty( new ObjectClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
	(pointer_diff)(&( obj_null_pointer->propName ) - NULL), baseClass, is_owner, (ObjectUpdateMethod)updateMethodPtr) );

#define ADD_OBJECT_PROP( propName, version, baseClass, is_owner ) \
	ADD_OBJECT_PROP_UPDATE( propName, version, baseClass, is_owner, NULL )

// do not use in CompoundProperties !
#define ADD_OBJECT_PROP_METHOD( propName, version, get, set, baseClass, is_owner ) \
	((MetaClass*)getClass())->addProperty( new ObjectClassProperty( #propName , MetaClass::FIRST_VERSION + version, \
	(GetObjectPropMethod)get, (SetObjectPropMethod)set, baseClass, is_owner) );
	




#endif
