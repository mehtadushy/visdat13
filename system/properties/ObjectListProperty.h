//---------------------------------------------------------------------------
#ifndef ObjectListPropertyH
#define ObjectListPropertyH
//---------------------------------------------------------------------------
#include "ClassProperty.h"
#include "Tools.h"
//---------------------------------------------------------------------------



class OutputObjectStream;
class InputObjectStream;
class ObjectList_Prop;
class ObjectListElementEditor;


class  ObjectListProperty : public ClassProperty {
 public:

   // property-access-methods
   typedef void (Object::* AddObjectMethod)(Object *obj);
   typedef void (Object::* DeleteObjectsMethod)(card32 begin,card32 end);
   typedef Object* (Object::* GetObjectMethod)(card32 index);
   typedef void (Object::* MoveObjectMethod)(card32 oldPos, card32 newPos);
   typedef card32 (Object::* GetNumObjectsMethod)();
   typedef string (Object::* GetObjectNameMethod)(card32 index);

// methods for copy&paste :-)
/*
   void addXX( Object * obj );
   void deleteXXs( card32 begin, card32 end );
   Object * getXX( card32 index );
   void moveXX( card32 oldPos, card32 newPos );
   card32 getNumXXs();
   string getXXName(card32 index );

// add property implementation:

   MetaClass->addProperty( new ObjectListProperty( "",MetaClass::FIRST_VERSION + 0,
      ::getClass(), (ObjectListProperty::AddObjectMethod)addXX,
      (ObjectListProperty::DeleteObjectsMethod)deleteXXs,
      (ObjectListProperty::GetObjectMethod)getXX,
      (ObjectListProperty::MoveObjectMethod)moveXX,
      (ObjectListProperty::GetNumObjectsMethod)getNumXXs,
      (ObjectListProperty::GetObjectNameMethod)getXXName ) );

*/

 friend class ObjectList_Prop;
 friend class ObjectListElementEditor;

 private:

   ObjectListProperty() {}

   AddObjectMethod      m_AddMethod;
   DeleteObjectsMethod   m_DelMethod;
   GetObjectMethod      m_GetMethod;
   MoveObjectMethod     m_MoveMethod;
   GetNumObjectsMethod  m_GetNumMethod;
   GetObjectNameMethod  m_GetNameMethod;

   card32               m_PreferedEditorWidth;
   card32               m_PreferedEditorHeight;
   MetaClass            *m_ObjectBaseClass;

 public:

   ObjectListProperty( const string &cName, const Version introducedInVersion,
      const MetaClass *objectBaseClass,
      AddObjectMethod addMethod, DeleteObjectsMethod delMethod,
      GetObjectMethod getMethod, MoveObjectMethod moveMethod, GetNumObjectsMethod getNumMethod,
      GetObjectNameMethod getNameMethod, card32 preferedEditorWidth = 0,
         card32 preferedEditorHeight = 300 );

   card32 getPreferedEditorWidth() const { return m_PreferedEditorWidth;  }
   void setPreferedEditorWidth( const card32 &Width )  {  m_PreferedEditorWidth = Width; }
   card32 getPreferedEditorHeight() const { return m_PreferedEditorHeight;  }
   void setPreferedEditorHeight( const card32 &height )  {  m_PreferedEditorHeight = height; }
   MetaClass * getObjectBaseClass() const {return m_ObjectBaseClass; }
   void setObjectBaseClass( MetaClass * mc ) {m_ObjectBaseClass = mc; }

   virtual const string  getTypeName() const
   {
      return "ObjectList";
   }

   virtual void writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const;
   virtual void readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const;

   virtual string toString(const Object* obj, pointer_diff offset) const {forget(obj ); return "";}
   virtual void   fromString(Object* obj, pointer_diff offset, const string str) const;

   virtual void assignProperty(const Object *from, Object *to, pointer_diff offset) const;
   virtual ClassProperty* copyClassProperty();

 private:

   void addObject( Object *obj, Object * toAdd ) const
   {
      (obj->*m_AddMethod)(toAdd);
   }

   void deleteObjects( Object *obj, card32 begin, card32 end ) const
   {
      (obj->*m_DelMethod)(begin,end);
   }

   Object * getObject( Object * obj, card32 index ) const
   {
      return (obj->*m_GetMethod)(index);
   }

   void moveObject(Object * obj,card32 oldPos, card32 newPos) const
   {
      (obj->*m_MoveMethod)(oldPos,newPos);
   }
   mpcard getNumObjects(Object * obj) const
   {
      return (obj->*m_GetNumMethod)();
   }

   string getObjectName( Object * obj, card32 index ) const
   {
      return (obj->*m_GetNameMethod)(index);
   }


};

#define ADD_OBJECT_LIST_PROP( propName, version, metaClass ) \
	((MetaClass*)getClass())->addProperty( new ObjectListProperty( #propName , MetaClass::FIRST_VERSION + version, \
	metaClass, (ObjectListProperty::AddObjectMethod)&add##propName, \
	(ObjectListProperty::DeleteObjectsMethod)&delete##propName##s , \
	(ObjectListProperty::GetObjectMethod)&get##propName , \
	(ObjectListProperty::MoveObjectMethod)&move##propName , \
	(ObjectListProperty::GetNumObjectsMethod)&getNum##propName##s , \
	(ObjectListProperty::GetObjectNameMethod)&get##propName##Name ) );


#endif
