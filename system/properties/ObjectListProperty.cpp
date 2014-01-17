//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ObjectListProperty.h"
//---------------------------------------------------------------------------
#include "ObjectListPropertyEditor.h"
#include "ObjectStreams.h"
#include "Persistent.h"
//---------------------------------------------------------------------------



ObjectListProperty::ObjectListProperty( const string &cName, const Version introducedInVersion,
      const MetaClass *objectBaseClass,
      AddObjectMethod addMethod, DeleteObjectsMethod delMethod,
      GetObjectMethod getMethod, MoveObjectMethod moveMethod, GetNumObjectsMethod getNumMethod,
      GetObjectNameMethod getNameMethod, card32 preferedEditorWidth, card32 preferedEditorHeight )
      :ClassProperty( cName, introducedInVersion )
{
   m_AddMethod = addMethod;
   m_DelMethod = delMethod;
   m_GetMethod = getMethod;
   m_MoveMethod = moveMethod;
   m_GetNumMethod = getNumMethod;
   m_GetNameMethod = getNameMethod;
   m_PreferedEditorWidth = preferedEditorWidth;
   m_PreferedEditorHeight = preferedEditorHeight;
   m_ObjectBaseClass = (MetaClass*)objectBaseClass;
	setCompoundAble( false );
   addPropertyEditor( ObjectListPropertyEditor::getClass(), true );
}


void ObjectListProperty::writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const
{
   card32 numObjs = getNumObjects((Object*)obj);
   out->write( numObjs );
   for( card32 i=0;i<numObjs;i++ )
   {
      out->writeObject( (Persistent*)getObject((Object*)obj,i) );
   }
}

void ObjectListProperty::readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const
{
   deleteObjects( obj, 0, getNumObjects(obj) );
   card32 numObjs;
   in->read( numObjs );
   for(card32 i=0;i<numObjs;i++ )
   {
      Persistent * tmp;
      in->readObject( tmp );
      addObject( obj, tmp );
   }

}

void   ObjectListProperty::fromString(Object* obj, pointer_diff offset, const string str) const
{
   forget(obj);
   forget(str);
}

void ObjectListProperty::assignProperty(const Object *from, Object *to, pointer_diff offset) const
{
   if( getNumObjects( to ) )
   {
      deleteObjects( to, 0, getNumObjects(to) );
   }
   for( card32 i=0;i<getNumObjects((Object*)from);i++ )
      addObject( to, getObject((Object*)from,i)->copy() );
}

ClassProperty* ObjectListProperty::copyClassProperty()
{
   ObjectListProperty * ret = new ObjectListProperty();
   *ret = *this;
   return (ClassProperty*)ret;
}

