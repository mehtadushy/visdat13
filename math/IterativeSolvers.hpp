//---------------------------------------------------------------------------
#ifndef IterativeSolvers_templCodeH
#define IterativeSolvers_templCodeH
//---------------------------------------------------------------------------
#include "IterativeSolvers.h"
//---------------------------------------------------------------------------
#include "Dynamiclinearalgebra.hpp"
#include "SparseLinearAlgebra.hpp"
#include "GeoXOutput.h"
#include "Random.h"
//---------------------------------------------------------------------------


// ---- linsolvers


template<typename FloatType>
void gaussSeidelSolve(
   const SparseMatrix<FloatType> &A,
   DynamicVector<FloatType> &x,
   const DynamicVector<FloatType> &b,
   card32& maxNumIterations,
   FloatType eps,
   FloatType stopDiff,
   bool printGeoXOutput)
{
   DynamicVector<FloatType> lastX, deltaX;
   const unsigned numRows = A.getRows();
   pAssert(numRows == b.getDim());
   unsigned numIterations = 0;
   FloatType lastStep;
   do {
      lastX = x;
      for (unsigned row=0; row<numRows; row++) {
         FloatType diagElement = (FloatType)0;
         FloatType rowRelax = b[row];
         unsigned thisRowEntries = A[row].getNumSparseEntries();
         for (unsigned j=0; j<thisRowEntries; j++) {
            unsigned colIndex;
            FloatType value;
            A[row].getSparseEntry(j, colIndex, value);
            if (colIndex == row) {
               diagElement = value;
            } else {
               pAssert(colIndex < x.getDim());
               rowRelax -= value * x[colIndex];
            }
         }
         if (fabs(diagElement) < eps) {
            warning("GaussSeidelSolve() - |diagonalEntry| < eps");
            pAssert(false);
            return;
         } else {
            rowRelax /= diagElement;
         }
         x[row] = rowRelax;
      }
      deltaX = x - lastX;
      lastStep = norm(deltaX);
      if (printGeoXOutput) {
         output << "step nr.: " << numIterations;
         output << " stepNorm: " << (long double)lastStep << "\n";
      }
      lastX = x;
      numIterations++;
   } while (lastStep > stopDiff && numIterations < maxNumIterations);
   maxNumIterations = numIterations;
}

template<typename FloatType>
void steepestDescentSolve(
   const SparseMatrix<FloatType> &A,
   DynamicVector<FloatType> &x,
   const DynamicVector<FloatType> &b,
   card32& maxNumIterations,
   FloatType stopRes,
   bool printGeoXOutput)
{
   const unsigned dim = A.getRows();
   DynamicVector<FloatType> residual(dim);
   DynamicVector<FloatType> h(dim);
   unsigned numIterations = 0;
   FloatType residualNorm;
   do {
      A.matVecMult(x, h);
      residual = b - h;
      FloatType resQuad = residual * residual;
      A.matVecMult(residual, h);
      FloatType alpha = resQuad / (residual * h);
      x += residual * alpha;
      numIterations++;
      residualNorm = sqrt(resQuad);
      if (printGeoXOutput) output << "descent step " << numIterations << " res. " << (long double)residualNorm << "\n";
   } while (numIterations < maxNumIterations && residualNorm >= stopRes);
   maxNumIterations = numIterations;
}


template<typename FloatType>
void conjugateGradientsSolve(
   const SparseMatrix<FloatType> &A,
   DynamicVector<FloatType> &x,
   const DynamicVector<FloatType> &b,
   card32& maxNumIterations,
   FloatType stopRes,
   bool printGeoXOutput,
   CGStatistics *statistics)
{
   const unsigned dim = A.getRows();
   DynamicVector<FloatType> residual(dim);
   DynamicVector<FloatType> lastResidual(dim);
   DynamicVector<FloatType> direction(dim);
   DynamicVector<FloatType> a_MUL_direction(dim);
	if (x.getDim() != b.getDim()) {
	   x = nullDVector<FloatType>(b.getDim());
	}
   pAssert(A.getRows() == b.getDim());
   residual = b - A*x;
   direction = residual;
   unsigned numIterations = 0;
   FloatType residualNorm = norm(residual);

   FloatType resQuad = residual * residual;

   while (residualNorm >= stopRes && numIterations < maxNumIterations) {
      A.matVecMult(direction, a_MUL_direction);
      FloatType alpha = resQuad / (direction * a_MUL_direction);
      x += direction * alpha;
      lastResidual = residual;
      residual -= a_MUL_direction * alpha;
      FloatType newResQuad = residual * residual;
      FloatType beta = newResQuad / resQuad;
      direction = residual + direction * beta;
      numIterations++;
      residualNorm = sqrt(newResQuad);
      resQuad = newResQuad;
      if (    printGeoXOutput 
           && ((numIterations % 10) == 0
                || numIterations == 0
                || residualNorm < stopRes
                || numIterations == maxNumIterations ))
      {
         output << "cg step " << numIterations << " res. " << (long double)residualNorm << "\n";
      }
   }
   maxNumIterations = numIterations;
   if (statistics) {
      statistics->lastResidualNorm = residualNorm;
      statistics->numIterations = numIterations;
   }
}

