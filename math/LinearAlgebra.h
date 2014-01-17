//---------------------------------------------------------------------------
#ifndef LinearAlgebraH
#define LinearAlgebraH
//---------------------------------------------------------------------------
#include "PTypes.h"
//---------------------------------------------------------------------------
#include <string>
//---------------------------------------------------------------------------


typedef float32 Degree;
typedef float32 Radian;


// ---------------------------------------------------------------------------
//
//                                 StaticVector
//
// ---------------------------------------------------------------------------


template <class FloatType, unsigned dim>
class StaticVector {
 private:
   FloatType v[dim];
 public:
   inline FloatType& operator[](const unsigned &index);
   inline const FloatType& operator[](const unsigned &index) const;

   //  ----  operators +, -, *, /, crossProduct
   inline StaticVector<FloatType, dim> operator+(const StaticVector<FloatType, dim> &op) const;
   inline StaticVector<FloatType, dim> operator-(const StaticVector<FloatType, dim> &op) const;
   inline StaticVector<FloatType, dim> operator-() const;
   inline FloatType operator*(const StaticVector<FloatType, dim> op) const;
   inline StaticVector<FloatType, dim> operator*(const FloatType &s) const;
   inline StaticVector<FloatType, dim> operator/(const FloatType &s) const;
   StaticVector<FloatType, dim> crossProduct(const StaticVector<FloatType, dim> &op) const;
   inline StaticVector<FloatType, dim> componentProduct(const StaticVector<FloatType, dim> &op) const;

   //  ---- operators +=, -=, *=, /=
   inline StaticVector<FloatType, dim> operator+=(const StaticVector<FloatType, dim> &op);
   inline StaticVector<FloatType, dim> operator-=(const StaticVector<FloatType, dim> &op);
   inline StaticVector<FloatType, dim> operator*=(const FloatType &s);
   inline StaticVector<FloatType, dim> operator/=(const FloatType &s);

   //  ---- operators =, ==, !=
   inline bool operator==(const StaticVector<FloatType, dim> op) const;
   inline bool operator!=(const StaticVector<FloatType, dim> op) const;

   //  ---- misc.
   void print();
   std::string toString() const;
   inline FloatType* data();
   inline const FloatType* data() const;
   inline void normalize();
   inline FloatType getSqrNorm() const;
   inline StaticVector<FloatType, dim>& setZero();

   /// compatibility with DynamicVector
   static inline unsigned getDim() {return dim;}
   static inline unsigned size() {return dim;}
};


// ---------------------------------------------------------------------------
//
//                                 StaticMatrix
//
// ---------------------------------------------------------------------------


template <class FloatType, unsigned columns, unsigned rows>
class StaticMatrix {
 private:
   StaticVector<FloatType, rows> theColumns[columns];

 public:
   // ---- constructors
   inline StaticMatrix<FloatType, columns, rows>();

   // ---- element access
   inline StaticVector<FloatType, rows>& operator[](const unsigned &index);
   inline const StaticVector<FloatType, rows>& operator[](const unsigned &index) const;

   //  ----  operators +, -, *, /
   inline StaticMatrix<FloatType, columns, rows> operator+(const StaticMatrix<FloatType, columns, rows> &op) const;
   inline StaticMatrix<FloatType, columns, rows> operator-(const StaticMatrix<FloatType, columns, rows> &op) const;
   inline StaticMatrix<FloatType, columns, rows> operator-() const;
   inline StaticMatrix<FloatType, columns, rows> operator*(const FloatType &s) const;
   /* This:    colums: columns (b)
               rows: rows (a)
      Operand: colums: colums2 (c)
               rows: columns (b)
      Result:  colums: columns2 (a)
               rows: rows (c)
   */
   template <unsigned columns2>
   inline StaticMatrix<FloatType, columns2, rows> operator*( const StaticMatrix<FloatType, columns2, columns>& mat ) const;

   inline StaticMatrix<FloatType, columns, rows> operator/(const FloatType &s) const;

   //  ---- operators +=, -=, *=, /=
   inline StaticMatrix<FloatType, columns, rows> operator+=(const StaticMatrix<FloatType, columns, rows> &op);
   inline StaticMatrix<FloatType, columns, rows> operator-=(const StaticMatrix<FloatType, columns, rows> &op);
   inline StaticMatrix<FloatType, columns, rows> operator*=(const FloatType &op);
   inline StaticMatrix<FloatType, columns, rows> operator*=(const StaticMatrix<FloatType, columns, rows> &op);
   inline StaticMatrix<FloatType, columns, rows> operator/=(const FloatType &op);

   //  ---- operators =, ==, !=
   inline StaticMatrix<FloatType, columns, rows> operator=(const StaticMatrix<FloatType, columns, rows> &op);
   inline bool operator==(const StaticMatrix<FloatType, columns, rows> &op) const;
   inline bool operator!=(const StaticMatrix<FloatType, columns, rows> &op) const;

   // ---- multiplying with vectors
   inline StaticVector<FloatType, rows> operator*(const StaticVector<FloatType, columns> &v) const;

