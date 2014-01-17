//---------------------------------------------------------------------------
#ifndef FrustumH
#define FrustumH
//---------------------------------------------------------------------------
#include "HalfSpace.h"
//---------------------------------------------------------------------------



class Frustum3f {
 public:
   HalfSpace<float32, 3> lowerPlane[2];
   HalfSpace<float32, 3> upperPlane[2];
   inline bool insideFrustum(const StaticVector<float32, 3> &v) {
      return lowerPlane[0].InHalfSpace(v) && lowerPlane[1].InHalfSpace(v)
          && upperPlane[0].InHalfSpace(v) && upperPlane[1].InHalfSpace(v);
   }
   BVIntersection firstCheckInsideFrustum(const Sphere<float32, 3> &s, card8 &parentPlanesChecked) const;

   inline BVIntersection insideFrustum(const Sphere<float, 3> &s, card8 &parentPlanesChecked) const {
      // parentPlanesChecked bit layout:
      // bit set == it is known that the parent bounding volume ...
      // 00000001   1  intersects lowerPlane[0]
      // 00000010   2  intersects lowerPlane[1]
      // 00000100   4  intersects upperPlane[0]
      // 00001000   8  intersects upperPlane[1]
      // bit cleared == s is inside the corresponding half space
      // it must be guaranted that parentPlanesChecked != 0
      BVIntersection test;

      if ((parentPlanesChecked & 1) != 0) {
         test = lowerPlane[0].InHalfSpace(s);
         if (test == BVQueryDisjoint) return BVQueryDisjoint;
         if (test == BVInsideQuery) parentPlanesChecked &= 15-1;
      }
      if ((parentPlanesChecked & 2) != 0) {
         test = lowerPlane[1].InHalfSpace(s);
         if (test == BVQueryDisjoint) return BVQueryDisjoint;
         if (test == BVInsideQuery) parentPlanesChecked &= 15-2;
      }
      if ((parentPlanesChecked & 4) != 0) {
         test = upperPlane[0].InHalfSpace(s);
         if (test == BVQueryDisjoint) return BVQueryDisjoint;
         if (test == BVInsideQuery) parentPlanesChecked &= 15-4;
      }
      if ((parentPlanesChecked & 8) != 0) {
         test = upperPlane[1].InHalfSpace(s);
         if (test == BVQueryDisjoint) return BVQueryDisjoint;
         if (test == BVInsideQuery) parentPlanesChecked &= 15-8;
      }

      if (parentPlanesChecked == 0)
         return BVInsideQuery;
      else
         return BVIntersectsQuery;
   }
};

class BoundedFrustum3f : public Frustum3f {
 public:
   HalfSpace<float32, 3> frontPlane;
   HalfSpace<float32, 3> backPlane;
   inline bool insideFrustum(const StaticVector<float32, 3> &v) {
      return Frustum3f::insideFrustum(v) && frontPlane.InHalfSpace(v) && backPlane.InHalfSpace(v);
   }
   BVIntersection firstCheckInsideFrustum(const Sphere<float32, 3> &s, card8 &parentPlanesChecked) const;
   inline BVIntersection insideFrustum(const Sphere<float32, 3> &s, card8 &parentPlanesChecked) const {
      BVIntersection test;
      if ((parentPlanesChecked & 1) != 0) {
         test = lowerPlane[0].InHalfSpace(s);
         if (test == BVQueryDisjoint) return BVQueryDisjoint;
         if (test == BVInsideQuery) parentPlanesChecked &= 255-1;
      }
      if ((parentPlanesChecked & 2) != 0) {
         test = lowerPlane[1].InHalfSpace(s);
         if (test == BVQueryDisjoint) return BVQueryDisjoint;
         if (test == BVInsideQuery) parentPlanesChecked &= 255-2;
      }
      if ((parentPlanesChecked & 4) != 0) {
         test = upperPlane[0].InHalfSpace(s);
         if (test == BVQueryDisjoint) return BVQueryDisjoint;
         if (test == BVInsideQuery) parentPlanesChecked &= 255-4;
      }
      if ((parentPlanesChecked & 8) != 0) {
         test = upperPlane[1].InHalfSpace(s);
         if (test == BVQueryDisjoint) return BVQueryDisjoint;
         if (test == BVInsideQuery) parentPlanesChecked &= 255-8;
      }
      if ((parentPlanesChecked & 16) != 0) {
         test = frontPlane.InHalfSpace(s);
         if (test == BVQueryDisjoint) return BVQueryDisjoint;
         if (test == BVInsideQuery) parentPlanesChecked &= 255-16;
      }
      if ((parentPlanesChecked & 32) != 0) {
         test = backPlane.InHalfSpace(s);
         if (test == BVQueryDisjoint) return BVQueryDisjoint;
         if (test == BVInsideQuery) parentPlanesChecked &= 255-32;
      }

      if (parentPlanesChecked == 0)
         return BVInsideQuery;
      else
         return BVIntersectsQuery;
   }
};



#endif
