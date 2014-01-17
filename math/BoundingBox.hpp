//---------------------------------------------------------------------------
#ifndef BoundingBox_templCodeH
#define BoundingBox_templCodeH
//---------------------------------------------------------------------------
#include "BoundingBox.h"
#include "LinearAlgebra.hpp"
#include "Sphere.h"
//---------------------------------------------------------------------------



template <class FloatType, unsigned dim>
BoundingBox<FloatType, dim>::BoundingBox(StaticVector<FloatType, dim> initPoint) {
   this->lowerCorner = initPoint;
   this->upperCorner = initPoint;
}

template <class FloatType, unsigned dim>
BoundingBox<FloatType, dim>::BoundingBox(StaticVector<FloatType, dim> lowerCorner, StaticVector<FloatType, dim> upperCorner) {
   this->lowerCorner = lowerCorner;
   this->upperCorner = upperCorner;
}

template <class FloatType, unsigned dim>
inline void BoundingBox<FloatType, dim>::addPoint(StaticVector<FloatType, dim> point) {
	for (unsigned int i=0; i<dim; i++) {
		if (lowerCorner[i] > point[i]) lowerCorner[i] = point[i];
		if (upperCorner[i] < point[i]) upperCorner[i] = point[i];
	}
}

template <class FloatType, unsigned dim>
inline StaticVector<FloatType, dim> BoundingBox<FloatType, dim>::getCenter() const {
   return (upperCorner + lowerCorner) / 2.0f;
}

template <class FloatType, unsigned dim>
inline Sphere<FloatType, dim> BoundingBox<FloatType, dim>::getBoundingSphere() const {
   StaticVector<FloatType, dim> center = getCenter();
   return Sphere<FloatType, dim>(center, norm(upperCorner-center));
}

template <class FloatType, unsigned dim>
inline BVIntersection BoundingBox<FloatType, dim>::intersectBoundingBox(const BoundingBox<FloatType, dim> &queryBB) const {
   for (int d=0; d<dim; d++) {
      if (    queryBB.upperCorner[d] < lowerCorner[d]
           || queryBB.lowerCorner[d] > upperCorner[d]) {
         return BVQueryDisjoint;
      }
   }
   for (int d=0; d<dim; d++) {
      if ( !(    queryBB.upperCorner[d] >= upperCorner[d]
              || queryBB.lowerCorner[d] <= lowerCorner[d]) ) {
         return BVIntersectsQuery;
      }
   }
   return BVInsideQuery;
}

template <class FloatType, unsigned dim>
inline void BoundingBox<FloatType, dim>::addBB(const BoundingBox<FloatType, dim> &ubb) {
   for (int d=0; d<dim; d++) {
      if (ubb.upperCorner[d] > upperCorner[d]) upperCorner[d] = ubb.upperCorner[d];
      if (ubb.lowerCorner[d] < lowerCorner[d]) lowerCorner[d] = ubb.lowerCorner[d];
   }
}

template <class FloatType, unsigned dim>
inline bool BoundingBox<FloatType, dim>::inBoundingBox(const StaticVector<FloatType, dim> &point) const {
   for (int d=0; d<dim; d++) {
      if (point[d] < lowerCorner[d] || point[d] > upperCorner[d]) {
         return false;
      }
   }
   return true;
}

template <class FloatType, unsigned dim>
inline bool BoundingBox<FloatType, dim>::containedIn(const BoundingBox<FloatType, dim> &containerBox) const {
   for (int d=0; d<dim; d++) {
      if (lowerCorner[d] < containerBox.lowerCorner[d] || upperCorner[d] > containerBox.upperCorner[d]) {
         return false;
      }
   }
   return true;
}

template <class FloatType, unsigned dim>
inline void BoundingBox<FloatType, dim>::enlarge(float32 factor) {
   for (int d=0; d<dim; d++) {
      float32 l = ((upperCorner[d] - lowerCorner[d]) / 2.0f ) * factor;
      float32 c = (upperCorner[d] + lowerCorner[d]) / 2.0f;
      lowerCorner[d] = c-l;
      upperCorner[d] = c+l;
   }
}

template <class FloatType, unsigned dim>
inline void BoundingBox<FloatType, dim>::translate( StaticVector<FloatType,dim> t ) 
{
	lowerCorner += t;
	upperCorner += t;
}

template <class FloatType, unsigned dim>
inline void BoundingBox<FloatType, dim>::scale( FloatType s ) 
{
	lowerCorner *= s;
	upperCorner *= s;
}

