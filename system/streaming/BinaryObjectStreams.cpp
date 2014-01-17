//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "BinaryObjectStreams.h"
//---------------------------------------------------------------------------
#include "Exceptions.h"
#include "MetaClass.h"
#include "StreamClassTable.h"
//---------------------------------------------------------------------------
#ifdef DEBUG_STREAMS
 #include "GeoXOutput.h"
#endif
//---------------------------------------------------------------------------




BinaryOutputObjectStream::BinaryOutputObjectStream(const char *filename,
   bool perObjectClassVersion, StreamClassTable *classTable, bool doWriteClassTable)
: OutputObjectStream(perObjectClassVersion, classTable, doWriteClassTable)
{
	fname = filename;
   file = fopen(filename, "wb");
   if (file == NULL) {
      throw EIOException("BinaryOutputObjectStream::BinaryOutputObjectStream() - could not open file.");
   }
   initStream();
}

void BinaryOutputObjectStream::writeClassTableHeader() {
   // Place holder for pointer to class table, will be filled later,
   // in method writeClassTable()
   fpos_t fpos;
   if (fgetpos(file, &fpos) != 0)
      throw EIOException("BinaryOutputObjectStream::writeClassTable() - could not get file position");
   classTabPointerPos = *(card64 *)(void *)&fpos;
   write<card64>(0);
}

void BinaryOutputObjectStream::writeBuffer(const void* buf, mpcard length) {
   if (fwrite(buf, length, 1, file) != 1 && length > 0) 
	{
      throw EIOException("BinaryOutputObjectStream::writeBuffer - could not write all data.");
   }
   #ifdef DEBUG_STREAMS
   char* cbuf = (char*)buf;
   output << "binary: ";
   for (mpcard i = 0; i <length; i++) {
      output << (card8)(*cbuf) << " ";
      cbuf++;
   }
   output.cr();
   #endif
}

void BinaryOutputObjectStream::writeClassTable() {
   fpos_t fpos;
   if (fgetpos(file, &fpos) != 0)
      throw EIOException("BinaryOutputObjectStream::writeClassTable() - could not get file position");
   card64 classTablePos = *(card64 *)(void *)&fpos;

   #ifdef DEBUG_STREAMS
   output << "Writing class table to position " << fpos;
   output.cr();
   output << "Writing " << (IntToStr(classIdTable.count())).c_str() << " class table entries.";
   output.cr();
   #endif

   classTable->write(this);

   if (fseek(file, (long)classTabPointerPos, SEEK_SET) != 0)
      throw EIOException("BinaryOutputObjectStream::writeClassTable() - could not seek to class table pointer");
   write<card64>(classTablePos);
}

string BinaryOutputObjectStream::getFileName() {
	return fname;
}


BinaryOutputObjectStream::~BinaryOutputObjectStream() {
   writeClassTable();

   if (fclose(file) != 0) {
      throw EIOException("BinaryOutputObjectStream::~BinaryOutputObjectStream() - could not close file.");
   }
}




BinaryInputObjectStream::BinaryInputObjectStream(const char *filename) {
	fname = filename;
   file = fopen(filename, "rb");
   if (file == NULL) {
      throw EIOException("BinaryInputObjectStream::BinaryInputObjectStream() - could not open file.");
   }
   initStream();
}

void BinaryInputObjectStream::readClassTable() {
   card64 classTablePos;
   read<card64>(classTablePos);

   fpos_t fposStart;
   if (fgetpos(file, &fposStart) != 0)
      throw EIOException("BinaryInputObjectStream::readClassTable() - could not get file position");

   if (fseek(file, long(classTablePos), SEEK_SET) != 0)
      throw EIOException("BinaryInputObjectStream::readClassTable() - could not seek to class table");

   #ifdef DEBUG_STREAMS
   output << "Reading class table";
   output.cr();
   #endif

   classTable->read(this);

   if (fseek(file, *(long *)(void *)&fposStart, SEEK_SET) != 0)
      throw EIOException("BinaryInputObjectStream::readClassTable() - could not seek back to beginning of file");

}

void BinaryInputObjectStream::readBuffer(void* buf, mpcard length) {
   if (length==0) return;

   if (fread(buf, length, 1, file) != 1) {
      throw EIOException("BinaryInputObjectStream::readBuffer - could not read all data.");
   }

   #ifdef DEBUG_STREAMS
   char* cbuf = (char*)buf;
   output << "binary: ";
   for (mpcard i = 0; i <length; i++) {
      output << (card8)(*cbuf) << " ";
      cbuf++;
   }
   output.cr();
   #endif
}

string BinaryInputObjectStream::getFileName() {
	return fname;
}


BinaryInputObjectStream::~BinaryInputObjectStream() {
   if (fclose(file) != 0) {
      throw EIOException("BinaryInputObjectStream::~BinaryInputObjectStream() - could not close file.");
   }
}
