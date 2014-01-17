//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "PolarCoordinates.h"
#include "LinearAlgebra.hpp"
//---------------------------------------------------------------------------
#include <math.h>
//---------------------------------------------------------------------------



Vector3f polarCoordsToNormal(const Vector2f &pCoords) {
   return makeVector3f(sin(pCoords[0])*sin(pCoords[1]),
                       sin(pCoords[0])*cos(pCoords[1]),
                       cos(pCoords[0]));
}

Vector2f normalToPolarCoords(const Vector3f &normal) {
   if (normal[0] == 0.0f && normal[1] == 0.0f) {
      if (normal[2] >= 0.0f)
         return makeVector2f(0.0f, 0.0f);
      else
         return makeVector2f((float)M_PI, 0.0f);
   } else {
      Vector3f normDir = normal / norm(normal);
      Vector3f planeDir = normDir; planeDir[2] = 0.0f;
      float32 d = norm(planeDir);
      Vector2f result;
      result[0] = acos(normDir[2]);
      result[1] = asin(planeDir[0]/d);
      if (planeDir[1] < 0) {
         result[1] = (float)M_PI - result[1];
      }
      if (result[1] < 0.0f) {
         result[1]+= 2.0f*(float)M_PI;
      }
      return result;
   }
}
