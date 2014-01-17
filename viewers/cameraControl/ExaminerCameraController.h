//---------------------------------------------------------------------------
#ifndef ExaminerCameraControllerH
#define ExaminerCameraControllerH
//---------------------------------------------------------------------------
#include "CameraController.h"
//---------------------------------------------------------------------------




class  ExaminerCameraController : public CameraController {
 private:
   static MetaClass *ExaminerCameraControllerMetaClass;
   static Object *newInstance();

   enum MouseState {neutral, dragDist, dragAngles, dragCenter};

   MouseState state;
   int32 lastX;
   int32 lastY;

   card32 mouseAreaWidth;
   card32 mouseAreaHeight;

   Vector3f center;
   float32 distance;
   Vector2f angles;
   float32 viewDirRot;

   float32 anglesIncrement;
   float32 anglePerScreenHeight;
   float32 distFactorPerScreenHeight;
   float32 distanceFactor;
   float32 absoluteCoordsIncrement;
   
   bool rotAroundZAxis;

 protected:
   void changed();
   void calcCamera();

 public:
   ExaminerCameraController();

   // Persistent:
   virtual const MetaClass *getInstanceClass() const;
   static const MetaClass *getClass();
   static void init(const MetaClass *parentClass);
   static void shutdown();

   // KeyboardController:
   virtual void keyDown(GeneralKey key);
   virtual void keyUp(GeneralKey key);

   // MouseController:
   virtual void mouseDown(int32 x, int32 y, MouseButtons buttonsState, Modifiers modifiersState);
   virtual void mouseMoved(int32 x, int32 y);
   virtual void mouseUp(int32 x, int32 y, MouseButtons buttonsState, Modifiers modifiersState);
   virtual void areaResize(card32 width, card32 height);

   // Editor:
   virtual void update();
   virtual void apply();
   virtual void setup( const Vector3f& position, const Vector3f& ref, const Vector3f& up );

   // Properties:
   float32 getDistance() const;
   void setDistance(float32 dist);
   void setToCamera(Camera* cam, Vector3f newCenter);
   float32 getAngle(card8 n) {return angles[n];}
   void setAngle(card8 n, float v) {angles[n] = v;}
};



#endif
