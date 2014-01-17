//---------------------------------------------------------------------------
#ifndef StreamClassTableH
#define StreamClassTableH
//---------------------------------------------------------------------------
#include "Persistent.h"
#include "HashTables.h"
//---------------------------------------------------------------------------



class StreamClassTable : public Persistent {
 private:
   static MetaClass *StreamClassTableMetaClass;
   static Object* newInstance();

 public:
   struct MetaClassEntry {
      const MetaClass *mc;
      card32 currentStreamIDNumber;
      card32 mainVersion;
      vector<card32> versionStack;
   };

 private:
   class  ClassHashComperator {
     public:
       static bool equals(const MetaClass* obj1, const MetaClass* obj2) { return obj1 == obj2; }
   };

   class  ClassCardHashTable : public HashTable<const MetaClass*, card32, ClassHashComperator> {
     public:
       virtual max_platform_card hashFunction(const MetaClass* metaClass) const {
         return (max_platform_card)(metaClass);
       }
   };

   vector<MetaClassEntry> classTable;
   ClassCardHashTable idByClass;

 public:

   StreamClassTable() {}

   virtual const MetaClass *getInstanceClass() const;
   static const MetaClass *getClass();
   static void init(const MetaClass *parentClass);
   static void shutdown();

   /// Get unique id number for the given class.
   /// The id is unique over the lifetime of this object.
   /// If the class is unknown, a new id is created.
   /// (this also holds if the stream class table has been serialized and reread)
   card32 getOrCreateClassID(const MetaClass* mc);
   /// Get id number for the given class.
   /// If the class is unknown, BAD_CLASS == 0xFFFFFFFF is returned
   /// (this also holds if the stream class table has been serialized and reread)
   card32 getClassID(const MetaClass* mc) const;
   static const card32 BAD_CLASS = 0xFFFFFFFF;

   /// Get class by id. This function must only be called if the
   /// class has been added before by "getClassID()" or "includeClassID()"
   /// (this also holds if the stream class table has been serialized and reread)
   const MetaClass *getClass(card32 id) const;

   /// Retrieve number of known class ids. An id is valid if it is smaller than the number of ids.
   card32 getNumClassIDs() const;

   /// Temporarly set a new version number for the specified class (the class must be known, i.e. have an id).
   /// Temporary versions are not serialized.
   /// This could be used for per-object versioning.
   void pushVersion(const MetaClass *mc, card32 currentVersion);

   /// Temporarly set a new version number for the specified class.
   /// Temporary versions are not serialized.
   /// This could be used for per-object versioning.
   void pushVersion(card32 id, card32 currentVersion);

   /// Reset temporary version to the last set one (the class must be known, i.e. have an id).
   /// An exception is raised if no corresponding push is found (stack empty).
   /// This could be used for per-object versioning.
   void popVersion(const MetaClass *mc);

   /// Reset temporary version to the last set one.
   /// An exception is raised if no corresponding push is found (stack empty).
   /// This could be used for per-object versioning.
   void popVersion(card32 id);

   /// Returns current class version (the class must be known, i.e. have an id).
   /// In case a temporary version has been set, the top of the version stack is returned,
   /// otherwise the version at first introduction of the class due to "getClassID()" is returned.
   card32 getCurrentVersion(const MetaClass *mc) const;

   /// Returns current class version.
   /// In case a temporary version has been set, the top of the version stack is returned,
   /// otherwise the version at first introduction of the class due to "getClassID()" is returned.
   card32 getCurrentVersion(card32 id) const;

   /// write to disc (compatible with former "ObjectStreams.h" class tables)
   virtual void write(OutputObjectStream *s) const;
   /// read from disc (compatible with former "ObjectStreams.h" class tables)
   virtual void read(InputObjectStream *s);

   ~StreamClassTable() {}
};




#endif
