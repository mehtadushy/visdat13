//---------------------------------------------------------------------------
#ifndef ObjectListBaseH
#define ObjectListBaseH
//---------------------------------------------------------------------------
#include <string>
#include "PTypes.h"
//---------------------------------------------------------------------------


using namespace std;

class Object;


class  ObjectListBase
{
public:

   virtual void addObject( Object * ) = 0;
   virtual void deleteObjects( const card32 &begin, const card32 &end ) = 0;
   void deleteObject( const card32 &index ) { deleteObjects(index,index+1); }
   virtual Object * getObject( const card32 &index ) = 0;
   virtual void moveObject( const card32 &oldPos, const card32 &newPos )  = 0;
   virtual card32 getNumObjects() const = 0;
   virtual string getObjectName( const card32 &index ) = 0;

};



#endif
