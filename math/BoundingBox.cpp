//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "BoundingBox.h"
#include "BoundingBox.hpp"
#include "LinearAlgebra.hpp"
//---------------------------------------------------------------------------


BoundingBox3f transformBoundingBox(const Matrix4f &transformation, const BoundingBox3f &boundingBox) {
   Vector3f v[8];
   v[0] = makeVector3f(boundingBox.lowerCorner[0], boundingBox.lowerCorner[1], boundingBox.lowerCorner[2]);
   v[1] = makeVector3f(boundingBox.lowerCorner[0], boundingBox.lowerCorner[1], boundingBox.upperCorner[2]);
   v[2] = makeVector3f(boundingBox.lowerCorner[0], boundingBox.upperCorner[1], boundingBox.lowerCorner[2]);
   v[3] = makeVector3f(boundingBox.lowerCorner[0], boundingBox.upperCorner[1], boundingBox.upperCorner[2]);
   v[4] = makeVector3f(boundingBox.upperCorner[0], boundingBox.lowerCorner[1], boundingBox.lowerCorner[2]);
   v[5] = makeVector3f(boundingBox.upperCorner[0], boundingBox.lowerCorner[1], boundingBox.upperCorner[2]);
   v[6] = makeVector3f(boundingBox.upperCorner[0], boundingBox.upperCorner[1], boundingBox.lowerCorner[2]);
   v[7] = makeVector3f(boundingBox.upperCorner[0], boundingBox.upperCorner[1], boundingBox.upperCorner[2]);
   Vector3f p = transformVector3f(transformation, v[0]);
   BoundingBox3f result(p);
   for (int i=1; i<8; i++) {
      result.addPoint(transformVector3f(transformation, v[i]));
   }
   return result;
}

float32 getMinPrj(const BoundingBox3f &bb, Vector3f origin, Vector3f prjVector) {
   float32 minPrj = (bb.getCorner(0) - origin)*prjVector;
   for (card32 i=1; i<8; i++) {
      float32 prj = (bb.getCorner(i) - origin)*prjVector;
      if (prj<minPrj) minPrj = prj;
   }
   return minPrj;
}

