//---------------------------------------------------------------------------
#ifndef HalfSpace_templCodeH
#define HalfSpace_templCodeH
//---------------------------------------------------------------------------
#include "HalfSpace.h"
//---------------------------------------------------------------------------
#include "BoundingBox.hpp"
#include "LinearAlgebra.hpp"
#include "Sphere.h"
#include "BoundingVolumeIntersection.h"
//---------------------------------------------------------------------------



template<class FloatType, unsigned dim>
inline HalfSpace<FloatType, dim>::HalfSpace(StaticVector<FloatType, dim> normal, FloatType originDist) {
   this->normal = normal / norm(normal);
   this->originDist = originDist;
}

template<class FloatType, unsigned dim>
inline void HalfSpace<FloatType, dim>::setNormal(StaticVector<FloatType, dim> normal) {
   this->normal = normal;
}

template<class FloatType, unsigned dim>
inline StaticVector<FloatType, dim> HalfSpace<FloatType, dim>::getNormal() {
   return normal;
}

template<class FloatType, unsigned dim>
inline void HalfSpace<FloatType, dim>::setOriginDist(FloatType originDist) {
   this->originDist = originDist;
}

template<class FloatType, unsigned dim>
inline FloatType HalfSpace<FloatType, dim>::getOriginDist() {
   return originDist;
}

template<class FloatType, unsigned dim>
inline HalfSpace<FloatType, dim>::HalfSpace(StaticVector<FloatType, dim> normal, StaticVector<FloatType, dim> pointOnPlane) {
   this->normal = normal / norm(normal);
   this->originDist = (this->normal)*pointOnPlane;
}

template<class FloatType, unsigned dim>
inline FloatType HalfSpace<FloatType, dim>::signedPlaneDistance(const StaticVector<FloatType, dim> &point) const {
   return normal * point - originDist;
}

template<class FloatType, unsigned dim>
inline bool HalfSpace<FloatType, dim>::InHalfSpace(const StaticVector<FloatType, dim> &point) const {
   return signedPlaneDistance(point) >= 0.0;
}

template<class FloatType, unsigned dim>
inline BVIntersection HalfSpace<FloatType, dim>::InHalfSpace(const Sphere<FloatType, dim> &sphere) const {
   FloatType dist = signedPlaneDistance(sphere.center);
   if (dist - sphere.radius < 0.0) {
      if (dist + sphere.radius >= 0.0) {
         return BVIntersectsQuery;
      } else {
         return BVQueryDisjoint;
      }
   } else {
      return BVInsideQuery;
   }
}

// template<class FloatType, unsigned dim>
// inline BVIntersection HalfSpace<FloatType, dim>::InHalfSpace(const BoundingBox<FloatType, dim> &bb) const {
//    FloatType dist = signedPlaneDistance(sphere.center);
//    bool oneInside = false;
//    bool oneOutside = false;
//    Vector3f point = bb.lowerCorner;  // 000
//    if (InHalfSpace(point)) {
//       oneInside = true;
//    } else {
//       oneOutside = true;
//    }
//    point[0] = bb.upperCorner[0];     // 100
//    if (InHalfSpace(point)) {
//       oneInside = true;
//    } else {
//       oneOutside = true;
//    }
//    point[1] = bb.upperCorner[1];     // 110
//    if (InHalfSpace(point)) {
//       oneInside = true;
//    } else {
//       oneOutside = true;
//    }
//    point[2] = bb.upperCorner[2];     // 111
//    if (InHalfSpace(point)) {
//       oneInside = true;
//    } else {
//       oneOutside = true;
//    }
//    point[0] = bb.lowererCorner[0];   // 011
//    if (InHalfSpace(point)) {
//       oneInside = true;
//    } else {
//       oneOutside = true;
//    }
//    point[1] = bb.lowerCorner[1];     // 001
//    if (InHalfSpace(point)) {
//       oneInside = true;
//    } else {
//       oneOutside = true;
//    }
//    point[0] = bb.upperCorner[0];     // 101
//    if (InHalfSpace(point)) {
//       oneInside = true;
//    } else {
//       oneOutside = true;
//    }
//    point[0] = bb.lowerCorner[0];    // 010
//    point[1] = bb.upperCorner[1];
//    point[2] = bb.lowerCorner[2];
//    if (InHalfSpace(point)) {
//       oneInside = true;
//    } else {
//       oneOutside = true;
//    }
//    if (!oneInside && oneOutside) {
//       return BVQueryDisjoint;
//    } else if (oneInside && !oneOutside) {
//       return BVInsideQuery;
//    } else {
//       return BVQueryIntersects;
//    }
// }


template<class FloatType>
void transformHalfSpace(HalfSpace<FloatType, 3> &halfSpace,
                        const StaticMatrix<FloatType, 4, 4> &absTrans,
                        const StaticMatrix<FloatType, 4, 4> &normalTrans) {
   StaticVector<FloatType, 3> planeVect = halfSpace.getNormal() * halfSpace.getOriginDist();
   halfSpace.setNormal(transformVector3f(normalTrans, halfSpace.getNormal()));
   planeVect = transformVector3f(absTrans, planeVect);
   halfSpace.setOriginDist(norm(planeVect));
}





#endif
