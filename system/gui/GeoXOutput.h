//---------------------------------------------------------------------------
#ifndef GeoXOutputH
#define GeoXOutputH
//---------------------------------------------------------------------------
#include <string>
#include "LinearAlgebra.h"
#include "DynamicLinearAlgebra.h"
#include "BoundingBox.h"
#include "SparseLinearAlgebra.h"
//---------------------------------------------------------------------------

using namespace std;

class MetaClass;

class GeoXOutput {
 public:
   GeoXOutput& operator<<(bool b);
   GeoXOutput& operator<<(int i);
   GeoXOutput& operator<<(unsigned int i);
   // this is necessary for 64-bit builds
   GeoXOutput& operator<<(int64 i);
   GeoXOutput& operator<<(card64 i);
   GeoXOutput& operator<<(long unsigned int i);
   GeoXOutput& operator<<(long double d);
   GeoXOutput& operator<<(float f);
   GeoXOutput& operator<<(double d);
   GeoXOutput& operator<<(const string& s);
   GeoXOutput& operator<<(const char* s);
   GeoXOutput& operator<<(const Vector2f &v);
   GeoXOutput& operator<<(const Vector3f &v);
   GeoXOutput& operator<<(const Vector4f &v);
   GeoXOutput& operator<<(const Vector6f &v);
   GeoXOutput& operator<<(const Vector3i &v);
   GeoXOutput& operator<<(const Matrix4f &m);
   GeoXOutput& operator<<(const Matrix3f &m);
   GeoXOutput& operator<<(const Matrix2f &m);
   GeoXOutput& operator<<(const BoundingBox3f &m);
   GeoXOutput& operator<<(const StaticVector<float32,7> &v);
   GeoXOutput& operator<<(const DynamicVector<float> &m);
   GeoXOutput& operator<<(const DynamicMatrix<float> &m);
   GeoXOutput& operator<<(SparseMatrixF &A);

   void writeIdented(unsigned ident, string s);
   void cr();

};

extern  GeoXOutput output;

/// printout warning message on debug console, pop-up window if non-visible.
void  error(const string& msg);

/// printout error message on debug console, pop-up window if non-visible.
void  warning(const string& msg);

/// printout message on debug console, pop-up window if non-visible.
void  message(const string& msg);



#endif
