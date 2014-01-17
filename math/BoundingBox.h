//---------------------------------------------------------------------------
#ifndef BoundingBoxH
#define BoundingBoxH
//---------------------------------------------------------------------------
#include "LinearAlgebra.h"
#include "Sphere.h"
#include "BoundingVolumeIntersection.h"
//---------------------------------------------------------------------------



template <class FloatType, unsigned dim>
class BoundingBox {
 public:
   // ---- data fields
   StaticVector<FloatType, dim> lowerCorner;
   StaticVector<FloatType, dim> upperCorner;

   // ---- methods
   BoundingBox() {}
   explicit BoundingBox(StaticVector<FloatType, dim> initPoint);
   BoundingBox(StaticVector<FloatType, dim> lowerCorner, StaticVector<FloatType, dim> upperCorner);

   inline void addPoint(StaticVector<FloatType, dim> point);
   inline void addBB(const BoundingBox<FloatType, dim> &ubb);
   inline void enlarge(float32 factor);
   inline void translate(StaticVector<FloatType, dim> t);
   inline void scale( FloatType s );
   inline void addBorder(float32 border);

   inline StaticVector<FloatType, dim> getCenter() const;
   inline Sphere<FloatType, dim> getBoundingSphere() const;
   inline FloatType getMinSideLength() const;
   inline FloatType getMaxSideLength() const;
   inline FloatType getMaxDist(const BoundingBox<FloatType, dim> &otherBB) const;
   inline FloatType getSideLength(unsigned i) const;

   inline BVIntersection intersectBoundingBox(const BoundingBox<FloatType, dim> &queryBB) const;
   inline bool inBoundingBox(const StaticVector<FloatType, dim> &point) const;
   inline bool containedIn(const BoundingBox<FloatType, dim> &containerBox) const;

   inline BoundingBox<FloatType, dim> operator*(const FloatType &s) const;
   inline BoundingBox<FloatType, dim> operator+(const BoundingBox<FloatType, dim> &bb) const;
   inline StaticVector<FloatType, dim> getCorner(card32 cornerNum) const;
   inline void makeBoundingCube();
   /// maintain the volume of "this" that is in "to"
   void cutTo(const BoundingBox<FloatType, dim> &to);
};



typedef BoundingBox<float32, 2> BoundingBox2f;
typedef BoundingBox<float32, 3> BoundingBox3f;
typedef BoundingBox<float64, 2> BoundingBox2d;
typedef BoundingBox<float64, 3> BoundingBox3d;


extern BoundingBox3f transformBoundingBox(const Matrix4f &transformation, const BoundingBox3f &boundingBox);
extern float32 getMinPrj(const BoundingBox3f &bb, Vector3f origin, Vector3f prjVector);

template <class FloatType, unsigned dim>
inline bool operator==(const BoundingBox<FloatType, dim> &bb1, const BoundingBox<FloatType, dim> &bb2);

                                   


#include "BoundingBox.hpp"

#endif