template<typename FloatType>
void conjugateGradientsSolve_Diag_Precond(
	const SparseMatrix<FloatType> &A,
	DynamicVector<FloatType> &x,
	const DynamicVector<FloatType> &b,
	const DynamicVector<FloatType> &diag,
	card32& maxNumIterations,
	FloatType stopRes,
	bool printGeoXOutput,
   CGStatistics *statistics)
{
	const unsigned dim = A.getRows();
	DynamicVector<FloatType> vec_r(dim);
	DynamicVector<FloatType> vec_Mr(dim);
	DynamicVector<FloatType> vec_s(dim);
	DynamicVector<FloatType> vec_As(dim);
	if (x.getDim() != b.getDim()) {
		x = nullDVector<FloatType>(b.getDim());
	}
	pAssert(A.getRows() == b.getDim());
	card32		numIterations = 0, i;
	FloatType	rnorm_c, rnorm_n;
	/// Initialize
	vec_r = b;
	for(i  = 0; i < dim; i++)
	{
		vec_s[i] = diag[i]*vec_r[i];
	}
	rnorm_n = vec_s*vec_r;
	while (numIterations < maxNumIterations) 
	{
		rnorm_c = rnorm_n;
		A.matVecMult(vec_s, vec_As);
		FloatType alpha = rnorm_c/ (vec_s*vec_As);
		for(i = 0; i < dim; i++)
		{
			vec_r[i]		=	vec_r[i]	- vec_As[i]*alpha; 
			x[i]				= x[i]			+ vec_s[i]*alpha;
			vec_Mr[i]	= diag[i]*vec_r[i];
		}
		rnorm_n = vec_r*vec_Mr;
		if (printGeoXOutput) 
		{
			output << "cg step " << numIterations << " res. " << (long double)rnorm_n << "\n";
//   		output.flush();
		}
		if(sqrt(rnorm_n) < stopRes){
			maxNumIterations = numIterations;
			break;
		}
		FloatType beta = rnorm_n/rnorm_c;
		for(i = 0; i < dim; i++)
		{
			vec_s[i] = vec_Mr[i] + vec_s[i]*beta;
		}
		numIterations++;
	}
   if (statistics) {
      statistics->lastResidualNorm = rnorm_n;
      statistics->numIterations = numIterations;
   }
}

template<typename FloatType>
void preconditioned_ConjugateGradientsSolve(
	const SparseMatrix<FloatType> &A,
	DynamicVector<FloatType> &x,
	const DynamicVector<FloatType> &b,
	const SparseMatrix<FloatType> &precond,
	card32& maxNumIterations,
	FloatType stopRes,
	bool printGeoXOutput
	)
{
	const unsigned dim = A.getRows();
	DynamicVector<FloatType> vec_r(dim);
	DynamicVector<FloatType> vec_Mr(dim);
	DynamicVector<FloatType> vec_s(dim);
	DynamicVector<FloatType> vec_As(dim);
	if (x.getDim() != b.getDim()) {
		x = nullDVector<FloatType>(b.getDim());
	}
	pAssert(A.getRows() == b.getDim());
	card32		numIterations = 0, i;
	FloatType	rnorm_c, rnorm_n;
	/// Initialize
	vec_r = b;
	precond.matVecMult(vec_r, vec_s);
	rnorm_n = vec_s*vec_r;
	while (numIterations < maxNumIterations) 
	{
		rnorm_c = rnorm_n;
		A.matVecMult(vec_s, vec_As);
		FloatType alpha = rnorm_c/ (vec_s*vec_As);
		for(i = 0; i < dim; i++)
		{
			vec_r[i]		=	vec_r[i]	- vec_As[i]*alpha; 
			x[i]				= x[i]			+ vec_s[i]*alpha;
		}
		precond.matVecMult(vec_r, vec_Mr);
		rnorm_n = vec_r*vec_Mr;
		if (printGeoXOutput) 
			output << "cg step " << numIterations << " res. " << (long double)rnorm_n << "\n";
//		output.flush();
		if(sqrt(rnorm_n) < stopRes){
			maxNumIterations = numIterations;
			break;
		}
		FloatType beta = rnorm_n/rnorm_c;
		for(i = 0; i < dim; i++)
		{
			vec_s[i] = vec_Mr[i] + vec_s[i]*beta;
		}
		numIterations++;
	}
}

