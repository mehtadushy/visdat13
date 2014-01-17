//---------------------------------------------------------------------------
#include "stdafx.h"

//---------------------------------------------------------------------------
#include "WalkthroughCameraController.h"
//---------------------------------------------------------------------------
#include "LinearAlgebra.hpp"
#include "BoundingBox.hpp"
#include "FixedArrayClassProperty.h"
#include "ObjectViewsTable.h"
#include "Camera.h"
//---------------------------------------------------------------------------


MetaClass* WalkthroughCameraController::WalkthroughCameraControllerMetaClass = NULL;

const MetaClass* WalkthroughCameraController::getInstanceClass() const {
   return getClass();
}

Object *WalkthroughCameraController::newInstance() {
   return new WalkthroughCameraController;
}

const MetaClass* WalkthroughCameraController::getClass() {
   if (WalkthroughCameraControllerMetaClass==NULL) {
      WalkthroughCameraControllerMetaClass = new MetaClass("WalkthroughCameraController", "Camera Controller with a camera walking through a scene.",
                                          newInstance, sizeof(WalkthroughCameraController));
   }
   return WalkthroughCameraControllerMetaClass;
}

/****************************************************************************/
//  WalkthroughCameraController - init
/// This method initializes the properties
/****************************************************************************/
void WalkthroughCameraController::init(const MetaClass *parentClass) {
   ((MetaClass*)getClass())->init(parentClass);
   WalkthroughCameraControllerMetaClass->copyParentProperties();

	BEGIN_CLASS_INIT( WalkthroughCameraController );
	ADD_VECTOR3F_PROP_UPDATE( camPos, 0, &WalkthroughCameraController::changed );
	ADD_VECTOR3F_PROP_UPDATE( camView, 0, &WalkthroughCameraController::changed );
	ADD_VECTOR3F_PROP_UPDATE( camUp, 0, &WalkthroughCameraController::changed );
	ADD_FLOAT32_PROP_UPDATE( angleIncr,0, &WalkthroughCameraController::changed );
	ADD_FLOAT32_PROP_UPDATE( absoluteCoordsIncr,0, &WalkthroughCameraController::changed );

}

/****************************************************************************/
//  WalkthroughCameraController - shutdown
/// This method is used to shut down the WalkthroughCameraController when
/// it is finished
/****************************************************************************/
void WalkthroughCameraController::shutdown() {
   delete WalkthroughCameraControllerMetaClass;
}

/****************************************************************************/
//  WalkthroughCameraController - changed
/// Update the ObjectViewsTable and the camera
/****************************************************************************/
void WalkthroughCameraController::changed() {
   if (camera == NULL) return;
   ObjectViewsTable::update(camera);
   cameraChange();
}

/****************************************************************************/
//  WalkthroughCameraController - constructor
/// Initializing the variables
/****************************************************************************/
WalkthroughCameraController::WalkthroughCameraController() {
   mouseAreaWidth = 500;
   mouseAreaHeight = 500;

   state = neutral;

   camView = makeVector3f(0,0,1);
   camUp   = makeVector3f(0,1,0);
   camPos  = makeVector3f(0,0,0);

   viewDirUp = makeVector3f(0,0.3f,0);

   angleIncr = 0.05f;
   absoluteCoordsIncr = 1;
   anglePerScreenHeight = (float)M_PI;
}


/****************************************************************************/
//  WalkthroughCameraController - shiftCtrlPressed
/// This method is called when the Control- and Shift-Key has been pressed.
/****************************************************************************/
void WalkthroughCameraController::shiftCrtlPressed(GeneralKey key) {
   float32 delta = absoluteCoordsIncr;

   if( key.getKey() == ::keyLeft ) {
      camPos -= viewRight * delta;
      camView -= viewRight * delta;
   }
   else if( key.getKey() == ::keyRight ) {
      camPos += viewRight * delta;
      camView += viewRight * delta;
   }
   else if( key.getKey() == ::keyDown ) {
      camPos -= viewDirUp * delta;
      camView -= viewDirUp * delta;
   }
   else if( key.getKey() == ::keyUp ) {
      camPos += viewDirUp * delta;
      camView += viewDirUp * delta;
   }
   camera->setup( camPos, camView, camUp);

}

