//---------------------------------------------------------------------------
#ifndef PolarCoordinatesH
#define PolarCoordinatesH
//---------------------------------------------------------------------------
#include "LinearAlgebra.h"
//---------------------------------------------------------------------------


typedef Vector2f PolarCoords;

extern Vector3f polarCoordsToNormal(const Vector2f &pCoords);
extern Vector2f normalToPolarCoords(const Vector3f &normal);



#endif
