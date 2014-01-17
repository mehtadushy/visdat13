//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "Camera.h"
//---------------------------------------------------------------------------
#include "LinearAlgebra.hpp"
#include "FixedArrayClassProperty.h"
#include "NumericalClassProperty.h"
//#include <GL/gl.h>
#include "ObjectStreams.h"
#include <math.h>
//---------------------------------------------------------------------------


MetaClass* Camera::CameraMetaClass = NULL;

Camera::Camera() {
   position = makeVector3f(0,0,0);
   lookAt = makeVector3f(0,0,1);
   up = makeVector3f(0,1,0);
   //verticalViewAngle = 45;

   orthoNormViewDirection = makeVector3f(0,0,1);
   orthoNormRightDirection = makeVector3f(1,0,0);
   orthoNormUpDirection = makeVector3f(0,1,0);
}

const MetaClass* Camera::getInstanceClass() const {
   return getClass();
}

Object *Camera::newInstance() {
   return new Camera();
}

const MetaClass* Camera::getClass() {
   if (CameraMetaClass==NULL) {
      CameraMetaClass = new MetaClass("Camera", "Camera coordinate system.",
                                          newInstance, sizeof(Camera));
   }
   return CameraMetaClass;
}

void Camera::init(const MetaClass *parentClass) {
   ((MetaClass*)getClass())->init(parentClass);
   CameraMetaClass->copyParentProperties();
   
	BEGIN_CLASS_INIT( Camera );
	ADD_VECTOR3F_PROP_UPDATE( position,0, &Camera::recalcCameraCoords );
	ADD_VECTOR3F_PROP_UPDATE( lookAt,0, &Camera::recalcCameraCoords );
	ADD_VECTOR3F_PROP_UPDATE( up,0, &Camera::recalcCameraCoords );


   CameraMetaClass->getGUIDescriptor()->setAutoApplyEditChanges(false);
}

void Camera::shutdown() {
   delete CameraMetaClass;
}

void Camera::setup() {
	recalcCameraCoords();
}

void Camera::setup(Vector3f position, Vector3f lookAt, Vector3f up) {
   this->position = position;
   this->lookAt = lookAt;
   this->up = up;
   setup();
}

void Camera::recalcCameraCoords() {
   const float eps = 1E-20f; // considered zero in calculations

   // view direction
   Vector3f diff = lookAt - position;
   float32  diffnorm = sqrt(diff * diff);
//   if (fabs(diffnorm) <= eps)
//      throw ERangeCheck("Camera::setup() - ViewingDirection (nearly) = (0,0,0)");
   diff /= diffnorm;
   orthoNormViewDirection = diff;

   // up direction
   float nup = norm(up);
//   if (fabs(nup) <= eps)
//      throw ERangeCheck("Camera::setup() - NormOrthUpDirection: Up (nearly) = (0,0,0)");
   Vector3f normup = up / nup;
   Vector3f normview = orthoNormViewDirection;
   float32 proj = -(normview*normup);

//   if (fabs(proj) >= (1-1.0E-7f))
//      throw ECollinearVectors("Camera::setup() - NormOrthUpDirection: (nearly) Up | ViewingDirection");
   normview *= proj;
   Vector3f r = up + normview;
   float rnorm = norm(r);
//   if (fabs(rnorm) <= eps)
//      throw ECollinearVectors("Camera::setup() - NormOrthUpDirection: (nearly) Up | ViewingDirection");
   r *= 1/rnorm;
   orthoNormUpDirection = r;

   // NormRightDirection
   orthoNormRightDirection = orthoNormUpDirection.crossProduct(orthoNormViewDirection);
}

void Camera::write(OutputObjectStream *s) const {
   s->write<Vector3f>(position);
   s->write<Vector3f>(lookAt);
   s->write<Vector3f>(up);
}

void Camera::read(InputObjectStream *s) {
   s->read<Vector3f>(position);
   s->read<Vector3f>(lookAt);
   s->read<Vector3f>(up);
   setup(position, lookAt, up);
}