   // ----  misc.
   inline void print();
   std::string toString() const;
   inline FloatType* data();
   inline const FloatType* data() const;
   inline void changeRows(unsigned row1, unsigned row2);
   inline void multRow(unsigned row, FloatType value);
   inline void combineRows(unsigned row, unsigned with, FloatType by);
   inline void addRows(unsigned row, unsigned with);
   inline StaticMatrix<FloatType, rows, columns> transpose() const;
   inline FloatType getDeterminant() const;
   inline StaticMatrix<FloatType, columns, rows>& setZero();
   inline void setIdentity();

   /// Sort eigenvectors according to eigenvalues in descending order
   void eigenSort( StaticVector<FloatType,rows>& eigenValues, StaticMatrix<FloatType,rows,columns>& eigenVectors ) const;
   /// Compute the eigenstructure (eigenvalues and eigenvectors) of a symmetric matrix (Numerical Recepies).
   /// The normalized eigenvectors are found as the rows of the eigenVectors-matrix.
   void solveEigenProblemSymmetric( StaticVector<FloatType,rows>& eigenValues, StaticMatrix<FloatType,rows,columns>& eigenVectors ) const;

   ///Computes EigenValues and EigenVectors of a general matrix.
   void solveEigenProblem(StaticVector< FloatType, rows >& EigenValuesReal,
                          StaticVector< FloatType, rows >& EigenValuesImaginary,
                          StaticMatrix< FloatType, rows, columns >& EigenVectors) const;

   /// compatibility with DynamicVector
   static inline unsigned getRowsDim() {return rows;}
   static inline unsigned getColsDim() {return columns;}
   static inline unsigned getRows()    {return rows;}
   static inline unsigned getColumns() {return columns;}
};


// ---------------------------------------------------------------------------
//
//                                  instances
//
// ---------------------------------------------------------------------------


typedef StaticVector<float,2> Vector2f;
typedef StaticVector<float,3> Vector3f;
typedef StaticVector<float,4> Vector4f;
typedef StaticVector<float,6> Vector6f;
typedef StaticVector<double,2> Vector2d;
typedef StaticVector<double,3> Vector3d;
typedef StaticVector<double,4> Vector4d;
typedef StaticVector<int32,2> Vector2i;
typedef StaticVector<int32,3> Vector3i;
typedef StaticVector<int32,4> Vector4i;
typedef StaticVector<card8,2> Vector2ub;
typedef StaticVector<card8,3> Vector3ub;
typedef StaticVector<card8,4> Vector4ub;
typedef StaticVector<card32,2> Vector2ui;
typedef StaticVector<card32,3> Vector3ui;
typedef StaticVector<card32,4> Vector4ui;

typedef StaticMatrix<float,2,2> Matrix2f;
typedef StaticMatrix<float,3,3> Matrix3f;
typedef StaticMatrix<float,4,4> Matrix4f;
typedef StaticMatrix<float,6,6> Matrix6f;
typedef StaticMatrix<double,2,2> Matrix2d;
typedef StaticMatrix<double,3,3> Matrix3d;
typedef StaticMatrix<double,4,4> Matrix4d;


// ---------------------------------------------------------------------------
//
//                                 misc. functions
//
// ---------------------------------------------------------------------------


template <class FloatTypeDest, class FloatTypeSource, int Dim>
StaticVector<FloatTypeDest, Dim> convertVector(const StaticVector<FloatTypeSource, Dim> &source);


inline Vector2i makeVector2i(int x, int y);
inline Vector2f makeVector2f(float x, float y);
inline Vector2d makeVector2d(double x, double y);
inline Vector2ub makeVector2ub(card8 x, card8 y);

inline Vector3f makeVector3f(float x, float y, float z);
inline Vector3d makeVector3d(double x, double y, double z);
inline Vector3i makeVector3i(int x, int y, int z);
inline Vector3ub makeVector3ub(card8 x, card8 y, card8 z);

inline Vector4f makeVector4f(float x, float y, float z, float w);
inline Vector4i makeVector4i(int x, int y, int z, int w);
inline Vector4d makeVector4d(double x, double y, double z, double w);
inline Vector4ub makeVector4ub(card8 x, card8 y, card8 z, card8 w);

inline Vector6f makeVector6f(float x, float y, float z, float u, float v, float w);


template<class FloatType,unsigned dim>
inline StaticVector<FloatType,dim> makeVector( const FloatType *data );

inline Vector2f makeVector2fv( const float32 * data );
inline Vector3f makeVector3fv( const float32 * data );
inline Vector4f makeVector4fv( const float32 * data );


inline Matrix2f makeMatrix2f(float m00, float m01,
                             float m10, float m11);

inline Matrix3f makeMatrix3f(float m00, float m10, float m20,
                             float m01, float m11, float m21,
                             float m02, float m12, float m22);

inline Matrix4f makeMatrix4f(float m00, float m10, float m20, float m30,
                             float m01, float m11, float m21, float m31,
                             float m02, float m12, float m22, float m32,
                             float m03, float m13, float m23, float m33);

