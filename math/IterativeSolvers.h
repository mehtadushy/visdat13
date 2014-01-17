//---------------------------------------------------------------------------
#ifndef IterativeSolversH
#define IterativeSolversH
//---------------------------------------------------------------------------
#include "SparseLinearAlgebra.h"
#include "DynamicLinearAlgebra.h"
//---------------------------------------------------------------------------



// --- Linear systems solvers


/// computes solution x to ((A*x = b)) using Gauss-Seidel iteration
/// the initial value of x is used as starting point for the iteration and refined sucessively
/// parameter eps is used to test for zeros -  values with |value| < eps are considered zero
/// parameter stopDiff is the maximum absolute norm of the difference in x between two steps at which the
/// method halts and returns the solution
template<typename FloatType>
void gaussSeidelSolve(
   const SparseMatrix<FloatType> &A,
   DynamicVector<FloatType> &x,
   const DynamicVector<FloatType> &b,
   card32& maxNumIterations,
   FloatType eps = (FloatType)1E-20,
   FloatType stopDiff = (FloatType)1E-7,
   bool printGeoXOutput = false
);


/// solves linear system by steepest descent on  0.5 * x^t * A * x  -  b^t * x
/// stopRes is the maximum residual norm at which the procedure should stop
template<typename FloatType>
void steepestDescentSolve(
   const SparseMatrix<FloatType> &A,
   DynamicVector<FloatType> &x,
   const DynamicVector<FloatType> &b,
   card32& maxNumIterations,
   FloatType stopRes = (FloatType)1E-7,
   bool printGeoXOutput = false
);


struct CGStatistics {
   card32 numIterations;
   float64 lastResidualNorm;
};


template<typename FloatType>
void conjugateGradientsSolve(
   const SparseMatrix<FloatType> &A,
   DynamicVector<FloatType> &x,
   const DynamicVector<FloatType> &b,
   card32& maxNumIterations,
   FloatType stopRes = (FloatType)1E-7,
   bool printGeoXOutput = false,
   CGStatistics *statistics = NULL
);

template<typename FloatType>
void conjugateGradientsSolve_Diag_Precond(
	const SparseMatrix<FloatType> &A,
	DynamicVector<FloatType> &x,
	const DynamicVector<FloatType> &b,
	const DynamicVector<FloatType> &diag,
	card32& maxNumIterations,
	FloatType stopRes = (FloatType)1E-7,
	bool printGeoXOutput = false,
   CGStatistics *statistics = NULL
	);

template<typename FloatType>
void preconditioned_ConjugateGradientsSolve(
	const SparseMatrix<FloatType> &A,
	DynamicVector<FloatType> &x,
	const DynamicVector<FloatType> &b,
	const SparseMatrix<FloatType> &precond,
	card32& maxNumIterations,
	FloatType stopRes = (FloatType)1E-7,
	bool printGeoXOutput = false
	);

template<typename FloatType, typename CompPrecision>
void conjugateGradientsSolveOtherPrecision(
   const SparseMatrix<FloatType> &A,
   DynamicVector<FloatType> &x,
   const DynamicVector<FloatType> &b,
   card32& maxNumIterations,
   CompPrecision stopRes = (CompPrecision)1E-7,
   bool printGeoXOutput = false
);


// --- Linear systems solvers - wrapper classes


/// general linear system solver for sparse matrix
/// function object for use as template parameter
template <typename FloatType>
class SparseLinSolver {
 public:
   virtual void solve(const SparseMatrix<FloatType> &A, DynamicVector<FloatType> &x, const DynamicVector<FloatType> &b) = 0;
   virtual ~SparseLinSolver() {}
};

/// Gauss-Seidel linsolver
/// function object for use as template parameter
template <typename FloatType>
class GaussSeidelSolver : public SparseLinSolver<FloatType> {
 public:
   FloatType eps;
   FloatType stopDiff;
   unsigned maxNumIterations;
   bool printGeoXOutput;

   GaussSeidelSolver();
   virtual void solve(const SparseMatrix<FloatType> &A, DynamicVector<FloatType> &x, const DynamicVector<FloatType> &b);
};

/// Steepest descent linsolver for SPD matrices
/// function object for use as template parameter
template <typename FloatType>
class SteepestDescentSolver : public SparseLinSolver<FloatType> {
 public:
   FloatType stopRes;
   unsigned maxNumIterations;
   bool printGeoXOutput;

   SteepestDescentSolver();
   virtual void solve(const SparseMatrix<FloatType> &A, DynamicVector<FloatType> &x, const DynamicVector<FloatType> &b);
};


/// Conjugate gradients linsolver for SPD matrices
/// function object for use as template parameter
template <typename FloatType>
class CGSolver : public SparseLinSolver<FloatType> {
 public:
   FloatType stopRes;
   unsigned maxNumIterations;
   bool printGeoXOutput;

   CGSolver();
   virtual void solve(const SparseMatrix<FloatType> &A, DynamicVector<FloatType> &x, const DynamicVector<FloatType> &b);
};

// --- Eigenvectors/values solvers


/// compute largest eigenvalue of sparse matrix A by power iteration
/// excludedDirection contains directions which are removed (projected & subtracted) from result
/// typically, these are previously computed eigenvectors of larger eigenvalues
/// (repeated application yields the complete spectrum)
/// stopRatio is the change to the eigenvalue at which the iteration stops
/// (stop if ratio of eigenvalues is below stopRatio)
template<typename FloatType, typename MatrixType>
void powerIteration(
   const MatrixType &A,
   const vector< DynamicVector<FloatType> > &excludedDirections,
   DynamicVector<FloatType> &eigenvector,
   FloatType &eigenvalue,
   FloatType stopRatio = (FloatType)1.00001,
   unsigned maxNumIterations = 10000,
   bool printGeoXOutput = false,
   bool nonNegative = false
);

template <typename FloatType>
void	powerIterationGivenInitialValue(
		const SparseMatrix<FloatType> &A,
		vector< DynamicVector<FloatType> > *excludedDirections,
		const vector< DynamicVector<FloatType> > &initialvector,
		DynamicVector<FloatType> &eigenvector,
		FloatType &eigenvalue,
		FloatType stopRatio = (FloatType)1.00001,
		unsigned maxNumIterations = 10000,
		bool printGeoXOutput = false);


/// the same as powerIteration, but applying the procedure to the inverse matrix, extracting the smallest eigenvalues first
/// linSolver refers to a SparseLinSolver class which is used to apply the inverse matrix to a vector repeatedly
template<typename FloatType>
void inversePowerIteration(
   const SparseMatrix<FloatType> &A,
   const vector< DynamicVector<FloatType> > &excludedDirections,
   DynamicVector<FloatType> &eigenvector,
   FloatType &eigenvalue,
   SparseLinSolver<FloatType> *linSolver,
   FloatType stopRatio = (FloatType)1.00001,
   unsigned maxNumIterations = 10000,
   bool printGeoXOutput = false
);


void iterativeSolvers_unitTest();




#endif
