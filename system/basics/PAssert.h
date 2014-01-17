//---------------------------------------------------------------------------
#ifndef PAssertH
#define PAssertH
//---------------------------------------------------------------------------
#include "Exceptions.h"
//---------------------------------------------------------------------------


#if defined(_DEBUG)
inline void pAssert(bool cond) {
   if (!cond) {
      throw PException(string("Assertion failed."));
   }
}
#else
   #define pAssert(C)
#endif

#if defined(_DEBUG)
inline void pAssertMsg(bool cond, string msg) {
   if (!cond) {
      throw PException(msg);
   }
}
#else
   #define pAssertMsg(C, M)
#endif



#endif
