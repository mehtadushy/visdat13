//---------------------------------------------------------------------------
#include "stdafx.h"

//---------------------------------------------------------------------------
#include "SparseLinearAlgebra.h"
#include "SparseLinearAlgebra.hpp"
//---------------------------------------------------------------------------
#include "GeoXOutput.h"
#include "Random.h"
//---------------------------------------------------------------------------



void sparse_linear_algebra_unitTest() {
   SparseVectorF test;
   output << "SparseLinearAlgebraUnitTest\n---------------------------\n\n";
   output << "nullVector: " << test.toString() << "\nwith zeros " << test.toString(true,10) << "\n\n";
   for (unsigned i=1; i<5; i++) {
      test.setEntry(i*2, (float32)i);
   }
   output << "testVector: " << test.toString() << "\nwith zeros " << test.toString(true, 10) << "\n\n";
   test *= 2;
   output << "mult *2: " << test.toString() << "\nwith zeros " << test.toString(true, 10) << "\n\n";
   test /= 2;
   output << "div /2: " << test.toString() << "\nwith zeros " << test.toString(true, 10) << "\n\n";
   SparseVectorF test2 = test;
   SparseVectorF test3;
   test3 = test;
   pAssert(test == test2 && test == test3 && test2 == test3);
   output << "test2: " << test2.toString() << "\nwith zeros " << test2.toString(true, 10) << "\n\n";
   output << "test3: " << test3.toString() << "\nwith zeros " << test3.toString(true, 10) << "\n\n";
   test3 *= 0;
   pAssert(test3 != test2);
   test2 *= 0;
   pAssert(test3 == test2);

   output << "\n--------\n\n";

   SparseVectorF test4;
   for (unsigned i=1; i<5; i++) {
      test2.setEntry(i, (float32)i);
      test4.setEntry(i*3, (float32)i*i);
   }

   test3 = test2.componentProduct(test4);
   output << "test2: " << test2.toString() << "\nwith zeros " << test2.toString(true, 15) << "\n\n";
   output << "test4: " << test4.toString() << "\nwith zeros " << test4.toString(true, 15) << "\n\n";
   output << "test2 * test4: " << test3.toString() << "\nwith zeros " << test3.toString(true, 15) << "\n\n";

   test3 = test2 + test4;
   output << "test2 + test4: " << test3.toString() << "\nwith zeros " << test3.toString(true, 15) << "\n\n";
   test3 = test4 + test2;
   output << "test4 + test2: " << test3.toString() << "\nwith zeros " << test3.toString(true, 15) << "\n\n";

   test3 = test2 - test4;
   output << "test2 - test4: " << test3.toString() << "\nwith zeros " << test3.toString(true, 15) << "\n\n";

   test3 = test4 - test2;
   output << "test4 - test2: " << test3.toString() << "\nwith zeros " << test3.toString(true, 15) << "\n\n";

   DVectorD testD[10];
   SparseVectorD testS[10];
   for (int i=0; i<10; i++) {
      testD[i] = nullDVector<double>(10);
   }
   DVectorD testEqD;
   SparseVectorD testEqS;
   for (int i=0; i<50000; i++) {
      int action = random(20);
      int target = random(10);
      int targetA = random(10);
      int targetB = random(10);
      switch (action) {
       case 0: case 14: case 15: case 16: case 17: case 18: case 19: {
         for (int j=0; j<10; j++) {
            double v = rnd01();
            if (rnd01() > 0.6f) {
               testD[target][j] = v;
               testS[target].setEntry(j, v);
            } else {
               testD[target][j] = 0;
               testS[target].setEntry(j, 0);
            }
         }
         testS[target].checkConsistency();
         pAssert(testS[target] == testD[target]);
         testEqD = testD[target];
         testEqS = testS[target];
         testEqS.checkConsistency();
         pAssert(testEqS == testEqD);
         int modEntry = random(10);
         if (rnd01() > 0.5) {
            testEqS.setEntry(modEntry, testEqS.getEntry(modEntry) - 1.0);
         } else {
            testEqD[modEntry] -= 1.0;
         }
         testEqS.checkConsistency();
         pAssert(testEqS != testEqD);
         break;
       }
       case 1: {
         testD[target] = testD[targetA] + testD[targetB];
         testS[target] = testS[targetA] + testS[targetB];
         testS[target].checkConsistency();
         pAssert(testS[target] == testD[target]);
         break;
       }
       case 2: {
         testD[target] = testD[targetA] - testD[targetB];
         testS[target] = testS[targetA] - testS[targetB];
         testS[target].checkConsistency();
         pAssert(testS[target] == testD[target]);
         break;
       }
       case 3: {
         double d = testD[targetA] * testD[targetB];
         double s = testS[targetA] * testS[targetB];
         double sd = testS[targetA] * testD[targetB];
         pAssert(s == d && s == sd);
         break;
       }
       case 4: {
         testD[target] = -testD[target];
         testS[target] = -testS[target];
         testS[target].checkConsistency();
         pAssert(testS[target] == testD[target]);
         break;
       }
       case 5: {
         testD[target] = -testD[target];
         testS[target] = -testS[target];
         testS[target].checkConsistency();
         pAssert(testS[target] == testD[target]);
         break;
       }
       case 6: {
         testD[target] = testD[target]*3.14;
         testS[target] = testS[target]*3.14;
         testS[target].checkConsistency();
         pAssert(testS[target] == testD[target]);
         break;
       }
       case 7: {
         testD[target] = testD[target]/3.14;
         testS[target] = testS[target]/3.14;
         testS[target].checkConsistency();
         pAssert(testS[target] == testD[target]);
         break;
       }
       case 8: {
         testD[target] = testD[targetA].componentProduct(testD[targetB]);
         testS[target] = testS[targetA].componentProduct(testS[targetB]);
         testS[target].checkConsistency();
         pAssert(testS[target] == testD[target]);
         break;
       }
       case 9: {
         testD[target] += testD[targetA];
         testS[target] += testS[targetA];
         testS[target].checkConsistency();
         pAssert(testS[target] == testD[target]);
         break;
       }
       case 10: {
         testD[target] -= testD[targetA];
         testS[target] -= testS[targetA];
         testS[target].checkConsistency();
         pAssert(testS[target] == testD[target]);
         break;
       }
       case 11: {
         testD[target] *= 3.14;
         testS[target] *= 3.14;
         testS[target].checkConsistency();
         pAssert(testS[target] == testD[target]);
         break;
       }
       case 12: {
         testD[target] /= 3.14;
         testS[target] /= 3.14;
         testS[target].checkConsistency();
         pAssert(testS[target] == testD[target]);
         break;
       }
       case 13: {
         for (int g=0; g<10; g++) {
            pAssert(testS[g] == testD[g]);
            testS[g].removeZeros();
            testS[g].checkConsistency();
            pAssert(testS[g] == testD[g]);
         }
         break;
       }

      }
   }
   for (card32 i=0; i<10; i++) {
      output << testD[i].toString() << "\n";
      output << testS[i].toString(true, 10) << "\n";
      output.cr();
      pAssert(testS[i] == testD[i]);
   }


   output << "\n\n--------------- Matrix Test ---------------\n\n";


   SparseMatrixD M(5);

   output << M.toString(true, 5) << "\n\n";

   unsigned rows[5] = {0,3,2,4,1};

   for (card32 i=0; i<5; i++) {
      M[i].setEntry(rows[i], i+1);
   }

   output << M.toString(true, 5) << "\n\n";

   DVectorD v = scalarDVector<double>(5, 1.0);

   output << v.toString() << "\n\n";

   DVectorD v2 = M*v;

   output << v2.toString() << "\n\n";

   SparseVectorD v3;

   for (unsigned i=0; i<6; i++) {
      v3.setEntry(i, 1);
   }

   DVectorD v4 = M*v3;

   output << v4.toString() << "\n\n";

   pAssert(M == M);
   pAssert(!(M != M));

   M += M;
   output << M.toString(true, 5) << "\n\n";
   M *= 2;
   output << M.toString(true, 5) << "\n\n";
   M /= 2;
   output << M.toString(true, 5) << "\n\n";
   M.setToComponentProduct(M);
   output << M.toString(true, 5) << "\n\n";
   M -= M;
   output << M.toString(true, 5) << "\n\n";

   for (card32 i=0; i<5; i++) {
      M[i].setEntry(rows[i], i+1);
   }

   output << M.toString(true, 5) << "\n\n";

   M.clear();

   output << M.toString(true, 5) << "\n\n";

   M.clear();
   for (card32 i=0; i<5; i++) {
      M[i].setEntry(rows[i], i+1);
      M[rows[i]].setEntry(i, M[rows[i]].getEntry(i) + i+1);
   }

   output << M.toString(true, 5) << "\n\n";

   for (card32 k=0; k<10; k++) {
      for (card32 i=0; i<5; i++) {
         v[i] = rnd01()-0.5f;
      }

      for (card32 i=0; i<10; i++) {
         for (card32 j=0; j<5; j++) {
            v = M*v;
            v /= norm(v);
         }
         output << v.toString();
         output << " - lambda1: " << (long double)norm(M*v) / norm(v) << "\n";
      }
      output.cr();
   }
   output.cr();
}
