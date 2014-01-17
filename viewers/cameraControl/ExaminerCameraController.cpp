//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ExaminerCameraController.h"
//---------------------------------------------------------------------------
#include "FixedArrayClassProperty.h"
#include "NumericalClassProperty.h"
#include "BooleanClassProperty.h"
#include "ObjectViewsTable.h"
#include "LinearAlgebra.h"
#include "LinearAlgebra.hpp"
//---------------------------------------------------------------------------



MetaClass* ExaminerCameraController::ExaminerCameraControllerMetaClass = NULL;

const MetaClass* ExaminerCameraController::getInstanceClass() const {
   return getClass();
}

Object *ExaminerCameraController::newInstance() {
   return new ExaminerCameraController;
}

const MetaClass* ExaminerCameraController::getClass() {
   if (ExaminerCameraControllerMetaClass==NULL) {
      ExaminerCameraControllerMetaClass = new MetaClass("ExaminerCameraController", "Camera Controller with a camera focusing on a point of interest and rotating about that point.",
                                          newInstance, sizeof(ExaminerCameraController));
   }
   return ExaminerCameraControllerMetaClass;
}

void ExaminerCameraController::init(const MetaClass *parentClass) {
   ((MetaClass*)getClass())->init(parentClass, MetaClass::FIRST_VERSION + 1);
   ExaminerCameraControllerMetaClass->copyParentProperties();

	
	BEGIN_CLASS_INIT( ExaminerCameraController );
	ADD_VECTOR2F_PROP_UPDATE( angles,0, &ExaminerCameraController::changed );
	ADD_FLOAT32_PROP_METHOD( distance,0, &ExaminerCameraController::getDistance, &ExaminerCameraController::setDistance );
	ADD_FLOAT32_PROP_UPDATE( absoluteCoordsIncrement, 0, &ExaminerCameraController::changed );
	ADD_BOOLEAN_PROP( rotAroundZAxis, 1 );


}

void ExaminerCameraController::shutdown() {
   delete ExaminerCameraControllerMetaClass;
}

void ExaminerCameraController::calcCamera() {
   Vector3f camView = makeVector3f(0,0,1);
   Vector3f camUp = makeRotZ3f(viewDirRot) * makeVector3f(0,1,0);
   Vector3f camPos = makeVector3f(0,0,-distance);

   Matrix3f rot;
   if (rotAroundZAxis) {
		rot = (makeRotZ3f(-angles[0]).transpose()) * (makeRotX3f(angles[1]).transpose());
   } else {
		rot = (makeRotY3f(angles[0]).transpose()) * (makeRotX3f(angles[1]).transpose());
	}

   Vector3f transfCamPos = rot * camPos + center;
   float32 magnitude = norm(transfCamPos);
   if (magnitude <= 0.0f) magnitude = 1.0f;
   Vector3f lookAt =  rot * (camPos + camView*magnitude) + center;
   Vector3f transfCamUp =  rot * camUp;

   camera->setup(transfCamPos, lookAt, transfCamUp);
}

void ExaminerCameraController::changed() {
   if (camera == NULL) return;
   ObjectViewsTable::update(camera);
   calcCamera();
   cameraChange();
}


// ----


ExaminerCameraController::ExaminerCameraController() {
   mouseAreaWidth = 500;
   mouseAreaHeight = 500;

   state = neutral;

   center = makeVector3f(0.0, 0.0, 0.0);
   distance = 10.0;
   angles = makeVector2f((float32)M_PI, (float32)M_PI);//(float)M_PI/2.0f);
   anglesIncrement = 5.0f / 180.0f * (float)M_PI;
   anglePerScreenHeight = (float)M_PI;
   distFactorPerScreenHeight = 2;
   distanceFactor = 1.25;
   absoluteCoordsIncrement = 1;
   
   rotAroundZAxis = true;
   viewDirRot = 0.0f;
}

// ----


