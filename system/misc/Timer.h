//---------------------------------------------------------------------------
#ifndef TimerH
#define TimerH
//---------------------------------------------------------------------------
#include "PTypes.h"
//---------------------------------------------------------------------------



class  Timer {
 private:
   card32 lastTime;

 public:
   Timer();
   card32 getCurrentValue();
   card32 getDeltaValue(card32 lastValue);
   card32 getDeltaValue();
   ~Timer();
};



#endif
