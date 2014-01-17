//---------------------------------------------------------------------------
#ifndef DynamicLinearAlgebraH
#define DynamicLinearAlgebraH
//---------------------------------------------------------------------------
#include <math.h>
#include <string>
//---------------------------------------------------------------------------
#include "PTypes.h"
#include "LinearAlgebra.h"
#include "PAssert.h"
//---------------------------------------------------------------------------
#include "Exceptions.h"
//---------------------------------------------------------------------------

using namespace std;

class OutputObjectStream;
class InputObjectStream;

// ---------------------------------------------------------------------------
//
//                                 DynamicVector
//
// ---------------------------------------------------------------------------


template <typename FloatType>
class DynamicVector {
 private:
   card32 dim;
   FloatType *v;

 public:
   //  ---- constructors, copy constructor
   inline DynamicVector<FloatType>(card32 dim = 0);
   inline DynamicVector<FloatType>(const DynamicVector<FloatType> &init);

   inline card32 getDim() const;
   /// alias to getDim()
   inline card32 size() const;
   inline void setDim(const card32 dim);
   /// set all entries to (FloatType)0
   inline void setZero();

   inline FloatType& operator[](const unsigned &index);
   inline const FloatType& operator[](const unsigned &index) const;

   //  ----  operators +, -, *, /, etc.
   inline DynamicVector<FloatType> operator+(const DynamicVector<FloatType> &op) const;
   inline DynamicVector<FloatType> operator-(const DynamicVector<FloatType> &op) const;
   inline DynamicVector<FloatType> operator-() const;
   inline FloatType operator*(const DynamicVector<FloatType> op) const;
   inline DynamicVector<FloatType> operator*(const FloatType &s) const;
   inline DynamicVector<FloatType> operator/(const FloatType &s) const;
   inline DynamicVector<FloatType> componentProduct(const DynamicVector<FloatType> &op) const;
   inline DynamicVector<FloatType> componentAddScalar(const FloatType &s) const;
   inline DynamicVector<FloatType> componentSubstractScalar(const FloatType &s) const;
   inline FloatType componentSum() const;
   inline FloatType average() const;

   //  ---- operators +=, -=, *=, /=
   inline DynamicVector<FloatType> operator+=(const DynamicVector<FloatType> &op);
   inline DynamicVector<FloatType> operator-=(const DynamicVector<FloatType> &op);
   inline DynamicVector<FloatType> operator*=(const FloatType &s);
   inline DynamicVector<FloatType> operator/=(const FloatType &s);

   //  ---- operators =, ==, !=
   inline void operator=(const DynamicVector<FloatType> &op);
   inline bool operator==(const DynamicVector<FloatType> op) const;
   inline bool operator!=(const DynamicVector<FloatType> op) const;

   inline FloatType maxComponent() const;
   inline FloatType minComponent() const;
   inline DynamicVector<FloatType> absoluteValues() const;

   inline bool allComponentsLargerEqual(const DynamicVector<FloatType> &op) const;
   inline bool allComponentsLarger(const DynamicVector<FloatType> &op) const;
   inline bool allComponentsSmallerEqual(const DynamicVector<FloatType> &op) const;
   inline bool allComponentsSmaller(const DynamicVector<FloatType> &op) const;
   inline float32 getNorm() const;

   //  ---- misc.
   void print();
   string toString() const;

   inline FloatType* data();
   inline const FloatType* data() const;
   inline FloatType* dataPtr();
   inline const FloatType* dataPtr() const;
	inline DynamicVector<FloatType> copyElements(const card32 &howMany) const;

   void write(OutputObjectStream *s) const;
   void read(InputObjectStream *s);	


   //Warning: data at "dataPointer" will be deleted by deleting DynamicVector
   inline void setDataPointer(FloatType* x, const card32 dim);

   // ---- destructor
   inline ~DynamicVector();
};



// ---------------------------------------------------------------------------
//
//                                 DynamicMatrix
//
// ---------------------------------------------------------------------------



template <typename FloatType>
class DynamicMatrix {
 private:
   card32 rows;
   card32 columns;
   DynamicVector<FloatType> *theColumns;

 public:

   // ---- constructors, copy constructor
   inline DynamicMatrix( card32 c = 0, card32 r = 0, bool initAsIdentity = true);
   inline DynamicMatrix(const DynamicMatrix<FloatType> &init);

   inline void setDimension(card32 columns = 0, card32 rows = 0, bool initAsIdentity = true);
   inline void setZero();
   inline card32 getRowsDim() const;
   inline card32 getColsDim() const;
   inline card32 getRows() const;
   inline card32 getColumns() const;

   inline DynamicVector<FloatType>& operator[](const unsigned &index);
   inline const DynamicVector<FloatType>& operator[](const unsigned &index) const;

