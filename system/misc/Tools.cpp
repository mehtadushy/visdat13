//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "Tools.h"
//---------------------------------------------------------------------------
#include "Exceptions.h"
#include "Object.h"
#include <fstream>
#include <stdio.h>
//---------------------------------------------------------------------------

void notImplemented() {
   throw PException("not implemented.");
}

string makeObjectDescription(const Object *obj, bool shortDescr) {
   if (obj == NULL) return "NULL";
   string objDescr;
   objDescr = obj->getInstanceClass()->getClassName().c_str();
  char buffer[1000];
   if (!shortDescr && obj != NULL )
  {
    sprintf( buffer, "@%p (%d Bytes)", obj, obj->getInstanceClass()->getInstanceSize() );
   } else {
    sprintf( buffer, "@%p", obj );
   }
  objDescr += buffer;
   return objDescr;

}

bool FileExists( const char * filename )
{
  std::ifstream in( filename );
  if( in.is_open() )
  {
    in.close();
    return true;
  }
  return false;
}

