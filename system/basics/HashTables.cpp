//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "HashTables.h"
#include "HashTables.hpp"
//---------------------------------------------------------------------------
#include "Random.h"
//---------------------------------------------------------------------------

struct TestKeyComp {
   static bool equals(card32 pos1, card32 pos2) {
      return pos1 == pos2;
   }
};

class HT2Test : public HashTable<card32, card32, TestKeyComp> {
 public:
   virtual max_platform_card hashFunction(card32 key) const {
      return key;
   }
   HT2Test(max_platform_card fillFactor = 1, max_platform_card startSizeIndex = 0) : HashTable<card32, card32, TestKeyComp>(fillFactor, startSizeIndex) {}
};


void HashTable_UnitTest() {
   #define SIZE 50
   vector<card32> delCand;

   card32 *rndTest = new card32[SIZE];
   for (card32 i=0; i<SIZE; i++) {
      rndTest[i] = randomNumber(10000);
   }
   HT2Test test(2, 0);
   card32 testValue = 0;
   for (card32 i=0; i<SIZE; i++) {
      if (randomNumber(3) <= testValue) {
         if (delCand.size() > 0) {
            card32 pos = randomNumber((card32)delCand.size());
            vector<card32>::iterator posit = delCand.begin(); posit += pos;
            test.deleteElement(*posit);
            delCand.erase(posit);
         }
      } else {
         test.addElement(rndTest[i], i);
         delCand.push_back(rndTest[i]);
      }
      if (i == SIZE/2)
         testValue = 10;
   }
   delete[] rndTest;
}
