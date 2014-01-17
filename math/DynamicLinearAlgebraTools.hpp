//---------------------------------------------------------------------------
#include "DynamicLinearAlgebraTools.h"
//---------------------------------------------------------------------------
#include "Dynamiclinearalgebra.hpp"
#include "Random.h"
//---------------------------------------------------------------------------


template <typename FloatType>
void calcBoundingBox(const vector< DynamicVector<FloatType> > &data, DynamicVector<FloatType> &lower, DynamicVector<FloatType> &upper) {
	unsigned n = data.size();
	pAssert(n > 0);
	if (n == 0) return;
	lower = data[0];
	upper = data[0];
	card32 dim = data[0].getDim();
	for (unsigned i=1; i<n; i++) {
		pAssert(data[i].getDim() == dim);
		for (card32 d=0; d<dim; d++) {
			if (data[i][d] < lower[d]) lower[d] = data[i][d];
			if (data[i][d] > upper[d]) upper[d] = data[i][d];
		}
	}
}


template <typename FloatType>
void randomDVector(DynamicVector<FloatType> &v, card32 dim) {
	v.setDim(dim);
	for (card32 i=0; i<dim; i++) {
		v[i] = rnd01();
	}
}

template <typename VectorTypeSource, typename VectorTypeDest>
VectorTypeDest convertVectorType(const VectorTypeSource &source, unsigned dim) {
   VectorTypeDest dest;
   for (unsigned i=0; i<dim; i++) {
      dest[i] = source[i];
   }
   return dest;
}


template <typename VectorTypeSource, typename VectorTypeDest>
void convertVectorType(const VectorTypeSource &source, VectorTypeDest & dest, unsigned dim) {
   for (unsigned i=0; i<dim; i++) {
      dest[i] = source[i];
   }
}


template <typename MatrixTypeSource, typename MatrixTypeDest>
MatrixTypeDest convertMatrixType(const MatrixTypeSource &source, unsigned cols, unsigned rows) {
   MatrixTypeDest dest;
   for (unsigned x=0; x<cols; x++) {
      for (unsigned y=0; y<rows; y++) {
         dest[x][y] = source[x][y];
      }
   }
   return dest;
}

template <typename MatrixTypeSource, typename MatrixTypeDest>
void convertMatrixType(const MatrixTypeSource &source, MatrixTypeDest &dest, unsigned cols, unsigned rows) {
   for (unsigned x=0; x<cols; x++) {
      for (unsigned y=0; y<rows; y++) {
         dest[x][y] = source[x][y];
      }
   }
}
