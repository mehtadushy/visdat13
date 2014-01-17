//---------------------------------------------------------------------------
#ifndef ObjectH
#define ObjectH
//---------------------------------------------------------------------------
#include "MetaClass.h"
#include "Exceptions.h"
//---------------------------------------------------------------------------



/// provides additional context information for copying
class  CopyContext {
 private:
   struct Entry {
      string name;
      Object *data;
   };
   vector<Entry> tagStack;

 public:
   Object *getData(string name);
   void pushData(string name, Object *obj);
   void popData();
};

class  ECannotAssign : public ETypeMismatch {
 public:
   ECannotAssign(const string msg) : ETypeMismatch(msg) {};
};

class  Object {
 private:
   static MetaClass* objectMetaClass;
 public:
   virtual const MetaClass *getInstanceClass() const;
   static const MetaClass *getClass();
   static void init(const MetaClass *parentClass);
   static void shutdown();
   virtual void assign(const Object* obj, CopyContext *context = NULL);
   virtual bool equal(const Object* obj) const;
   virtual card32 getFieldsHashValue() const;
   virtual Object* copy(CopyContext *context = NULL) const;
   virtual ~Object() {}
};

/**
	Declares an abstract class - example:
	class MyClass : public Persistent
	{
		GEOX_ABSTRACT_CLASS( MyClass )

	public:
		...
	};
*/
#define GEOX_ABSTRACT_CLASS( className ) \
private: \
	static MetaClass* className##MetaClass;\
public:\
   virtual const MetaClass *getInstanceClass() const;\
   static const MetaClass *getClass();\
   static void init(const MetaClass *parentClass);\
	static void initMetaClass();\
   static void shutdown();\

/**
	Implements an abstract class - example (MyClass.cpp):
	
	IMPLEMENT_GEOX_ABTRACT_CLASS( MyClass, 3 ) // version is MetaClass::FIRST_VERSION + 3 here !
	{
		MyClassMetaClass->addProperty...
	}

	MyClass::MyClass() {}
	...

*/
#define IMPLEMENT_GEOX_ABSTRACT_CLASS_GENERIC( className, version, newInstance ) \
MetaClass* className::className##MetaClass = NULL;\
const MetaClass* className::getInstanceClass() const {\
   return getClass();\
}\
const MetaClass* className::getClass() {\
   if (className##MetaClass==NULL) {\
		className##MetaClass = new MetaClass( #className , "no description.",\
                                  newInstance, sizeof( className ));\
   }\
   return className##MetaClass;\
}\
void className::shutdown() {\
   delete className##MetaClass;\
}\
void className::init(const MetaClass *parentClass) \
{\
   ((MetaClass*)getClass())->init(parentClass, MetaClass::FIRST_VERSION + version );\
   className##MetaClass->copyParentProperties();\
	initMetaClass();\
}\
void className::initMetaClass()

#define IMPLEMENT_GEOX_ABSTRACT_CLASS( className, version )  IMPLEMENT_GEOX_ABSTRACT_CLASS_GENERIC( className, version, NULL )


/**
	Declares a class - example:
	class MyClass : public Persistent
	{
		GEOX_CLASS( MyClass )

	public:
		...
	};
*/
#define GEOX_CLASS( className ) \
private:\
   static Object *newInstance();\
	GEOX_ABSTRACT_CLASS( className )


/**
	Implements a class - example (MyClass.cpp):
	
	IMPLEMENT_GEOX_CLASS( MyClass, 3 ) // version is MetaClass::FIRST_VERSION + 3 here !
	{
		// init properties and methods here
		BEGIN_CLASS_INIT( MyClass );

		MyClassMetaClass->addProperty...
	}

	MyClass::MyClass() {}
	...

*/
#define IMPLEMENT_GEOX_CLASS( className, version ) \
	Object* className ::newInstance() {\
	return new className ;\
} IMPLEMENT_GEOX_ABSTRACT_CLASS_GENERIC( className, version, newInstance )

#define BEGIN_CLASS_INIT( MyClass ) MyClass * obj_null_pointer = NULL;
#define SET_USER_EDIT( b ) ((MetaClass *)getClass())->actProperty_setUserEdit(b);
#define SET_EDIT_READONLY( b ) ((MetaClass *)getClass())->actProperty_setEditReadonly(b);
#define SET_STREAM_PROPERTY( b ) ((MetaClass *)getClass())->actProperty_setStreamProperty(b);
#define SET_NAME( name ) ((MetaClass *)getClass())->actProperty_setPropertyName(name);
#define SET_EDITOR( name ) ((MetaClass *)getClass())->actProperty_addPropertyEditor(name, true);

#define GEOX_MEMBER( mType, mName ) \
private: \
	mType m_##mName ; \
public: \
	void set##mName##( mType v ) { m_##mName = v; } \
	mType get##mName##() const {return m_##mName ;}




#endif
