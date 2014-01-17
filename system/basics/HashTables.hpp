//---------------------------------------------------------------------------
#ifndef HashTables_templCodeH
#define HashTables_templCodeH
//---------------------------------------------------------------------------
#include "HashTables.h"
//---------------------------------------------------------------------------
#undef DEBUG_CHECK_HASH_TABLE2
//---------------------------------------------------------------------------

using namespace std;


template <class KeyType, class DataType>
HashTable__Entry__outside<KeyType, DataType>::HashTable__Entry__outside(KeyType key, DataType data, max_platform_int nextEntry) {
   this->key = key;
   this->data = data;
   this->nextEntry = nextEntry;
}

template <class KeyType, class DataType, class KeyComparator>
const max_platform_card HashTable<KeyType, DataType, KeyComparator>::tableSizes[33] =
      {1, 2, 3, 5, 11, 17, 37, 67, 131, 257, 521, 1031, 2053, 4099,
      8209, 16411, 32771, 65537, 131101, 262147, 524309, 1048583,
      2097169, 4194319, 8388617, 16777259, 33554467, 67108879, 134217757,
      268435459, 536870923, 1073741827, 2147483659};      // Problem? last Number > maxInt

template <class KeyType, class DataType, class KeyComparator>
HashTable<KeyType, DataType, KeyComparator>::HashTable(max_platform_card fillFactor, max_platform_card startSizeIndex) {
   this->fillFactor = fillFactor;
   currentSizeIndex = startSizeIndex;
   tableSize = tableSizes[currentSizeIndex];
   rebuildTable();
}


template <class KeyType, class DataType, class KeyComparator>
HashTable<KeyType, DataType, KeyComparator>::HashTable(const HashTable<KeyType, DataType, KeyComparator> &init) {
   fillFactor = init.fillFactor;
   currentSizeIndex = init.currentSizeIndex;
   tableSize = init.tableSize;
   hashTable = init.hashTable;
   allEntries = init.allEntries;
}


template <class KeyType, class DataType, class KeyComparator>
void HashTable<KeyType, DataType, KeyComparator>::insertEntry(max_platform_card htIndex, max_platform_int entryIndex)
{
   pAssert(hashTable.size() > htIndex);
   if (hashTable[htIndex] == -1) {
      hashTable[htIndex] = entryIndex;
      allEntries[entryIndex].nextEntry = -1;
   } else {
      pAssert(hashTable[htIndex] < (max_platform_int)allEntries.size());
      pAssert(entryIndex < (max_platform_int)allEntries.size());
      allEntries[entryIndex].nextEntry = hashTable[htIndex];
      hashTable[htIndex] = entryIndex;
   }
}


template <class KeyType, class DataType, class KeyComparator>
void HashTable<KeyType, DataType, KeyComparator>::rebuildTable() {
   hashTable.clear();
   hashTable.resize(tableSize, -1);

   const max_platform_card n = (max_platform_card)allEntries.size();
   for (max_platform_card i=0; i<n; i++) {
      max_platform_int place = hashFunction(allEntries[i].key) % tableSize;
      insertEntry(place, i);
   }
}


template <class KeyType, class DataType, class KeyComparator>
void HashTable<KeyType, DataType, KeyComparator>::operator=(const HashTable<KeyType, DataType, KeyComparator> &other) {
   fillFactor = other.fillFactor;
   currentSizeIndex = other.currentSizeIndex;
   tableSize = other.tableSize;
   hashTable = other.hashTable;
   allEntries = other.allEntries;
}


template <class KeyType, class DataType, class KeyComparator>
void HashTable<KeyType, DataType, KeyComparator>::clear() {
   allEntries.clear();
   hashTable.clear();
   currentSizeIndex = 0;
   tableSize = tableSizes[currentSizeIndex];
   rebuildTable();
}


template <class KeyType, class DataType, class KeyComparator>
void HashTable<KeyType, DataType, KeyComparator>::checkEnlarge() {
   if (allEntries.size() / fillFactor > tableSize) {
      currentSizeIndex++;
      tableSize = tableSizes[currentSizeIndex];
      rebuildTable();
   }
}


template <class KeyType, class DataType, class KeyComparator>
void HashTable<KeyType, DataType, KeyComparator>::checkShrink() {
   if (allEntries.size() / fillFactor < tableSize/4) {
      currentSizeIndex--;
      tableSize = tableSizes[currentSizeIndex];
      rebuildTable();
   }
}