// ---- wrappers


template<typename FloatType>
GaussSeidelSolver<FloatType>::GaussSeidelSolver() {
   eps = (FloatType)1E-20;
   stopDiff = (FloatType)1E-7;
   maxNumIterations = 1000000000;
   printGeoXOutput = false;
}

template<typename FloatType>
void GaussSeidelSolver<FloatType>::solve(const SparseMatrix<FloatType> &A, DynamicVector<FloatType> &x, const DynamicVector<FloatType> &b) {
   gaussSeidelSolve(A, x, b, maxNumIterations, eps, stopDiff, printGeoXOutput);
}


template<typename FloatType>
SteepestDescentSolver<FloatType>::SteepestDescentSolver() {
   stopRes = (FloatType)1E-7;
   maxNumIterations = 1000000000;
   printGeoXOutput = false;
}

template<typename FloatType>
void SteepestDescentSolver<FloatType>::solve(const SparseMatrix<FloatType> &A, DynamicVector<FloatType> &x, const DynamicVector<FloatType> &b) {
   steepestDescentSolve(A, x, b, maxNumIterations, stopRes, printGeoXOutput);
}


template<typename FloatType>
CGSolver<FloatType>::CGSolver() {
   stopRes = (FloatType)1E-7;
   maxNumIterations = 1000000000;
   printGeoXOutput = false;
}

template<typename FloatType>
void CGSolver<FloatType>::solve(const SparseMatrix<FloatType> &A, DynamicVector<FloatType> &x, const DynamicVector<FloatType> &b) {
   conjugateGradientsSolve(A, x, b, maxNumIterations, stopRes, printGeoXOutput);
}


// ---- eigenvalue solvers


template<typename FloatType, typename MatrixType>
void powerIteration(
   const MatrixType &A,
   const vector< DynamicVector<FloatType> > &excludedDirections,
   DynamicVector<FloatType> &eigenvector,
   FloatType &eigenvalue,
   FloatType stopRatio,
   unsigned maxNumIterations,
   bool printGeoXOutput,
   bool nonNegative)
{
   const unsigned dim = A.getRows();
   if (dim == 0) {
      throw PException("powerIteration - Matrix empty.");
   }
   DynamicVector<FloatType> testVect(dim);
   do {
      for (unsigned i=0; i<dim; i++) {
         testVect[i] = (FloatType)(rnd01()-(nonNegative?0:0.5f));
      }
   } while (norm(testVect) < 0.001);

   FloatType eigenvalueEst = (FloatType)1;
   FloatType lastEigenvalueEst = (FloatType)1;
   FloatType eigenValueRatio = stopRatio + (FloatType)1;
   unsigned numIterations = 0;
   do {
      testVect = A * testVect;
      FloatType tnorm = norm(testVect);
      testVect *= ((FloatType)1)/tnorm;

      size_t numExcluded = excludedDirections.size();
      for (unsigned i=0; i<numExcluded; i++) {
         testVect -= excludedDirections[i] * ((testVect * excludedDirections[i]) / (excludedDirections[i]*excludedDirections[i]));
      }

      numIterations++;
      if (numIterations >= 2) {
         eigenvalueEst = tnorm;
         if (numIterations >= 3) {
            eigenValueRatio = fabs(eigenvalueEst / lastEigenvalueEst);
         }
         if (printGeoXOutput) {
            output << "poweriteration - iteration " << numIterations << " eigenvalEst " << eigenvalueEst << "\n";
         }
         lastEigenvalueEst = eigenvalueEst;
      }
   } while (numIterations < maxNumIterations && !(numIterations >= 3 && eigenValueRatio < stopRatio) );
   eigenvalue = eigenvalueEst;
   eigenvector = testVect;
}

