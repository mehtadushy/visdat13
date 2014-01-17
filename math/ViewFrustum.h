//---------------------------------------------------------------------------
#ifndef ViewFrustumH
#define ViewFrustumH
//---------------------------------------------------------------------------
#include "PTypes.h"
#include "MetaClass.h"
#include "Persistent.h"
#include "LinearAlgebra.h"
//---------------------------------------------------------------------------



class ViewFrustum : public Persistent {
 private:
   static MetaClass *ViewFrustumMetaClass;
   static Object* newInstance();

   float32 zNearClip;
   float32 zFarClip;
   float32 verticalFieldOfView;
   float32 xToYAspectRatio;

 public:
   ViewFrustum();

   virtual const MetaClass *getInstanceClass() const;
   static const MetaClass *getClass();
   static void init(const MetaClass *parentClass);
   static void shutdown();

   inline  float32 getZNearClip() const {return zNearClip;}
   virtual void    setZNearClip(float32 znc);

   inline  float32 getZFarClip() const {return zFarClip;}
   virtual void    setZFarClip(float32 zfc);

   inline  float32 getVerticalFieldOfView() const {return verticalFieldOfView;}
   virtual void    setVerticalFieldOfView(float32 vfv);

   inline  float32 getXToYAspectRatio() const {return xToYAspectRatio;}
   virtual void    setXToYAspectRatio(float32 xToYAspectRatio);

   virtual void    setAll(float32 zNearClip, float32 zFarClip, float32 verticalFieldOfView, float32 xToYAspectRatio);

   bool operator==(const ViewFrustum &vf) const;
   bool operator!=(const ViewFrustum &vf) const;

   /// returns projection matrix for normalized device coordinates [-1,1]^2 x [0,1] (x,y,z)
   Matrix4f getProjectionMatrix() const;
};




#endif
