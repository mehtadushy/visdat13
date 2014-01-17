//---------------------------------------------------------------------------
#ifndef BinaryObjectStreamsH
#define BinaryObjectStreamsH
//---------------------------------------------------------------------------
#include "ObjectStreams.h"
//---------------------------------------------------------------------------
#include <stdio.h>
//---------------------------------------------------------------------------


class  BinaryOutputObjectStream : public OutputObjectStream {
 private:
   FILE *file;     
   string fname;
   card64 classTabPointerPos;

 protected:
   virtual void writeClassTableHeader();
   virtual void writeClassTable();

 public:
   BinaryOutputObjectStream(const char *filename, bool perObjectClassVersion = false, StreamClassTable *classTable = NULL, bool doWriteClassTable = true);
   virtual void writeBuffer(const void* buf, mpcard length);
   virtual string getFileName();
   virtual ~BinaryOutputObjectStream();
};

class  BinaryInputObjectStream : public InputObjectStream {
 private:
   FILE *file;
   string fname;

 protected:
   virtual void readClassTable();

 public:
   BinaryInputObjectStream(const char *filename);
   virtual void readBuffer(void* buf, mpcard length);
   virtual string getFileName();
   virtual ~BinaryInputObjectStream();
};



#endif
