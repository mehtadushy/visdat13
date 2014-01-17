//---------------------------------------------------------------------------
#ifndef CameraControllerH
#define CameraControllerH
//---------------------------------------------------------------------------
#include "Editor.h"
#include "Camera.h"
#include "Delegate.h"
#include "KeyboardController.h"
#include "MouseController.h"
#include "LinearAlgebra.h"
//---------------------------------------------------------------------------



class  CameraController : public Persistent, public Editor, public KeyboardController, public MouseController {
 private:
   static MetaClass *CameraControllerMetaClass;
   QDelegate notifyOnChange;

 protected:
   Camera *camera;
   void cameraChange();

 public:
   CameraController();

   virtual const MetaClass *getInstanceClass() const;
   static const MetaClass *getClass();
   static void init(const MetaClass *parentClass);
   static void shutdown();

   virtual void bindCamera(Camera *cam);
   virtual Object *getObject();
   void setNotifyOnCameraChange(QDelegate notifyOnChange);
   QDelegate getNotifyOnCameraChange();

   /*** Member access ***/
   virtual void setTime( float32 currentTime );
   virtual void setup( const Vector3f& position, const Vector3f& ref, const Vector3f& up ) = 0;
   Camera* getCamera(){ return camera; };
};













#endif
