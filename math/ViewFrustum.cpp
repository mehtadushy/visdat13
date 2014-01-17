//---------------------------------------------------------------------------
#include "stdafx.h"

//---------------------------------------------------------------------------
#include "ViewFrustum.h"
//---------------------------------------------------------------------------
#include "LinearAlgebra.hpp"
#include "NumericalClassProperty.h"
//---------------------------------------------------------------------------



MetaClass* ViewFrustum::ViewFrustumMetaClass = NULL;

const MetaClass* ViewFrustum::getInstanceClass() const {
   return getClass();
}

Object* ViewFrustum::newInstance() {
   return new ViewFrustum;
}

const MetaClass* ViewFrustum::getClass() {
   if (ViewFrustumMetaClass==NULL) {
      ViewFrustumMetaClass = new MetaClass("ViewFrustum", "no description.", newInstance, sizeof(ViewFrustum));
   }
   return ViewFrustumMetaClass;
}

void ViewFrustum::init(const MetaClass *parentClass) {
   ((MetaClass*)getClass())->init(parentClass);
   ViewFrustumMetaClass->copyParentProperties();

	BEGIN_CLASS_INIT( ViewFrustum );
	ADD_FLOAT32_PROP( zNearClip , 0 );
	ADD_FLOAT32_PROP( zFarClip , 0);
	ADD_FLOAT32_PROP( verticalFieldOfView , 0);
	ADD_FLOAT32_PROP( xToYAspectRatio , 0);
}

void ViewFrustum::shutdown() {
   delete ViewFrustumMetaClass;
}


// ----

ViewFrustum::ViewFrustum() {
   zNearClip = 0.1f;
   zFarClip = 1000.0f;
   verticalFieldOfView = 45.0f;
   xToYAspectRatio = 1.0f;
}

void ViewFrustum::setZNearClip(float32 znc) {
   this->zNearClip = znc;
}

void ViewFrustum::setZFarClip(float32 zfc) {
   this->zFarClip = zfc;
}

void ViewFrustum::setVerticalFieldOfView(float32 vfv) {
   this->verticalFieldOfView = vfv;
}

void ViewFrustum::setXToYAspectRatio(float32 xToYAspectRatio) {
   this->xToYAspectRatio = xToYAspectRatio;
}

void ViewFrustum::setAll(float32 zNearClip, float32 zFarClip, float32 verticalFieldOfView, float32 xToYAspectRatio) {
   this->zNearClip = zNearClip;
   this->zFarClip = zFarClip;
   this->verticalFieldOfView = verticalFieldOfView;
   this->xToYAspectRatio = xToYAspectRatio;
}

bool ViewFrustum::operator==(const ViewFrustum &vf) const {
   return    this->zNearClip == vf.zNearClip
          && this->zFarClip == vf.zFarClip
          && this->verticalFieldOfView == vf.verticalFieldOfView
          && this->xToYAspectRatio == vf.xToYAspectRatio;
}

bool ViewFrustum::operator!=(const ViewFrustum &vf) const {
   return !(*this == vf);
}

Matrix4f ViewFrustum::getProjectionMatrix() const {
   float32 d = 1.0f/(float)tan(verticalFieldOfView*0.5f*(M_PI/180.0f));
   float32 a = (zFarClip+zNearClip)/(zNearClip-zFarClip);
   float32 b = 2*zNearClip*zFarClip/(zNearClip-zFarClip);
   return makeMatrix4f(
     d/xToYAspectRatio,0,0,0,
                        0,d,0,0,
                        0,0,a,b,
                        0,0,-1,0
   );
}

