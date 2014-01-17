//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ObjectStreams.h"
#include "Persistent.h"
#include "MetaClass.h"
#include "PTypes.h"
#include "StreamClassTable.h"
#include "PAssert.h"
#include "StringHelper.h"
#include "HashTables.hpp"
#include "GeoXOutput.h"
//---------------------------------------------------------------------------
#undef DEBUG_STREAMS
#ifdef DEBUG_STREAMS
 #include "GeoXOutput.h"
#endif
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------



/****************************************************************************/
/* global file revision, not used anylonger, must be 200                    */
/****************************************************************************/
const card32 CURRENT_FILE_VERSION = 200;
/****************************************************************************/
const char *OLD_FILE_ID_STRING = "XRTObjectStream";
const char *FILE_ID_STRING     = "XGRTObjStream64";


const card32 HASHTABLE_FILLFACTOR = 16;



/// explicit instantiation so that "HashTables.hpp" does not need to be included in modules using this one
template class HashTable<mpcard, Object*, ObjectStream_ObjectHashComperator>;
/*

inline bool ObjectStream_ObjectHashComperator::equals(mpcard obj1, mpcard obj2) {
   return obj1 == obj2;
}

*/

max_platform_card ObjectStream_ObjIDHashTable::hashFunction(mpcard objID) const {
   return objID;
}


OutputObjectStream::OutputObjectStream(bool perObjectClassVersion,
                                       StreamClassTable *classTable,
                                       bool doWriteClassTable) {
   objIdTable.setFillFactor(HASHTABLE_FILLFACTOR);
   this->perObjectClassVersion = perObjectClassVersion;
   this->doWriteClassTable = doWriteClassTable;
   if (classTable == NULL) {
      this->classTable = new StreamClassTable();
      externClassTable = false;
   } else {
      this->classTable = classTable;
      externClassTable = true;
   }
}

InputObjectStream::InputObjectStream(StreamClassTable *classTable) {
   objIdTable.setFillFactor(HASHTABLE_FILLFACTOR);
   lastClass = NULL;
   if (classTable == NULL) {
      this->classTable = new StreamClassTable();
      externClassTable = false;
   } else {
      this->classTable = classTable;
      externClassTable = true;
   }
   majorStreamVersion = SV_UNDEFINED;
}

void OutputObjectStream::initStream() {
   writeBuffer(FILE_ID_STRING, 15);
   card32 versionWithFlags = CURRENT_FILE_VERSION;
   if (!doWriteClassTable) versionWithFlags |= 0x10000000;
   if (perObjectClassVersion) versionWithFlags |= 0x20000000;
   write<card32>(versionWithFlags);
   if (doWriteClassTable) {
      writeClassTableHeader();
   }
}

void InputObjectStream::initStream() {
   char fileID[16];
   readBuffer(fileID, 15);
   fileID[15] = 0;
   if (strcmp(FILE_ID_STRING, fileID) != 0) {
      if (strcmp(OLD_FILE_ID_STRING, fileID) != 0) {
         throw EIOException("BinaryInputObjectStream::BinaryInputObjectStream() - the file is no GeoX ObjectStream.");
      } else {
         majorStreamVersion = SV_XRTObjectStream;
      }
   } else {
      majorStreamVersion = SV_XGRTObjStream64;
   }
   card32 versionWithFlags;
   read<card32>(versionWithFlags);

   this->version = (versionWithFlags & 0x0FFFFFFF);
   bool dontReadClassTable = ((versionWithFlags & 0x10000000) != 0);
   perObjectClassVersion = ((versionWithFlags & 0x20000000) != 0);
   if (this->version >= 200 && !dontReadClassTable) readClassTable();
}

void OutputObjectStream::writeBool(const bool &v) {
   static const card8 t = 1;
   static const card8 f = 0;
   if (v) {
      writeBuffer(&t, 1);
   } else {
      writeBuffer(&f, 1);
   }
}

void InputObjectStream::readBool(bool &v) {
   card8 buf;
   readBuffer(&buf, 1);
   if (buf != 0) {
      v = true;
   } else {
      v = false;
   }
}

void OutputObjectStream::writeString(const string &v) {
   card64 length = (card64)v.length();
   writeBuffer(&length, sizeof(length));
   if (length > 0) {
      writeBuffer(v.c_str(), (mpcard)length);
   }
}

void InputObjectStream::readString(string &v) {
   card64 length;
   if (majorStreamVersion == SV_XRTObjectStream) {
      card32 length32;
      readBuffer(&length32, sizeof(length32));
      length = length32;
   } else {
      #ifdef _DEBUG
      if (majorStreamVersion != SV_XGRTObjStream64) {
         pAssert(false);
      }
      #endif
      readBuffer(&length, sizeof(length));
   } 
   char *buf = new char[(unsigned int)length+1];
   if (length > 0) {
      readBuffer(buf, (mpcard)length);
   }
   buf[length] = 0;
   v = buf;
   delete[] buf;
}

