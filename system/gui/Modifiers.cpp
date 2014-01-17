//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "Modifiers.h"
//---------------------------------------------------------------------------


Modifiers::Modifiers(Qt::KeyboardModifiers qtModifiers) {
   modstate = 0;
   if ((qtModifiers & Qt::ShiftModifier) != 0) {
      modstate |= shift;
   }
   if ((qtModifiers & Qt::AltModifier) != 0) {
      modstate |= alt;
   }
   if ((qtModifiers & Qt::ControlModifier) != 0) {
      modstate |= ctrl;
   }
}
