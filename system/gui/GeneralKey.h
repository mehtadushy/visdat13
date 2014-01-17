//---------------------------------------------------------------------------
#ifndef GeneralKeyH
#define GeneralKeyH
//---------------------------------------------------------------------------
#include "Modifiers.h"
//---------------------------------------------------------------------------



typedef enum {keyUnknown,           /* unknown key code */
              keyAlphaNum,          /* a-z, 0-9 */
              keyUp, keyDown, keyLeft, keyRight, keyPageUp, keyPageDown, keyHome, keyEnd,
              keyEsc, keyTab, keyDelete, keyReturn, keyShift, keyControl, keyAlt, keyPause,
              keyCapsLock, keyPrtScrn, keyF1, keyF2, keyF3, keyF4, keyF5, keyF6, keyF7,
              keyF8, keyF9, keyF10, keyF11, keyF12, keyBack, keySpace, keyPtrScrn, keyInsert,
              keyNumPad0, keyNumPad1, keyNumPad2, keyNumPad3, keyNumPad4, keyNumPad5,
              keyNumPad6, keyNumPad7, keyNumPad8, keyNumPad9, keyMult, keyPlus, keyMinus,
              keyDiv, keyDecimal} KeyType;  /* special keys */


class  GeneralKey {
 private:
   char alphaNum;
   KeyType key;
   Modifiers mod;
 public:

    GeneralKey(int virtualKeyCode, Modifiers mod);

   KeyType getKey() {return key;}
   char getkeyAlphaNumKey() {
      if (key == keyAlphaNum)
         return alphaNum;
      else
         return 0;
   }
   Modifiers getModifiers() {return mod;}
};




#endif

