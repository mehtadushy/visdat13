//---------------------------------------------------------------------------
#ifndef MouseControllerH
#define MouseControllerH
//---------------------------------------------------------------------------
#include "MouseButtons.h"
#include "Modifiers.h"
#include "PTypes.h"
//---------------------------------------------------------------------------



class  MouseController { /* INTERFACE */
 public:
   virtual void mouseDown(int32 x, int32 y, MouseButtons buttonsState, Modifiers modifiersState) = 0;
   virtual void mouseMoved(int32 x, int32 y) = 0;
   virtual void mouseUp(int32 x, int32 y, MouseButtons buttonsState, Modifiers modifiersState) = 0;
   virtual void areaResize(card32 width, card32 height) = 0;
   virtual ~MouseController() {}
};


class  MouseControllerFloat { /* INTERFACE */
 public:
   virtual void mouseDown(float32 x, float32 y, MouseButtons buttonsState, Modifiers modifiersState) = 0;
   virtual void mouseMoved(float32 x, float32 y) = 0;
   virtual void mouseUp(float32 x, float32 y, MouseButtons buttonsState, Modifiers modifiersState) = 0;
   virtual void areaResize(float32 width, float32 height) = 0;
   virtual ~MouseControllerFloat() {}
};



#endif
