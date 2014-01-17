//---------------------------------------------------------------------------
#ifndef ObjectStreamsH
#define ObjectStreamsH
//---------------------------------------------------------------------------
#include <string>
#include <vector>
#include <string.h>
//---------------------------------------------------------------------------
#include "PTypes.h"
#include "HashTables.h"
//---------------------------------------------------------------------------

using namespace std;

class Object;
class Persistent;
class MetaClass;
class StreamClassTable;


/// global file revision. not used any longer (must be 200, see .cpp file); now replaced by per-class revision number.
extern const card32 CURRENT_FILE_VERSION;



/// helper class for object streams;
class  ObjectStream_ObjectHashComperator {
  public:
	  inline static bool equals(mpcard obj1, mpcard obj2){return obj1 == obj2;}
};

/// helper class for object streams;
class  ObjectStream_ObjIDHashTable : public HashTable<mpcard, Object*, ObjectStream_ObjectHashComperator> {
  public:
    virtual max_platform_card hashFunction(mpcard objID) const;
};



/// class that allows a serialization of objects of type "Persistent" (output version).
class  OutputObjectStream {
 private:
   ObjectStream_ObjIDHashTable objIdTable;

 protected:
   StreamClassTable *classTable;
   bool doWriteClassTable;
   bool perObjectClassVersion;
   bool externClassTable;

   /// some initialization during construction, to be called in subclass after stream has been opened / established
   void initStream();
   /// reserve space for pointer to class table (in non-seeking files, a all-classes table might be submitted at this point)
   /// (only called if constructor parameter doWriteClassTable == true)
   virtual void writeClassTableHeader() = 0;
   /// write the class table after all objects and their classes have been recorded
   /// (only called if constructor parameter doWriteClassTable == true)
   virtual void writeClassTable() = 0;

 public:
   /// Construct stream; usually, the default parameters should be used.
   /// For special applications, the classTable (a dictionary of classes included in the stream, beeing updated during writing)
   /// can be specified manually and its (default) inclusion in the stream can be ommited (e.g. if multiple streams are
   /// packed into one file, only one class table might be diserable).
   OutputObjectStream(bool perObjectClassVersion = false, StreamClassTable *classTable = NULL, bool doWriteClassTable = true);

   /// write a buffer of bytes. to be overridden in descendant classes.
   virtual void writeBuffer(const void* buf, mpcard length) = 0;

   /// write arbitrary data types as block of bytes
   template <class ElementaryType>
   void write(const ElementaryType &v) {
      writeBuffer((const void*)(&v), sizeof(ElementaryType));
   };

   /// write boolean value
   virtual void writeBool(const bool &v);
   /// write an stl string
   virtual void writeString(const string &v);
   /// write an object (including class id, multiple references are only written once and tagged with an id)
   virtual void writeObject(const Persistent *obj);
   /// write a meta class (class id)
   virtual void writeMetaClass(const MetaClass *mc);

   /// include a meta class in the class id table (although the class is never encountered by writeObject/writeMetaClasss);
   /// usefull for embedded classes with versioning
   virtual void includeClass(const MetaClass *mc);
   /// write a cardinal value with maximum bit length of the current platform;
   /// in the stream, a 64-bit representation is always used
   virtual void writeMaxPlatformCard(const max_platform_card &v);
   /// as writeMaxPlatformCard, but for signed values
   virtual void writeMaxPlatformInt(const max_platform_int &v);

   /// add an object that is not stored in the stream but only as a fixed pointer (obj)
   /// index is an number >= 0 that identifies the object uniquely (also after serialization and reconstruction)
   virtual void addGlobalObject(Persistent *obj, mpcard index);

	/// this function returns a filename if the stream is being stored to a file
	/// if not, an empty string is returned
	virtual string getFileName();

   /// close stream again
   virtual ~OutputObjectStream();
};



/// class that allows a serialization of objects of type "Persistent" (input version).
class  InputObjectStream {
 private:
   ObjectStream_ObjIDHashTable objIdTable;
   card32 version;

   /// type to distinguish between major revisions of the streaming system
   /// will be set according to the first 15 characters of the file, as in the suffix below

   enum MajorStreamVersion {
      /// initial value, at streaming-time, it causes an exception      
      SV_UNDEFINED, 
      /// original streaming format, restricted to 32 bit versions of XGRT/GeoX
      SV_XRTObjectStream, 
      /// the new streaming format (default for all newly created streams), 64 bit capable
      SV_XGRTObjStream64
   };
   MajorStreamVersion majorStreamVersion;


 protected:
   const MetaClass *lastClass;
   StreamClassTable *classTable;
   bool perObjectClassVersion;
   bool externClassTable;

   /// some initialization during construction, to be called in subclass after stream has been opened / established
   void initStream();
   /// retrieve class table from stream (called only if a class table has been included by writing)
   virtual void readClassTable() = 0;

 public:
   /// Construct stream object. If the stream to be read does not include a class table, a matching object has to be specified
   /// to the constructor. By default, all streams include a class table, the parameter might be NULL.
   InputObjectStream(StreamClassTable *classTable = NULL);

   /// read a buffer of bytes. to be overridden in descendant classes.
   virtual void readBuffer(void* buf, mpcard length) = 0;

   /// get the version of the stream itself (for new streams currently constantly == 200, legacy streams have smaller values)
   card32 getVersion() {return version;}

   /// read any block of data (not containing pointers or objects that need special construction!)
   template <class ElementaryType>
   void read(ElementaryType &v) {
      readBuffer((void*)(&v), sizeof(ElementaryType));
   };

   /// read a boolean value
   virtual void readBool(bool &v);
   /// read an stl string
   virtual void readString(string &v);
   /// read an object (including class id, multiple references to the same object are reconstructed properly)
   virtual void readObject(Persistent *&obj);
   /// template to avoid conversion issues with readObject
   template <class T>
   inline void readAnyObject(T *&obj) {Persistent *p; readObject(p); pAssert(p == NULL || p->getInstanceClass()->inheritsFrom(T::getClass())); obj = (T*)p;}
   /// write a meta class (class id)
   virtual void readMetaClass(const MetaClass *&mc);
   /// read a cardinal value with maximum bitlength of the current platform;
   /// in the stream, a 64-bit representation is always used
   virtual void readMaxPlatformCard(max_platform_card &v);
   /// as readMaxPlatformCard, but for signed values
   virtual void readMaxPlatformInt(max_platform_int &v);

   /// returns the version of the class at the time of serialization
   /// If perObjectClassVersion has been true for the construction of the writing stream,
   /// each object might belong to a separate class version. The number returned depends
   /// on the object currently beeing read (beginning to read an object pushes its version to
   /// a version stack, the end of reading the object pops the stack. At other times, the class
   /// version might be arbitrary. If no perObjectClassVersion has been set (default), the
   /// version number is constant for one and the same type.
   card32 getClassVersion(const MetaClass* metaClass);

   /// add an object that is not stored in the stream but only as a fixed pointer (obj)
   virtual void addGlobalObject(Persistent *obj, mpcard index);

	/// this function returns a filename if the stream is beeing read from a file
	/// if not, an empty string is returned
	virtual string getFileName();

   /// close stream again
   virtual ~InputObjectStream();
};





#endif
