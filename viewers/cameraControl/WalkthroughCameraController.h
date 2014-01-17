//---------------------------------------------------------------------------
#ifndef WalkthroughCameraControllerH
#define WalkthroughCameraControllerH
//---------------------------------------------------------------------------
#include "CameraController.h"
#include "BoundingBox.h"
//---------------------------------------------------------------------------



class  WalkthroughCameraController : public CameraController {
 private:
   static MetaClass *WalkthroughCameraControllerMetaClass;
   static Object *newInstance();

   enum MouseState {neutral, dragDist, dragAngles};

   Vector3f camView;
   Vector3f camUp;
   Vector3f camPos;
   Vector3f viewRight;
   Vector3f viewDirHor;
   Vector3f viewDirUp;

   float32 angleIncr;
   float32 absoluteCoordsIncr;

   MouseState state;
   int32 lastX;
   int32 lastY;

   card32 mouseAreaWidth;
   card32 mouseAreaHeight;
   float32 anglePerScreenHeight;

 protected:
   void changed();

 public:
   WalkthroughCameraController();

   // Persistent:
   virtual const MetaClass *getInstanceClass() const;
   static const MetaClass *getClass();
   static void init(const MetaClass *parentClass);
   static void shutdown();

   // KeyboardController:
   virtual void keyDown(GeneralKey key);
   virtual void keyUp(GeneralKey key);

   void shiftCrtlPressed(GeneralKey key);
   void shiftPressed(GeneralKey key);
   void crtlPressed(GeneralKey key);
   void noKeyPressed(GeneralKey key);

   // MouseController:
   virtual void mouseDown(int32 x, int32 y, MouseButtons buttonsState, Modifiers modifiersState);
   virtual void mouseMoved(int32 x, int32 y);
   virtual void mouseUp(int32 x, int32 y, MouseButtons buttonsState, Modifiers modifiersState);
   virtual void areaResize(card32 width, card32 height);

   // Editor:
   virtual void update();
   virtual void apply();
   void setup( const Vector3f& position, const Vector3f& ref, const Vector3f& up );

   // Properties:
   void setToCamera(Camera* cam, Vector3f newCenter);
};



#endif