template<typename FloatType>
void powerIterationGivenInitialValue(
						  const SparseMatrix<FloatType> &A,
						  vector< DynamicVector<FloatType> > *excludedDirections,
						  const DynamicVector<FloatType> &initialvalue,
						  DynamicVector<FloatType> &eigenvector,
						  FloatType &eigenvalue,
						  FloatType stopRatio,
						  unsigned maxNumIterations,
						  bool printGeoXOutput)
{
	const unsigned dim = A.getRows();

	FloatType eigenvalueEst = (FloatType)1;
	FloatType lastEigenvalueEst = (FloatType)1;
	FloatType eigenValueRatio = stopRatio + (FloatType)1;
	unsigned numIterations = 0;

	eigenvector = initialvalue;
	do {
		eigenvector = A * eigenvector;
		FloatType tnorm = normInfinity(eigenvector);
		eigenvector *= ((FloatType)1)/tnorm;

		if(excludedDirections != NULL)
		{
			size_t numExcluded = excludedDirections->size();
			for (unsigned i=0; i<numExcluded; i++) {
				eigenvector -= (*excludedDirections)[i] * ((eigenvector * (*excludedDirections)[i]) / ((*excludedDirections)[i]*(*excludedDirections)[i]));
			}
		}

		numIterations++;
		if (numIterations >= 2) {
			eigenvalueEst = tnorm;
			if (numIterations >= 3) {
				eigenValueRatio = fabs(eigenvalueEst / lastEigenvalueEst);
			}
			if (printGeoXOutput) {
				output << "poweriteration - iteration " << numIterations << " eigenvalEst " << eigenvalueEst << "\n";
			}
			lastEigenvalueEst = eigenvalueEst;
		}
	} while (numIterations < maxNumIterations && !(numIterations >= 3 && eigenValueRatio < stopRatio) );
	eigenvalue = eigenvalueEst;
}

template<typename FloatType>
void inversePowerIteration(
   const SparseMatrix<FloatType> &A,
   const vector< DynamicVector<FloatType> > &excludedDirections,
   DynamicVector<FloatType> &eigenvector,
   FloatType &eigenvalue,
   SparseLinSolver<FloatType> *linSolver,
   FloatType stopRatio,
   unsigned maxNumIterations,
   bool printGeoXOutput)
{
   const unsigned dim = A.getRows();
   DynamicVector<FloatType> testVect(dim);
   do {
      for (unsigned i=0; i<dim; i++) {
         testVect[i] = (FloatType)(rnd01()-0.5f);
      }
   } while (norm(testVect) < 0.001);

   FloatType eigenvalueEst = (FloatType)1;
   FloatType lastEigenvalueEst = (FloatType)1;
   FloatType eigenValueRatio = stopRatio + (FloatType)1;
   unsigned numIterations = 0;
   DynamicVector<FloatType> testVectorCopy;
   do {
      testVectorCopy = testVect;
      linSolver->solve(A, testVect, testVectorCopy);
      FloatType tnorm = norm(testVect);
      testVect *= ((FloatType)1)/tnorm;

      size_t numExcluded = excludedDirections.size();
      for (unsigned i=0; i<numExcluded; i++) {
         testVect -= excludedDirections[i] * ((testVect * excludedDirections[i]) / (excludedDirections[i]*excludedDirections[i]));
      }

      numIterations++;
      if (numIterations >= 2) {
         eigenvalueEst = tnorm;
         if (numIterations >= 3) {
            eigenValueRatio = fabs(eigenvalueEst / lastEigenvalueEst);
         }
         if (printGeoXOutput) {
            output << "poweriteration - iteration " << numIterations << " eigenvalEst " << eigenvalueEst << "\n";
         }
         lastEigenvalueEst = eigenvalueEst;
      }
   } while (numIterations < maxNumIterations && !(numIterations >= 3 && eigenValueRatio < stopRatio) );
   eigenvalue = (FloatType)1/eigenvalueEst;
   eigenvector = testVect;
}


template<typename FloatType, typename CompPrecision>
void conjugateGradientsSolveOtherPrecision(
   const SparseMatrix<FloatType> &A,
   DynamicVector<FloatType> &x,
   const DynamicVector<FloatType> &b,
   CompPrecision stopRes,
   unsigned maxNumIterations,
   bool printGeoXOutput)
{
   SparseMatrix<CompPrecision> Ap;
   DynamicVector<CompPrecision> xp;
   DynamicVector<CompPrecision> bp;
   convertSparseMatrixType<FloatType, CompPrecision>(A, Ap);
   convertDVectorType<FloatType, CompPrecision>(x, xp);
   convertDVectorType<FloatType, CompPrecision>(b, bp);
   card32 numIterations = maxNumIterations;
   conjugateGradientsSolve<CompPrecision>(Ap, xp, bp, numIterations, stopRes, printGeoXOutput);
   convertDVectorType<CompPrecision, FloatType>(xp, x);
}






#endif

