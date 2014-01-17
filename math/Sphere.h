//---------------------------------------------------------------------------
#ifndef SphereH
#define SphereH
//---------------------------------------------------------------------------
#include "LinearAlgebra.h"
//---------------------------------------------------------------------------



template <class FloatType, unsigned dim>
class Sphere {
 public:
   StaticVector<FloatType, dim> center;
   FloatType  radius;

   Sphere() {}
   Sphere(StaticVector<FloatType, dim> center, FloatType  radius)
	{
	   this->center = center;
		this->radius = radius;
	}
};

typedef Sphere<float32, 2> Circle2f;
typedef Sphere<float32, 3> Sphere3f;
typedef Sphere<float64, 2> Circle2d;
typedef Sphere<float64, 3> Sphere3d;




#endif
