//---------------------------------------------------------------------------
#ifndef HashTablesH
#define HashTablesH
//---------------------------------------------------------------------------
#include <vector>
#include <list>
#include "PTypes.h"
#include "PAssert.h"
//---------------------------------------------------------------------------

using namespace std;

/// declare class outside because of linker errors for externally defined constructors of embedded classes
template <class KeyType, class DataType>
class HashTable__Entry__outside {
 public:
   KeyType  key;
   DataType data;
   max_platform_int nextEntry;
   HashTable__Entry__outside(KeyType key, DataType data, max_platform_int nextEntry);

};

template <class KeyType, class DataType, class KeyComparator>
class HashTable {
 private:
   typedef HashTable__Entry__outside<KeyType, DataType> Entry;

   static const max_platform_card tableSizes[33];

   max_platform_card fillFactor;
   max_platform_card tableSize;
   max_platform_card currentSizeIndex;
   vector<max_platform_int> hashTable;
   vector<Entry> allEntries;

   void checkEnlarge();
   void checkShrink();
   void rebuildTable();

   void insertEntry(max_platform_card htIndex, max_platform_int entryIndex);
   void removeEntriesIndex(max_platform_int pos);


 protected:
   /** hash function KeyType --> max_platform_int, to be overwritten in descendants. */
   virtual max_platform_card hashFunction(KeyType key) const = 0;

 public:
   /// constructor
   HashTable(max_platform_card fillFactor = 1, max_platform_card startSizeIndex = 0);
   /// copy constructor
   HashTable(const HashTable<KeyType, DataType, KeyComparator> &init);
   /// assignment
   void operator=(const HashTable<KeyType, DataType, KeyComparator> &other);
   /// clear table
   void clear();
   /// get num elements
   max_platform_card count() const {return (max_platform_card)allEntries.size();}

   /// elements per slot / time-memory trade-off
   max_platform_card getFillFactor() {return fillFactor;}
   /// elements per slot / time-memory trade-off
   void setFillFactor(card32 fillFactor) {this->fillFactor = fillFactor;}


   /** addElement: if key is not in the table then (key, data) is added to the table,
     * if key is allready in the table then data is updated to the value given.
     * return value is an (abstract) pointer to the new entry. */
   void setElement(KeyType key, DataType data);
   /// alias for compatibility
   inline void addElement(KeyType key, DataType data) {setElement(key, data);}

   /** retrieve element from the table. if key is not found the result is false, otherwise
     * the result is true and data is set to the data stored with the key given. */
   bool getElement(KeyType key, DataType &data) const;
   /// alias for compatibility
   inline bool getData(KeyType key, DataType &data) const {return getElement(key, data);}

   /** deleteElement: if the key is in the table then (key, data) is removed from the table.
     * the return value is true if the element has been found and deleted. */
   bool deleteElement(KeyType key);

   max_platform_card getNumElements() const {return (max_platform_card)allEntries.size();}
   KeyType getKey(max_platform_card pos) {return allEntries[pos].key;}
   DataType getData(max_platform_card pos) {return allEntries[pos].data;}

   typedef void (*EntryCallback)(KeyType key, DataType data);
   void forEachEntry(EntryCallback cb);

   typedef void (*ExtendedEntryCallback)(void* context, KeyType key, DataType data);
   void forEachEntry(void* context, ExtendedEntryCallback cb);

   KeyType getKey(card32 entryIndex) const {return allEntries[entryIndex].key;}
   DataType getData(card32 entryIndex) const {return allEntries[entryIndex].data;}


   virtual ~HashTable();
};

extern void HashTable_UnitTest();



#include "HashTables.hpp"

#endif