void OutputObjectStream::writeObject(const Persistent *obj) {

   // Generate new class id or read existing class id from hash table
   card32 classId = 0;
   if (obj != NULL) {
      classId = classTable->getOrCreateClassID(obj->getInstanceClass());
      #ifdef _DEBUG
      if (obj != NULL) {
         string cName = obj->getInstanceClass()->getClassName();
         if (!MetaClass::isClassRegistered(cName)) {
            error("OutputObjectStream::writeObject() - writing a class that has not been registered.");
            pAssert(false);
         }
      }
      #endif

      // Also put all parent classes into the class table
      const MetaClass* mc = (obj->getInstanceClass())->getParentClass();
      while (mc != NULL) {
         classTable->getOrCreateClassID(mc);
         mc = mc->getParentClass();
      }
   }

   #ifdef DEBUG_STREAMS
   static DEBUG_IDENT = 0;
   output.writeIdented(3*DEBUG_IDENT, ((AnsiString("w objID: ") + IntToStr((card64)obj))).c_str());
   output.cr();
   if (obj == NULL) {
      output.writeIdented(3*DEBUG_IDENT, "w class: NULL");
      output.cr();
   } else {
      output.writeIdented(3*DEBUG_IDENT, (string("w class: ") + obj->getInstanceClass()->getClassName()));
      output.cr();
      output.writeIdented(3*DEBUG_IDENT, (string("w classID: ") + IntToStr(classId).c_str()));
      output.cr();
   }
   #endif

   if (obj == NULL) {
      write<card64>(0);
   } else {
      Object* storedID;
      // Write object if it wasn't written before
      if (!objIdTable.getData((mpcard)obj, storedID)) {
         card64 objID = (card64)obj;
         write<card64>(objID);
         objIdTable.addElement((mpcard)obj, (Object*)obj);
         write<card32>(classId + 1000);
         if (perObjectClassVersion) {
            write<card32>(obj->getInstanceClass()->getClassVersion());
         }

         #ifdef DEBUG_STREAMS
          DEBUG_IDENT++;
         #endif

         obj->write(this);

         #ifdef DEBUG_STREAMS
          DEBUG_IDENT--;
         #endif
      } else {
         pAssert( (storedID == (Object*)obj || ((card64)storedID > 0 && (card64)storedID < 100)) );
         write<card64>((card64)storedID);

      }
   }
}

void OutputObjectStream::writeMetaClass(const MetaClass *mc) {
   write<card32>(classTable->getOrCreateClassID(mc) + 1000);

   #ifdef _DEBUG
    if (mc != NULL) {
       string cName = mc->getClassName();
       if (!MetaClass::isClassRegistered(cName)) {
          /*
          AnsiString cNameAS = cName.c_str();
          Application->MessageBox(
             "Warning:",
             (AnsiString( "OutputObjectStream::writeMetaClass() - writing a class that has not been registered (class " )
             +cNameAS+AnsiString("). Reading the stream again is not possible unless the class has been registered.")).c_str(),
             MB_ICONEXCLAMATION | MB_OK
          );
          */
       }
    }
   #endif

}

void OutputObjectStream::includeClass(const MetaClass *mc) {
   classTable->getOrCreateClassID(mc);
}

void InputObjectStream::readMetaClass(const MetaClass *&mc) {
   if (getVersion() < 200) {   // Old file version, downward compatibility
      card32 uniqueClassID;
      read<card32>(uniqueClassID);
      mc = MetaClass::getClassByUniqueIdentifier(uniqueClassID);
   } else {                    // New file version
      card32 classId;
      read<card32>(classId);
      pAssert(classId >= 1000);
      mc = classTable->getClass(classId - 1000);
      if (mc == NULL) {
         throw EIOException(string("InputObjectStream::readMetaClass() - class not found, id: ") + intToStr(classId).c_str());
      }
   }
}


