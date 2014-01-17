//---------------------------------------------------------------------------
#ifndef HalfSpaceH
#define HalfSpaceH
//---------------------------------------------------------------------------
#include "BoundingBox.h"
#include "LinearAlgebra.h"
#include "BoundingVolumeIntersection.h"
#include "Sphere.h"
#include "PAssert.h"
//---------------------------------------------------------------------------



template<class FloatType, unsigned dim>
class  HalfSpace {
 private:
   /// normal * x  -  originDist = 0
   StaticVector<FloatType, dim> normal;
   FloatType originDist;

 public:
   inline HalfSpace() {}
   inline HalfSpace(StaticVector<FloatType, dim> normal, FloatType originDist);
   inline HalfSpace(StaticVector<FloatType, dim> normal, StaticVector<FloatType, dim> pointOnPlane);

   inline void setNormal(StaticVector<FloatType, dim> normal);
   inline StaticVector<FloatType, dim> getNormal();
   inline void setOriginDist(FloatType originDist);
   inline FloatType getOriginDist();

   inline FloatType signedPlaneDistance(const StaticVector<FloatType, dim> &point) const;
   inline bool InHalfSpace(const StaticVector<FloatType, dim> &point) const;
   inline BVIntersection InHalfSpace(const Sphere<FloatType, dim> &sphere) const;
   inline BVIntersection InHalfSpace(const BoundingBox<FloatType, dim> &bb) const;
};


template<class FloatType>
void transformHalfSpace(HalfSpace<FloatType, 3> &halfSpace,
                        const StaticMatrix<FloatType, 4, 4> &absTrans,
                        const StaticMatrix<FloatType, 4, 4> &normalTrans);


typedef HalfSpace<float32, 2> HalfSpace2f;
typedef HalfSpace<float32, 3> HalfSpace3f;






#endif
