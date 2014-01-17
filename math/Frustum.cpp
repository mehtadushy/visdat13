//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "Frustum.h"
#include "HalfSpace.hpp"
//---------------------------------------------------------------------------


static BVIntersection sphereInHalfSpace(const HalfSpace3f hs, const Sphere3f sphere) {
   float32 dist = hs.signedPlaneDistance(sphere.center);
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


BVIntersection Frustum3f::firstCheckInsideFrustum(const Sphere<float, 3> &s, card8 &parentPlanesChecked) const {
   // setup parentPlanesChecked for first call
   BVIntersection test;
   parentPlanesChecked = 0;
   test = sphereInHalfSpace(lowerPlane[0], s);
   if (test == BVQueryDisjoint) return BVQueryDisjoint;
   if (test == BVIntersectsQuery) parentPlanesChecked |= 1;
   test = sphereInHalfSpace(lowerPlane[1], s);
   if (test == BVQueryDisjoint) return BVQueryDisjoint;
   if (test == BVIntersectsQuery) parentPlanesChecked |= 2;
   test = sphereInHalfSpace(upperPlane[0], s);
   if (test == BVQueryDisjoint) return BVQueryDisjoint;
   if (test == BVIntersectsQuery) parentPlanesChecked |= 4;
   test = sphereInHalfSpace(upperPlane[1], s);
   if (test == BVQueryDisjoint) return BVQueryDisjoint;
   if (test == BVIntersectsQuery) parentPlanesChecked |= 8;

   if (parentPlanesChecked == 0)
      return BVInsideQuery;
   else
      return BVIntersectsQuery;
}

BVIntersection BoundedFrustum3f::firstCheckInsideFrustum(const Sphere<float, 3> &s, card8 &parentPlanesChecked) const {
   BVIntersection test;
   test = Frustum3f::firstCheckInsideFrustum(s, parentPlanesChecked);
   if (test == BVQueryDisjoint) return BVQueryDisjoint;

   test = sphereInHalfSpace(frontPlane, s);
   if (test == BVQueryDisjoint) return BVQueryDisjoint;
   if (test == BVIntersectsQuery) parentPlanesChecked |= 16;

   test = sphereInHalfSpace(backPlane, s);
   if (test == BVQueryDisjoint) return BVQueryDisjoint;
   if (test == BVIntersectsQuery) parentPlanesChecked |= 32;

   if (parentPlanesChecked == 0)
      return BVInsideQuery;
   else
      return BVIntersectsQuery;
}

