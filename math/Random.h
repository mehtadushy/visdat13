//---------------------------------------------------------------------------
#ifndef RandomH
#define RandomH
//---------------------------------------------------------------------------
#include <stdlib.h>
//---------------------------------------------------------------------------


inline float32 rnd01() {
   return (float32)rand() / ((float32)(RAND_MAX)+1);
}

inline card32 randomNumber(card32 maxNumberPlusOne) {
	float32 ret = rnd01() * (float32)maxNumberPlusOne;
	return (card32)ret;
}

inline card32 random32bit(card32 maxNumberPlusOne = 0xffffffff)
{
	// rand() produces a pseudo random number between 0 and 2^15
	card32 ret = (rand() | 
		((rand() & 0x7fff) << 15 ) |
		((rand() & 0x7fff) << 30 )) % maxNumberPlusOne;
	return ret;
}

inline card64 random64bit(card64 maxNumberPlusOne = 0xffffffff)
{
   // rand() produces a pseudo random number between 0 and 2^15
   card64 ret = (rand() | 
      (((card64)(rand() & 0x7fff)) << 15 ) |
      (((card64)(rand() & 0x7fff)) << 30 ) |
      (((card64)(rand() & 0x7fff)) << 45 ) |
      (((card64)(rand() & 0x7fff)) << 60 ) ) % maxNumberPlusOne;
   return ret;
}

inline card32 random(card32 maxNumberPlusOne)
{
	return randomNumber(maxNumberPlusOne);
}




//---------------------------------------------------------------------------
#endif