void InputObjectStream::readObject(Persistent *&obj) {
   #ifdef DEBUG_STREAMS
    static DEBUG_IDENT = 0;
   #endif

   card64 objID;
   if (majorStreamVersion == SV_XGRTObjStream64) {
      read<card64>(objID);
   } else {
      #ifdef _DEBUG
      if (majorStreamVersion != SV_XRTObjectStream) {
         pAssert(false);
      }
      #endif
      card32 objID32;
      read<card32>(objID32);
      objID = objID32;
   }

   #ifdef DEBUG_STREAMS
    output.cr();
    output.writeIdented(3*DEBUG_IDENT, ((AnsiString("r objID: ") + IntToStr(objID))).c_str());
    output.cr();
   #endif

   if (objID == 0) {
      obj = NULL;
      return;
   } else {
      Persistent* storedObj = NULL;
      bool found = objIdTable.getData((mpcard)objID, (Object*&)storedObj);
      // Read object if it wasn't read before
      if (!found) {
         card32 uniqueClassID;
         read<card32>(uniqueClassID);

         #ifdef DEBUG_STREAMS
          output.writeIdented(3*DEBUG_IDENT, ((AnsiString("r classID: ") + IntToStr(uniqueClassID))).c_str());
          output.cr();
         #endif

         // new streams store classes with version numbers starting at 1000 (this has probably been a misconception, but now necessary for backward compatibility)
         if (uniqueClassID >= 1000) uniqueClassID -= 1000;

         if (perObjectClassVersion) {
            card32 localVersion;
            read<card32>(localVersion);
            classTable->pushVersion(uniqueClassID, localVersion);
         }

         const MetaClass* objClass;
         if (getVersion() < 200) {    // Old file version, downward compatibility
            objClass = MetaClass::getClassByUniqueIdentifier(uniqueClassID);
            lastClass = objClass;
         } else {                     // New file version
            if (uniqueClassID < classTable->getNumClassIDs()) {
               objClass = classTable->getClass(uniqueClassID);
               if (objClass == NULL) {
                  string lastClassName = "no previous class (first in stream).";
                  if (lastClass != NULL) {
                     lastClassName = lastClass->getClassName();
                  }
                  throw EIOException(string("InputObjectStream::readObject() - class not found, id (normalized, 1000 subtracted if necessary): ")
                                     + intToStr(uniqueClassID)
                                     + string("Last class has been: ")
                                     + lastClassName);
               }
               lastClass = objClass;
            } else {
               string lastClassName = "no previous class (first in stream).";
               if (lastClass != NULL) {
                  lastClassName = lastClass->getClassName();
               }
               throw PException("InputObjectStream::readObject - unknown class id encountered. Last class has been: "+lastClassName);
            }
         }
         #ifdef DEBUG_STREAMS
          output.writeIdented(3*DEBUG_IDENT, string("r class name: ") + objClass->getClassName());
          output.cr();
         #endif

         obj = (Persistent*)(objClass->newInstance());
         objIdTable.addElement((mpcard)objID, obj);

         #ifdef DEBUG_STREAMS
          DEBUG_IDENT++;
         #endif

         obj->read(this);

         if (perObjectClassVersion) {
            classTable->popVersion(uniqueClassID);
         }

         #ifdef DEBUG_STREAMS
          DEBUG_IDENT--;
         #endif

         return;
      } else {
         obj = storedObj;
         return;
      }
   }
}

void OutputObjectStream::writeMaxPlatformCard(const max_platform_card &v) {
   card64 vlarge = v;
   write<card64>(vlarge);
}

void OutputObjectStream::writeMaxPlatformInt(const max_platform_int &v) {
   int64 vlarge = v;
   write<int64>(vlarge);
}

void InputObjectStream::readMaxPlatformCard(max_platform_card &v) {
   card64 vlarge;
   read<card64>(vlarge);
   if (vlarge > card64(0xFFFFFFFF)) {
      throw EIOException("InputObjectStream::readMaxPlatformCard() - file contains a number that cannot be represented on a 32bit platform.");
   }
   v = (max_platform_card)vlarge;
}

void InputObjectStream::readMaxPlatformInt(max_platform_int &v) {
   int64 vlarge;
   read<int64>(vlarge);
   if (vlarge >= int64(0x7FFFFFFF) || vlarge < -int64(0x7FFFFFFF)) {
      throw EIOException("InputObjectStream::readMaxPlatformInt() - file contains a number that cannot be represented on a 32bit platform.");
   }
   v = (max_platform_int)vlarge;
}

card32 InputObjectStream::getClassVersion(const MetaClass* metaClass) {
   card32 id = classTable->getClassID(metaClass);
   if (id == StreamClassTable::BAD_CLASS) {
      return 1000;
   } else {
      return classTable->getCurrentVersion(id);
   }
}

void OutputObjectStream::addGlobalObject(Persistent *obj, mpcard index) {
   pAssert(index < 100); // EVIL hack - assumes no objects in first 100 bytes and supports only 100 global objects
   objIdTable.addElement((mpcard)obj, (Object*)(index+1));
}

void InputObjectStream::addGlobalObject(Persistent *obj, mpcard index) {
   pAssert(index < 100); // EVIL hack - assumes no objects in first 100 bytes and supports only 100 global objects
   objIdTable.addElement(index+1, (Object*)obj);
}

string OutputObjectStream::getFileName() {
	return "";
}

string InputObjectStream::getFileName() {
	return "";
}

OutputObjectStream::~OutputObjectStream() {
   if (!externClassTable) {
      delete classTable;
   }
}

InputObjectStream::~InputObjectStream() {
   if (!externClassTable) {
      delete classTable;
   }
}

