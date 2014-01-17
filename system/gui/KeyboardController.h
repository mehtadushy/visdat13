//---------------------------------------------------------------------------
#ifndef KeyboardControllerH
#define KeyboardControllerH
//---------------------------------------------------------------------------
#include "GeneralKey.h"
//---------------------------------------------------------------------------


class KeyboardController { /* INTERFACE */
 public:
   virtual void keyDown(GeneralKey key) = 0;
   virtual void keyUp(GeneralKey key) = 0;
   virtual ~KeyboardController() {}
};


#endif
