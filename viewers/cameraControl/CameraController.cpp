//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "CameraController.h"
#include "Tools.h"
#include "LinearAlgebra.hpp"
//---------------------------------------------------------------------------


MetaClass* CameraController::CameraControllerMetaClass = NULL;

const MetaClass* CameraController::getInstanceClass() const {
   return getClass();
}

const MetaClass* CameraController::getClass() {
   if (CameraControllerMetaClass==NULL) {
      CameraControllerMetaClass = new MetaClass("CameraController", "Keyboard / mouse event driven controller for camera objects.",
                                          NULL, sizeof(CameraController));
   }
   return CameraControllerMetaClass;
}

void CameraController::init(const MetaClass *parentClass) {
   ((MetaClass*)getClass())->init(parentClass);
   CameraControllerMetaClass->copyParentProperties();
}

void CameraController::shutdown() {
   delete CameraControllerMetaClass;
}


// ----

CameraController::CameraController() : notifyOnChange(NULL) {
   camera = NULL;
}


void CameraController::bindCamera(Camera *cam) {
   this->camera = cam;
}

void CameraController::setNotifyOnCameraChange(QDelegate notifyOnChange) {
   this->notifyOnChange = notifyOnChange;
}

QDelegate CameraController::getNotifyOnCameraChange() {
   return notifyOnChange;
}

void CameraController::cameraChange() {
   if (notifyOnChange != NULL) notifyOnChange();
}

Object *CameraController::getObject() {
   return camera;
}

void CameraController::setTime(float32 currentTime) {
}