void ExaminerCameraController::keyDown(GeneralKey key) {
   if (key.getKey() == ::keyMinus) {
      if (key.getModifiers().getCtrl()) {
         absoluteCoordsIncrement /= 2;
      } else {
         absoluteCoordsIncrement /= 10;
      }
   } else if (key.getKey() == ::keyPlus) {
      if (key.getModifiers().getCtrl()) {
         absoluteCoordsIncrement *= 2;
      } else {
         absoluteCoordsIncrement *= 10;
      }
   } else if  (key.getKey() == ::keyHome) {
      if (key.getModifiers().getCtrl()) {
         viewDirRot += anglesIncrement/10;
      } else {
         viewDirRot += anglesIncrement;
      }
   } else if  (key.getKey() == ::keyEnd) {
      if (key.getModifiers().getCtrl()) {
         viewDirRot -= anglesIncrement/10;
      } else {
         viewDirRot -= anglesIncrement;
      }
   } 

   float32 effectiveDistanceFactor = distanceFactor;
   if (key.getModifiers().getCtrl()) effectiveDistanceFactor = pow(effectiveDistanceFactor, 0.1f);

   if  (key.getKey() == ::keyPageUp) {
     distance /= effectiveDistanceFactor;
   } else if  (key.getKey() == ::keyPageDown) {
     distance *= effectiveDistanceFactor;
   }

   float32 delta = absoluteCoordsIncrement;
   if (key.getModifiers().getCtrl()) delta /= 10.0f;

	Matrix3f rot;
   if (rotAroundZAxis) {
		rot = (makeRotZ3f(-angles[0]).transpose()) * (makeRotX3f(angles[1]).transpose());
	} else {
	   rot = (makeRotY3f(angles[0]).transpose()) * (makeRotX3f(angles[1]).transpose());
	}

   Matrix3f dir;
   dir[0] = rot * makeVector3f(1,0,0);
   dir[1] = rot * makeVector3f(0,1,0);
   dir[2] = rot * makeVector3f(0,0,1);

   if  (key.getKey() == ::keyLeft) {
     center += dir[0] * delta;
   } else if  (key.getKey() == ::keyRight) {
     center -= dir[0] * delta;
   } else {
      if (key.getModifiers().getShift()) {
         if (key.getKey() == ::keyUp) {
           center += dir[1] * delta;
         } else if  (key.getKey() == ::keyDown) {
           center -= dir[1] * delta;
         }
      } else {
         if (key.getKey() == ::keyUp) {
           center += dir[2] * delta;
         } else if  (key.getKey() == ::keyDown) {
           center -= dir[2] * delta;
         }
      }
   }
   if (key.getKey() == ::keyF9) {
      center -= dir[2] * distance * (1.0f-1e-5f);
      distance = 1e-5f;
   } else if (key.getKey() == ::keyF6) {
      rotAroundZAxis = !rotAroundZAxis;
   }
   changed();
}

void ExaminerCameraController::keyUp(GeneralKey key) {

}

// ----

void ExaminerCameraController::mouseDown(int32 x, int32 y, MouseButtons buttonsState, Modifiers modifiersState) {
//   if (modifiersState.getShift()) {
      if (buttonsState.getRight()) { // former getLeft()
         state = dragDist;
         lastX = x;
         lastY = y;
      }
      else if (buttonsState.getLeft()) {
         state = dragAngles;
         lastX = x;
         lastY = y;
      }
   //}
		else{
			state = dragCenter;
			lastX = x;
			lastY = y;
		}

}

void ExaminerCameraController::mouseMoved(int32 x, int32 y) {
   if (mouseAreaHeight <= 0) return;
   int32 deltaX = x - lastX;
   int32 deltaY = y - lastY;
   lastX = x;
   lastY = y;
   if (state == dragAngles) {
      float32 normDx = deltaX * anglePerScreenHeight / mouseAreaHeight;
      float32 normDy = deltaY * anglePerScreenHeight / mouseAreaHeight;
      angles[0] += normDx;
      angles[1] += normDy;
      changed();
   } else if (state == dragDist) {
      float32 normDy = pow (distFactorPerScreenHeight, - 5.0f * (float32)deltaY / (float32)mouseAreaHeight);
      distance *= normDy;
      changed();
   }
	else if (state == dragCenter) {
		Vector3f rightVec = normalize((camera->getLookAt()- camera->getPosition()).crossProduct(camera->getUp()));
		center -= rightVec * deltaX * distance * 0.005f;
		center += camera->getUp() * deltaY * distance * 0.005f;
		changed();
	}
   
}

void ExaminerCameraController::mouseUp(int32 x, int32 y, MouseButtons buttonsState, Modifiers modifiersState) {
//   if (buttonsState.getLeft()) {
      state = neutral;
//   }
}

void ExaminerCameraController::areaResize(card32 width, card32 height) {
   this->mouseAreaWidth = width;
   this->mouseAreaHeight = height;
}

// ----

void ExaminerCameraController::update() {
}

void ExaminerCameraController::apply() {
   ObjectViewsTable::update(camera);
   calcCamera();
   cameraChange();
}

// ----

float32 ExaminerCameraController::getDistance() const {
   return distance;
}

void ExaminerCameraController::setDistance(float32 dist) {
   this->distance = dist;
   changed();
}

void ExaminerCameraController::setup( const Vector3f& position, const Vector3f& ref, const Vector3f& up )
{
	this->center = ref;
	this->distance = sqrt((position-ref).getSqrNorm());
	this->angles = makeVector2f((float32)M_PI, (float32)M_PI);//(float)M_PI/2.0f);
	
	changed();
}

void ExaminerCameraController::setToCamera(Camera* cam, Vector3f newCenter) {
}