   //  ----  operators +, -, *, /
   inline DynamicMatrix<FloatType> operator+(const DynamicMatrix<FloatType> &op) const;
   inline DynamicMatrix<FloatType> operator-(const DynamicMatrix<FloatType> &op) const;
   inline DynamicMatrix<FloatType> operator-() const;
   inline DynamicMatrix<FloatType> operator*(const FloatType &s) const;
   inline DynamicMatrix<FloatType> operator*(const DynamicMatrix<FloatType> &op) const;
   inline DynamicMatrix<FloatType> operator/(const FloatType &s) const;

   //  ---- operators +=, -=, *=, /=
   inline DynamicMatrix<FloatType> operator+=(const DynamicMatrix<FloatType> &op);
   inline DynamicMatrix<FloatType> operator-=(const DynamicMatrix<FloatType> &op);
   inline DynamicMatrix<FloatType> operator*=(const FloatType &op);
   inline DynamicMatrix<FloatType> operator*=(const DynamicMatrix<FloatType> &op);
   inline DynamicMatrix<FloatType> operator/=(const FloatType &op);

   //  ---- operators =, ==, !=
   inline void operator=(const DynamicMatrix<FloatType> &op);
   inline bool operator==(const DynamicMatrix<FloatType> &op) const;
   inline bool operator!=(const DynamicMatrix<FloatType> &op) const;

   // ---- multiplying with vectors
   inline DynamicVector<FloatType> operator*(const DynamicVector<FloatType> &v) const;

   // ----  misc.
   inline void print();
   string toString() const;

   inline void copyDataToArray(FloatType* target) const;
   inline void copyDataFromArray(FloatType* source);
   ///get top-left 3x3 values (for debug)
   inline Matrix3f getMatrix3f();

   inline void changeRows(unsigned row1, unsigned row2);
   inline void multRow(unsigned row, FloatType value);
   inline void combineRows(unsigned row, unsigned with, FloatType by);
   inline void addRows(unsigned row, unsigned with);
   inline DynamicMatrix<FloatType> transpose() const;

   /// copies subset of columns to new matrix
   inline DynamicMatrix<FloatType> copyColumns(unsigned startColumn, unsigned numColumns);

   inline FloatType getDeterminant() const;

   void write(OutputObjectStream *s) const;
   void read(InputObjectStream *s);	

   // ---- destructor
   inline ~DynamicMatrix();
};


// ---------------------------------------------------------------------------
//
//                                   instances
//
// ---------------------------------------------------------------------------


typedef DynamicVector<float> DVectorF;
typedef DynamicVector<double> DVectorD;
typedef DynamicVector<int> DVectorI;

typedef DynamicMatrix<float> DMatrixF;
typedef DynamicMatrix<double> DMatrixD;
typedef DynamicMatrix<int> DMatrixI;


// ---------------------------------------------------------------------------
//
//                                 aux. routines
//
// ---------------------------------------------------------------------------


template <typename FloatType>
inline DynamicVector<FloatType> scalarDVector(card32 dim, const FloatType &s);

template <typename FloatType>
inline DynamicVector<FloatType> nullDVector(card32 dim);

template <typename FloatType>
inline DynamicVector<FloatType> makeDVector(card32 dim, const FloatType * dataptr);

template <typename FloatType1, typename FloatType2>
inline void convertDVectorType(const DynamicVector<FloatType1> &source, DynamicVector<FloatType2> &dest);


template <typename FloatType>
inline DynamicMatrix<FloatType> scalarDMatrix(card32 c, card32 r, const FloatType &s);

template <typename FloatType>
inline DynamicMatrix<FloatType> nullDMatrix(card32 c, card32 r);

/// v1^t v2
template <typename FloatType>
inline DynamicMatrix<FloatType> outerProduct( const DynamicVector<FloatType> &v1,const DynamicVector<FloatType> &v2 );

template <typename FloatType>
inline FloatType normQuad(const DynamicVector<FloatType> &v);

template <typename FloatType>
inline FloatType norm(const DynamicVector<FloatType> &v);

template <typename FloatType>
inline FloatType normInfinity(const DynamicVector<FloatType> &v);

template <typename FloatType>
inline unsigned searchPivot(const DynamicMatrix<FloatType> &mat, unsigned step);

template <typename FloatType>
inline DynamicMatrix<FloatType> invertMatrix(const DynamicMatrix<FloatType> &mat);


template <typename FloatType>
inline DynamicVector<FloatType> maxVector( const DynamicVector<FloatType> &v1,const DynamicVector<FloatType> &v2);


template <typename FloatType>
inline DynamicVector<FloatType> minVector( const DynamicVector<FloatType> &v1,const DynamicVector<FloatType> &v2);




#include "Dynamiclinearalgebra.hpp"

#endif
