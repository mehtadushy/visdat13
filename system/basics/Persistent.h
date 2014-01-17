//---------------------------------------------------------------------------
#ifndef PersistentH
#define PersistentH
//---------------------------------------------------------------------------
#include "Object.h"
#include "MetaClass.h"
//---------------------------------------------------------------------------



class OutputObjectStream;
class InputObjectStream;


class  Persistent : public Object {
 private:
   static MetaClass *persistentMetaClass;
 public:
   virtual const MetaClass *getInstanceClass() const;
   static const MetaClass *getClass();
   static void init(const MetaClass *parentClass);
   static void shutdown();

   virtual void write(OutputObjectStream *s) const;
   virtual void read(InputObjectStream *s);
};

// Standard object I/O: read/write all properties declared as stream properties
void  writeObjectProperties(OutputObjectStream *s, const Persistent *obj);
void  readObjectProperties(InputObjectStream *s, Persistent *obj);




#endif