void Camera::transformCamera(const Matrix4f &inverseTransformation) {
   Vector3f nViewPoint = transformVector3f(inverseTransformation, position);
   Vector3f nLookAt    = transformVector3f(inverseTransformation, lookAt);
   Vector3f nUp        = transformVector3f(inverseTransformation, position + up) - nViewPoint;
   setup(nViewPoint, nLookAt, nUp);
}

bool Camera::operator==(const Camera &cam) const {
   return this->position == cam.position && this->lookAt == cam.lookAt && this->up == cam.up;
}

bool Camera::operator!=(const Camera &cam) const {
   return !(*this == cam);
}

Matrix4f Camera::getViewMatrix() const {
   Matrix3f camCoords;
   camCoords[0] = orthoNormRightDirection;
   camCoords[1] = orthoNormUpDirection;
   camCoords[2] = orthoNormViewDirection;
   Matrix4f camTranslate = makeTranslation4f(-position);
   return expand3To4(camCoords.transpose())*camTranslate;
}

Matrix3f Camera::getLocalCoordsMatrix() const{
   Matrix3f result;
   result[0] = orthoNormRightDirection;
   result[1] = orthoNormUpDirection;
   result[2] = orthoNormViewDirection;
   return result;
}


void Camera::setup(const Vector3f &position, const Vector3f &orientationAngles, float lookAtDistance) 
{
	Matrix3f mat;
	float cos_alpha = cos(orientationAngles[0]);
	float sin_alpha = sin(orientationAngles[0]);
	float cos_beta = cos(orientationAngles[1]);
	float sin_beta = sin(orientationAngles[1]);
	float cos_gamma = cos(orientationAngles[2]);
	float sin_gamma = sin(orientationAngles[2]);

	// http://de.wikipedia.org/wiki/Roll-Pitch-Yaw-Winkel
	mat[0][0] = cos_alpha * cos_beta;	mat[0][1] = cos_alpha * sin_beta * sin_gamma - sin_alpha*cos_gamma;	mat[0][2] = cos_alpha*sin_beta*cos_gamma + sin_alpha*sin_gamma;
	mat[1][0] = sin_alpha * cos_beta;	mat[1][1] = sin_alpha * sin_beta * sin_gamma + cos_alpha*cos_gamma;	mat[1][2] = sin_alpha*sin_beta*cos_gamma - cos_alpha*sin_gamma;
	mat[2][0] = -sin_beta;	mat[2][1] = cos_beta * sin_gamma;		mat[2][2] = cos_beta * cos_gamma;

	orthoNormViewDirection = mat[2];
	orthoNormUpDirection = mat[1];
	orthoNormRightDirection = mat[0];
	
	this->position = position;
	this->lookAt = position + orthoNormViewDirection * lookAtDistance;
	this->up = orthoNormUpDirection;
}

void Camera::setup(const StaticVector<float,6> &p, float lookAtDistance) 
{
	setup( makeVector3f(p[0],p[1],p[2]), makeVector3f(p[3],p[4],p[5]), lookAtDistance );
}

Vector3f Camera::calculateRotationAngles() 
{
	// http://de.wikipedia.org/wiki/Roll-Pitch-Yaw-Winkel
	Matrix3f mat = getLocalCoordsMatrix();
	float beta = atan2( -mat[2][0], sqrt(pow(mat[0][0],2.0f)+pow(mat[1][0],2.0f)) );
	float alpha, gamma;
	if(fabs(beta - M_PI/2.0f) < 1e-10f)
	{
		alpha = 0;
		gamma = atan2(mat[0][1],mat[1][1]);
	}
	else
	{
		if(fabs(beta + M_PI/2.0f) < 1e-10f)
		{
			alpha = 0;
			gamma = -atan2(mat[0][1],mat[1][1]);
		}
		else
		{
			float cos_beta = cos(beta);
			alpha = atan2(mat[1][0] / cos_beta,mat[0][0] / cos_beta);
			gamma = atan2(mat[2][1] / cos_beta,mat[2][2] / cos_beta);
		}
	}

	return makeVector3f(alpha,beta,gamma);
}