inline Matrix6f makeMatrix6f(float m00, float m10, float m20, float m30, float m40, float m50,
                             float m01, float m11, float m21, float m31, float m41, float m51,
                             float m02, float m12, float m22, float m32, float m42, float m52,
                             float m03, float m13, float m23, float m33, float m43, float m53,
               float m04, float m14, float m24, float m34, float m44, float m54,
               float m05, float m15, float m25, float m35, float m45, float m55);

inline Vector4f expand3To4(Vector3f v);
inline Matrix4f expand3To4(Matrix3f v);
inline Vector3f shrink4To3(Vector4f v);
inline Matrix3f shrink4To3(Matrix4f v);

inline Vector4f expand2To3(Vector3f v);
inline Vector3f projectHomogeneous3To2(Vector4f v);
inline Vector3f projectHomogeneous4To3(Vector4f v);

template <class FloatType, unsigned dim>
inline StaticMatrix<FloatType,dim,dim> outerProduct(const StaticVector<FloatType,dim> &v1, const StaticVector<FloatType,dim> &v2);

template <class FloatType, unsigned dim>
inline FloatType normQuad(StaticVector<FloatType, dim> v);

template <class FloatType, unsigned dim>
inline FloatType norm(StaticVector<FloatType, dim> v);


template <class FloatType, unsigned dim>
inline StaticVector<FloatType, dim> normalize(const StaticVector<FloatType, dim> &v);


inline Matrix3f makeRotX3f(Radian angle);
inline Matrix4f makeRotX4f(Radian angle);

inline Matrix3f makeRotY3f(Radian angle);
inline Matrix4f makeRotY4f(Radian angle);

inline Matrix3f makeRotZ3f(Radian angle);
inline Matrix4f makeRotZ4f(Radian angle);

inline Matrix3f makeRotVector3f(Vector3f vec, float32 angle);
inline Matrix4f makeRotVector4f(Vector3f vec, float32 angle);

inline Matrix4f makeTranslation4f(const Vector3f &t);
inline Matrix4f makeTranslation4f(float32 x, float32 y, float32 z);

inline Matrix3f makeScale3f(const Vector3f &s);
inline Matrix4f makeScale4f(const Vector3f &s);
inline Matrix3f makeScale3f(float32 x, float32 y, float32 z);
inline Matrix4f makeScale4f(float32 x, float32 y, float32 z);

inline Vector3f transformVector3f(const Matrix4f &M, const Vector3f &v);
inline Vector2f projectiveTransformVector2f(const Matrix3f &M, const Vector2f &v);
inline Vector3f projectiveTransformVector3f(const Matrix4f &M, const Vector3f &v);

inline Matrix3f makeNewCoordPrjMatrix3f(Vector3f v1, Vector3f v2, Vector3f v3);


template<class FloatType, unsigned size>
inline unsigned searchPivot(const StaticMatrix<FloatType, size, size> &mat, unsigned step, bool *success = NULL, FloatType pivotEps = (FloatType)0);

/// if success is not NULL, true is returned if inversion was successfull and false if the matrix was found to be singular
/// otherwise, an exception is thrown in case of singularity
template<class FloatType, unsigned size>
inline StaticMatrix<FloatType, size, size> invertMatrix(const StaticMatrix<FloatType, size, size> &mat, bool *success = NULL, FloatType pivotEps = (FloatType)0);


inline Matrix4f invertFrame( const Matrix4f &m);

// ---------------------------------------------------------------------------
//
//                             predefined constants
//
// ---------------------------------------------------------------------------


extern const Vector2ub NULL_VECTOR2UB;
extern const Vector3ub NULL_VECTOR3UB;
extern const Vector4ub NULL_VECTOR4UB;

extern const Vector2i NULL_VECTOR2I;
extern const Vector3i NULL_VECTOR3I;
extern const Vector4i NULL_VECTOR4I;

extern const Vector2f NULL_VECTOR2F;
extern const Vector3f NULL_VECTOR3F;
extern const Vector4f NULL_VECTOR4F;
extern const Vector6f NULL_VECTOR6F;

extern const Vector4f HOMOGENEOUS_NULL_VECTOR4F;

extern const Vector2d NULL_VECTOR2D;
extern const Vector3d NULL_VECTOR3D;
extern const Vector4d NULL_VECTOR4D;


extern const Vector3f XAXIS_VECTOR3F;
extern const Vector3f YAXIS_VECTOR3F;
extern const Vector3f ZAXIS_VECTOR3F;

extern const Vector4f XAXIS_VECTOR4F;
extern const Vector4f YAXIS_VECTOR4F;
extern const Vector4f ZAXIS_VECTOR4F;

extern const Matrix2f IDENTITY2F;
extern const Matrix3f IDENTITY3F;
extern const Matrix4f IDENTITY4F;
extern const Matrix6f IDENTITY6F;
extern const Matrix3d IDENTITY3D;



#include "LinearAlgebra.hpp"

#endif