/****************************************************************************/
//  WalkthroughCameraController - shiftPressed
/// This method is called when the Shift-Key has been pressed.
/****************************************************************************/
void WalkthroughCameraController::shiftPressed(GeneralKey key) {
   viewRight = ( camView - camPos ).crossProduct(camUp);
   normalize(viewRight);

   float32 delta = absoluteCoordsIncr;

   if( key.getKey() == ::keyLeft ) {
      camPos -= viewRight * delta;
      camView -= viewRight * delta;
   }
   else if( key.getKey() == ::keyRight ) {
      camPos += viewRight * delta;
      camView += viewRight * delta;
   }
   else if( key.getKey() == ::keyDown ) {
      camPos -= viewDirUp * delta;
      camView -= viewDirUp * delta;
   }
   else if( key.getKey() == ::keyUp ) {
      camPos += viewDirUp * delta;
      camView += viewDirUp * delta;
   }
   camera->setup( camPos, camView, camUp);
}

/****************************************************************************/
//  WalkthroughCameraController - ctrlPressed
/// This method is called when the Control-Key has been pressed.
/****************************************************************************/
void WalkthroughCameraController::crtlPressed(GeneralKey key) {
   viewDirHor = makeVector3f( camView[0] - camPos[0], 0, camView[2] - camPos[2] );
   normalize(viewDirHor);
   viewRight = ( camView - camPos ).crossProduct(camUp);
   normalize(viewRight);
   Vector4f curr_camView = expand3To4(camView);
   Vector4f curr_camPos  = expand3To4(camPos);
   Matrix4f trans = makeTranslation4f(-camPos);

   float32 delta = absoluteCoordsIncr / 10;
   float32 deltaRot = angleIncr / 10;

   if( key.getKey() == ::keyUp ) {
      camPos += viewDirHor * delta;
      camView += viewDirHor * delta;
   }
   else if( key.getKey() == ::keyDown ) {
      camPos -= viewDirHor * delta;
      camView -= viewDirHor * delta;
   }
   else if( key.getKey() == ::keyLeft ) {
      curr_camView = ( makeRotY4f(deltaRot) * trans * curr_camView ) + curr_camPos;
      camUp = (makeRotY3f(deltaRot).transpose()) * camUp;
      camView = shrink4To3(curr_camView);
   }
   else if( key.getKey() == ::keyRight ) {
      curr_camView = ( makeRotY4f(-deltaRot) * trans * curr_camView ) + curr_camPos;
      camUp = (makeRotY3f(-deltaRot).transpose()) * camUp;
      camView = shrink4To3(curr_camView);
   }
   normalize(camUp);
   camera->setup(camPos, camView, camUp);
}

/****************************************************************************/
//  WalkthroughCameraController - noKeyPressed
/// This method is called when no Key has been pressed.
/****************************************************************************/
void WalkthroughCameraController::noKeyPressed(GeneralKey key) {
   float32 delta = absoluteCoordsIncr;
   float32 deltaRot = angleIncr;

   viewRight = ( camView - camPos ).crossProduct(camUp);
   normalize(viewRight);

   viewDirHor = makeVector3f( camView[0] - camPos[0], 0, camView[2] - camPos[2] );
   normalize(viewDirHor);

   Vector4f curr_camView = expand3To4(camView);
   Vector4f curr_camPos  = expand3To4(camPos);
   Matrix4f trans = makeTranslation4f(-camPos);

   if( key.getKey() == ::keyUp ) {
      camPos += viewDirHor * delta;
      camView += viewDirHor * delta;
   }
   else if( key.getKey() == ::keyDown ) {
      camPos -= viewDirHor * delta;
      camView -= viewDirHor * delta;
   }
   else if( key.getKey() == ::keyLeft ) {
      curr_camView = ( makeRotY4f(-deltaRot) * trans * curr_camView ) + curr_camPos;
      camUp = (makeRotY3f(-deltaRot).transpose()) * camUp;
      camView = shrink4To3(curr_camView);
   }
   else if( key.getKey() == ::keyRight ) {
      curr_camView = ( makeRotY4f(deltaRot) * trans * curr_camView ) + curr_camPos;
      camUp = (makeRotY3f(deltaRot).transpose()) * camUp;
      camView = shrink4To3(curr_camView);
   }
   normalize(camUp);
   camera->setup(camPos, camView, camUp);
}