template <class FloatType, unsigned dim>
inline void BoundingBox<FloatType, dim>::addBorder(float32 border) {
   for (int d=0; d<dim; d++) {
      lowerCorner[d] -= border;
      upperCorner[d] += border;
   }
}

template <class FloatType, unsigned dim>
inline FloatType BoundingBox<FloatType, dim>::getMinSideLength() const {
   FloatType min = upperCorner[0] - lowerCorner[0];
   for (int i=1; i<dim; i++) {
      FloatType s = upperCorner[i] - lowerCorner[i];
      if (s < min) min = s;
   }
   return min;
}

template <class FloatType, unsigned dim>
inline FloatType BoundingBox<FloatType, dim>::getMaxSideLength() const {
   FloatType max = upperCorner[0] - lowerCorner[0];
   for (int i=1; i<dim; i++) {
      FloatType s = upperCorner[i] - lowerCorner[i];
      if (s > max) max = s;
   }
   return max;
}

template <class FloatType, unsigned dim>
inline FloatType BoundingBox<FloatType, dim>::getMaxDist(const BoundingBox<FloatType, dim> &otherBB) const {
   FloatType maxDist = lowerCorner[0] - otherBB.lowerCorner[0];
   for (int d=1; d<dim; d++) {
      FloatType dist = lowerCorner[d] - otherBB.lowerCorner[d];
      if (dist > maxDist) {
         maxDist = dist;
      }
   }
   for (int d=0; d<dim; d++) {
      FloatType dist = otherBB.upperCorner[d] - upperCorner[d];
      if (dist > maxDist) {
         maxDist = dist;
      }
   }
   return maxDist;
}

template <class FloatType, unsigned dim>
inline FloatType BoundingBox<FloatType, dim>::getSideLength(unsigned i) const {
   return upperCorner[i] - lowerCorner[i];
}

template <class FloatType, unsigned dim>
inline BoundingBox<FloatType, dim> BoundingBox<FloatType, dim>::operator*(const FloatType &s) const {
   BoundingBox<FloatType, dim> result = *this;
   result.lowerCorner *= s;
   result.upperCorner *= s;
   return result;
}

template <class FloatType, unsigned dim>
inline BoundingBox<FloatType, dim> BoundingBox<FloatType, dim>::operator+(const BoundingBox<FloatType, dim> &bb) const {
   BoundingBox<FloatType, dim> result = *this;
   result.lowerCorner += bb.lowerCorner;
   result.upperCorner += bb.upperCorner;
   return result;
}

template <class FloatType, unsigned dim>
inline StaticVector<FloatType, dim> BoundingBox<FloatType, dim>::getCorner(card32 cornerNum) const {
   StaticVector<FloatType, dim> result;
   card32 pos = 1;
   for (card32 i=0; i<dim; i++) {
      if ((cornerNum & pos) == 0) {
         result[i] = lowerCorner[i];
      } else {
         result[i] = upperCorner[i];
      }
      pos = pos << 1;
   }
   return result;
}

template <class FloatType, unsigned dim>
inline void BoundingBox<FloatType, dim>::makeBoundingCube() {
   StaticVector<FloatType, dim> center = getCenter();
   FloatType maxh = getMaxSideLength() / 2.0;
   StaticVector<FloatType, dim> maxhv;
   for (card32 i=0; i<dim; i++) {
      maxhv[i] = maxh;
   }
   lowerCorner = center - maxhv;
   upperCorner = center + maxhv;
}

template <class FloatType, unsigned dim>
void BoundingBox<FloatType, dim>::cutTo(const BoundingBox<FloatType, dim> &to) {
   for (card32 i=0; i<dim; i++) {
      if (lowerCorner[i] < to.lowerCorner[i]) lowerCorner[i] = to.lowerCorner[i];
      if (upperCorner[i] > to.upperCorner[i]) upperCorner[i] = to.upperCorner[i];
      if (lowerCorner[i] > upperCorner[i]) {
         throw EInvalidState("BoundingBox::cutTo() - \"this\" does not overlap with \"to\".");
      }
   }
}




template <class FloatType, unsigned dim>
inline bool operator==(const BoundingBox<FloatType, dim> &bb1, const BoundingBox<FloatType, dim> &bb2) {
   return bb1.lowerCorner == bb2.lowerCorner  &&  bb1.upperCorner == bb2.upperCorner;
}




#endif
