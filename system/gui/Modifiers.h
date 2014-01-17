//---------------------------------------------------------------------------
#ifndef ModifiersH
#define ModifiersH
//---------------------------------------------------------------------------
#include "PTypes.h"
#include <QtCore/Qt>
//---------------------------------------------------------------------------



class  Modifiers {
 private:
   static const card8 shift = 1;
   static const card8 alt = 2;
   static const card8 ctrl = 4;
   card8 modstate;
 public:
   inline Modifiers() {modstate = 0;}
   inline Modifiers(bool ext_shift, bool ext_alt, bool ext_ctrl) {
      modstate = 0;
      if (ext_shift)  modstate =  modstate | shift;
      if (ext_alt) modstate = modstate | alt;
      if (ext_ctrl) modstate = modstate | ctrl;
   }
   Modifiers(Qt::KeyboardModifiers qtModifiers);
   inline bool getShift() {
      return ((modstate & shift) != 0);
   }
   inline bool getCtrl() {
      return ((modstate & ctrl) != 0);
   }
   inline bool getAlt() {
      return ((modstate & alt) != 0);
   }
   inline bool neutral() {
      return modstate == 0;
   }
};


#endif
