//---------------------------------------------------------------------------
#ifndef DynamicLinearAlgebraToolsH
#define DynamicLinearAlgebraToolsH
//---------------------------------------------------------------------------
#include "DynamicLinearAlgebra.h"
//---------------------------------------------------------------------------
#include "Random.h"
#include "GeoXOutput.h"
//---------------------------------------------------------------------------

using namespace std;


template <typename FloatType>
void calcBoundingBox(const vector< DynamicVector<FloatType> > &data, DynamicVector<FloatType> &lower, DynamicVector<FloatType> &upper);

/// computes a random vector with uniform, independent ("rand()") entries from the range [0..1]
template <typename FloatType>
void randomDVector(DynamicVector<FloatType> &v, card32 dim);

template <typename VectorTypeSource, typename VectorTypeDest>
VectorTypeDest convertVectorType(const VectorTypeSource &source, unsigned dim);

template <typename VectorTypeSource, typename VectorTypeDest>
void convertVectorType(const VectorTypeSource &source, VectorTypeDest &dest, unsigned dim);

template <typename MatrixTypeSource, typename MatrixTypeDest>
MatrixTypeDest convertMatrixType(const MatrixTypeSource &source, unsigned cols, unsigned rows);

template <typename MatrixTypeSource, typename MatrixTypeDest>
void convertMatrixType(const MatrixTypeSource &source, MatrixTypeDest &dest, unsigned cols, unsigned rows);



#endif
