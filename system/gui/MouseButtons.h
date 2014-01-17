//---------------------------------------------------------------------------
#ifndef MouseButtonsH
#define MouseButtonsH
//---------------------------------------------------------------------------
#include "PTypes.h"
//---------------------------------------------------------------------------



class  MouseButtons {
 private:
   static const card8 leftMB = 1;
   static const card8 rightMB = 2;
   static const card8 leftMBdouble = 4;
   static const card8 middleMB = 8;
   card8 buttonstate;
 public:
   inline MouseButtons() {buttonstate =0;}
   inline MouseButtons(bool left, bool right, bool leftDouble = false, bool middle = false) {
      buttonstate = 0;
      if (left)  buttonstate |=  leftMB;
      if (middle) buttonstate |= middleMB;
      if (right) buttonstate |= rightMB;
      if (leftDouble) buttonstate |= leftMBdouble;
   }

   // VSPORT notice: is int correct ? previous definition: inline getLeft() ...
   inline int getLeft() {
      return ((buttonstate & leftMB) != 0);
   }
   inline int getMiddle() {
      return ((buttonstate & middleMB) != 0);
   }
   inline int getRight() {
      return ((buttonstate & rightMB) != 0);
   }
   inline int getLeftDouble() {
      return ((buttonstate & leftMBdouble) != 0);
   }
};



#endif