template <class KeyType, class DataType, class KeyComparator>
void HashTable<KeyType, DataType, KeyComparator>::setElement(KeyType key, DataType data) {
   max_platform_int place = hashFunction(key) % tableSize;
   pAssert(place < (max_platform_int)hashTable.size());
   max_platform_int pos = hashTable[place];
   while (pos != -1) {
      pAssert((max_platform_int)allEntries.size() > pos);
      if (KeyComparator::equals(allEntries[pos].key, key) ) {
         allEntries[pos].data = data;
         return;
      }
      pos = allEntries[pos].nextEntry;
   }

   Entry e(key, data, -1);

   max_platform_int newElemPos = (max_platform_int)allEntries.size();
   allEntries.push_back(e);

   insertEntry(place, newElemPos);

   checkEnlarge();
}


template <class KeyType, class DataType, class KeyComparator>
void HashTable<KeyType, DataType, KeyComparator>::removeEntriesIndex(max_platform_int toRemove) {
   pAssert(allEntries.size() >= 1);
   pAssert((max_platform_int)allEntries.size() > toRemove);

   if (toRemove == ((max_platform_int)allEntries.size())-1) {

      allEntries.pop_back();

   } else {

      KeyType key = allEntries.back().key;
      max_platform_int place = hashFunction(key) % tableSize;
      pAssert(place < (max_platform_int)hashTable.size());
      max_platform_int pos = hashTable[place];
      max_platform_int *lastIndex = &hashTable[place];

      while (pos != -1) {
         if (KeyComparator::equals(allEntries[pos].key, key) ) {
            *lastIndex = toRemove;
            allEntries[toRemove] = allEntries.back();
            allEntries.pop_back();
            return;
         }
         lastIndex = &(allEntries[pos].nextEntry);
         pos = allEntries[pos].nextEntry;
      }
      pAssert(false);

   }
}


template <class KeyType, class DataType, class KeyComparator>
bool HashTable<KeyType, DataType, KeyComparator>::deleteElement(KeyType key) {
   max_platform_int place = hashFunction(key) % tableSize;
   pAssert(place < (max_platform_int)hashTable.size());
   max_platform_int pos = hashTable[place];
   max_platform_int *lastIndex = &hashTable[place];

   while (pos != -1) {
      pAssert((max_platform_int)allEntries.size() > pos);
      if (KeyComparator::equals(allEntries[pos].key, key) ) {
         *lastIndex = allEntries[pos].nextEntry;
         removeEntriesIndex(pos);
         checkShrink();
         return true;
      }
      lastIndex = &(allEntries[pos].nextEntry);
      pos = allEntries[pos].nextEntry;
   }

   return false;
}


template <class KeyType, class DataType, class KeyComparator>
bool HashTable<KeyType, DataType, KeyComparator>::getElement(KeyType key, DataType &data) const {
   max_platform_int place = hashFunction(key) % tableSize;
   pAssert(place < (max_platform_int)hashTable.size());
   max_platform_int pos = hashTable[place];
   while (pos != -1) {
      pAssert((max_platform_int)allEntries.size() > pos);
      if (KeyComparator::equals(allEntries[pos].key, key) ) {
         data = allEntries[pos].data;
         return true;
      }
      pos = allEntries[pos].nextEntry;
   }
   return false;
}

template <class KeyType, class DataType, class KeyComparator>
void HashTable<KeyType, DataType, KeyComparator>::forEachEntry(EntryCallback cb) {
   max_platform_card index = 0;
   while (index < allEntries.size()) {
      max_platform_card oldN = (max_platform_card)allEntries.size();
      cb(allEntries[index].key, allEntries[index].data);
      if (oldN <= allEntries.size()) {
         index++;
      }
      // otherwise: assume that an element has been deleted at the current position
   }
}

template <class KeyType, class DataType, class KeyComparator>
void HashTable<KeyType, DataType, KeyComparator>::forEachEntry(void* context, ExtendedEntryCallback cb) {
   max_platform_card index = 0;
   while (index < allEntries.size()) {
      max_platform_card oldN = (max_platform_card)allEntries.size();
      cb(context, allEntries[index].key, allEntries[index].data);
      if (oldN <= allEntries.size()) {
         index++;
      }
      // otherwise: assume that an element has been deleted at the current position
   }
}



template <class KeyType, class DataType, class KeyComparator>
HashTable<KeyType, DataType, KeyComparator>::~HashTable() {}




#endif