/****************************************************************************/
//  WalkthroughCameraController - keyDown
/// This method is called when a key has been pressed down.
/****************************************************************************/
void WalkthroughCameraController::keyDown(GeneralKey key) {
   if( key.getModifiers().getShift() ) {
      if( key.getModifiers().getCtrl() ) {
         shiftCrtlPressed(key);
      }
      else {
         shiftPressed(key);
      }
   }
   else if( key.getModifiers().getCtrl() ) {
      crtlPressed(key);
   }
   else {
      noKeyPressed(key);
   }
   changed();
}

/****************************************************************************/
//  WalkthroughCameraController - keyUp
/// This method is called when a key has been released.
/****************************************************************************/
void WalkthroughCameraController::keyUp(GeneralKey key) {

}

/****************************************************************************/
//  WalkthroughCameraController - mouseDown
/// This method is called when a mouse button has been pressed down.
/****************************************************************************/
void WalkthroughCameraController::mouseDown(int32 x, int32 y, MouseButtons buttonsState, Modifiers modifiersState) {
   if (buttonsState.getLeft()) {
       state = dragAngles;
       lastX = x;
       lastY = y;
   }

}

/****************************************************************************/
//  WalkthroughCameraController - mouseMoved
/// This method is called when the mouse has been moved.
/****************************************************************************/
void WalkthroughCameraController::mouseMoved(int32 x, int32 y) {
   viewRight = ( camView - camPos ).crossProduct(camUp);
   normalize(viewRight);

   if (mouseAreaHeight <= 0) return;
   int32 deltaX = x - lastX;
   int32 deltaY = y - lastY;
   lastX = x;
   lastY = y;
   
   float32 camDist = norm(camView-camPos);

   if (state == dragAngles) {
      if( deltaX != 0 ) {
         float32 normDx = -deltaX * anglePerScreenHeight / mouseAreaHeight;
         camView = makeRotY3f(normDx) * normalize(camView-camPos) * camDist + camPos;
         camUp = normalize(makeRotY3f(normDx) * camUp);
         camera->setup( camPos, camView, camUp);
         changed();
      }
      if( deltaY != 0 ) {
         float32 normDy = deltaY * anglePerScreenHeight / mouseAreaHeight;
         Matrix3f rot_matrix_viewRight = makeRotVector3f(viewRight, -normDy);
         camView = rot_matrix_viewRight * normalize(camView-camPos) * camDist + camPos;
         camUp = normalize(makeRotVector3f(viewRight, -normDy) * camUp);
         camera->setup( camPos, camView, camUp);
         changed();
      }
   }
}

/****************************************************************************/
//  WalkthroughCameraController - mouseUp
/// This method is called when a mouse button has been released.
/****************************************************************************/
void WalkthroughCameraController::mouseUp(int32 x, int32 y, MouseButtons buttonsState, Modifiers modifiersState) {
   if (buttonsState.getLeft()) {
      state = neutral;
   }
}

/****************************************************************************/
//  WalkthroughCameraController - areaResize
/// This method is called when you want to resize the area in which the scene
/// is rendered.
/****************************************************************************/
void WalkthroughCameraController::areaResize(card32 width, card32 height) {
   this->mouseAreaWidth = width;
   this->mouseAreaHeight = height;
}


/****************************************************************************/
//  WalkthroughCameraController - update
/// This method does nothing.
/****************************************************************************/
void WalkthroughCameraController::update() {
}

/****************************************************************************/
//  WalkthroughCameraController - apply
/// This method applies the ObjectViewsTable, the calcCamera and the
/// cameraChange.
/****************************************************************************/
void WalkthroughCameraController::apply() {
   ObjectViewsTable::update(camera);
   cameraChange();
}

/****************************************************************************/
//  WalkthroughCameraController - setToCamera
/// This methods is used for the properties.
/****************************************************************************/
void WalkthroughCameraController::setToCamera(Camera* cam, Vector3f newCenter) {
}

void WalkthroughCameraController::setup( const Vector3f& position, const Vector3f& ref, const Vector3f& up )
{
	camView = ref;
	camUp = up;
	camPos = position;
	changed();
}

