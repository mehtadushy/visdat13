//---------------------------------------------------------------------------
#ifndef CameraH
#define CameraH
//---------------------------------------------------------------------------
#include "LinearAlgebra.h"
#include "Persistent.h"
#include "Exceptions.h"
//---------------------------------------------------------------------------



class  ECollinearVectors : public EMath {
 public:
   ECollinearVectors(const string msg) : EMath(msg) {};
};


class  Camera : public Persistent {
 private:
   static MetaClass *CameraMetaClass;
   static Object *newInstance();

   // defining properties
   Vector3f position;
   Vector3f lookAt;
   Vector3f up;
   //Radian   verticalViewAngle;

   // derived properties
   Vector3f orthoNormViewDirection;
   Vector3f orthoNormRightDirection;
   Vector3f orthoNormUpDirection;

   void recalcCameraCoords();

 public:
   Camera();

   virtual const MetaClass *getInstanceClass() const;
   static const MetaClass *getClass();
   static void init(const MetaClass *parentClass);
   static void shutdown();

   inline Vector3f getPosition() const {return position;}
   inline Vector3f getLookAt() const {return lookAt;}
   inline Vector3f getUp() const {return up;}
   inline void setPosition( const Vector3f& v ) { position = v; }
   inline void setLookAt( const Vector3f& v ) { lookAt = v; }
   inline void setUp( const Vector3f& v ) { up = v; }

   inline Vector3f getOrthoNormViewDirection() const {return orthoNormViewDirection;}
   inline Vector3f getOrthoNormRightDirection() const {return orthoNormRightDirection;}
   inline Vector3f getOrthoNormUpDirection() const {return orthoNormUpDirection;}

   // throws ECollinearVectors if up || lookAt-pos and ERangeCheck if difference vectors are too small
   void setup(Vector3f position, Vector3f lookAt, Vector3f up);
   void setup();
	void setup(const Vector3f &position, const Vector3f &orientationAngles, float lookAtDistance = 1);
	void setup(const StaticVector<float,6> &parameters, float lookAtDistance = 1);
	Vector3f calculateRotationAngles();

   /** transform camera as if geometry would have been transformed by inverseTransformation^(-1) */
   void transformCamera(const Matrix4f &inverseTransformation);

   Matrix4f getViewMatrix() const;
   /// z:orthonorm-viewdir, x:orthonorm-viewright, y:orthonorm-viewup
   Matrix3f getLocalCoordsMatrix() const;

   virtual void write(OutputObjectStream *s) const;
   virtual void read(InputObjectStream *s);

   bool operator==(const Camera &cam) const;
   bool operator!=(const Camera &cam) const;
};





#endif
