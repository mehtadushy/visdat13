//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "GeneralKey.h"
//---------------------------------------------------------------------------
#include <QtCore/qnamespace.h>
//---------------------------------------------------------------------------
using namespace Qt;
//---------------------------------------------------------------------------


GeneralKey::GeneralKey(int virtualKeyCode, Modifiers mod) {
   this->mod = mod;
   switch (virtualKeyCode) {
      case Key_Backtab:      key = keyBack;       break;
      case Key_Tab:       key = keyTab;        break;
      case Key_Delete:     key = keyDelete;     break;
      case Key_Return:    key = keyReturn;     break;
      case Key_Shift:     key = keyShift;      break;
      case Key_Control:   key = keyControl;    break;
      case Key_Alt:      key = keyAlt;        break;
      case Key_Pause:     key = keyPause;      break;
      case Key_CapsLock:   key = keyCapsLock;   break;
      case Key_Escape:    key = keyEsc;        break;
      case Key_Space:     key = keySpace;      break;
      case Key_PageUp:     key = keyPageUp;     break;
      case Key_PageDown:      key = keyPageDown;   break;
      case Key_End:       key = keyEnd;        break;
      case Key_Home:      key = keyHome;       break;
      case Key_Left:      key = keyLeft;       break;
      case Key_Up:        key = keyUp;         break;
      case Key_Right:     key = keyRight;      break;
      case Key_Down:      key = keyDown;       break;
      case Key_Print:  key = keyPtrScrn;    break;
      case Key_Insert:    key = keyInsert;     break;
      case Key_F1:        key = keyF1;         break;
      case Key_F2:        key = keyF2;         break;
      case Key_F3:        key = keyF3;         break;
      case Key_F4:        key = keyF4;         break;
      case Key_F5:        key = keyF5;         break;
      case Key_F6:        key = keyF6;         break;
      case Key_F7:        key = keyF7;         break;
      case Key_F8:        key = keyF8;         break;
      case Key_F9:        key = keyF9;         break;
      case Key_F10:       key = keyF10;        break;
      case Key_F11:       key = keyF11;        break;
      case Key_F12:       key = keyF12;        break;
      case Key_0:   key = keyNumPad0;    break;
      case Key_1:   key = keyNumPad1;    break;
      case Key_2:   key = keyNumPad2;    break;
      case Key_3:   key = keyNumPad3;    break;
      case Key_4:   key = keyNumPad4;    break;
      case Key_5:   key = keyNumPad5;    break;
      case Key_6:   key = keyNumPad6;    break;
      case Key_7:   key = keyNumPad7;    break;
      case Key_8:   key = keyNumPad8;    break;
      case Key_9:   key = keyNumPad9;    break;
      case Key_Asterisk:  key = keyMult;       break;
      case Key_Plus:       key = keyPlus;       break;
      case Key_Minus:  key = keyMinus;      break;
      case Key_division:    key = keyDiv;        break;
    //  case VK_DECIMAL:   key = keyDecimal;    break;
      default:
         if (virtualKeyCode >= 0x30 && virtualKeyCode <= 0x5A) {
            key = keyAlphaNum;
            alphaNum = (char) virtualKeyCode;
         } else {
            key = keyUnknown;
         }
      break;
   }
   if (virtualKeyCode >= 0x30 && virtualKeyCode <= 0x5A) {
      alphaNum = (char) virtualKeyCode;
   }
}

