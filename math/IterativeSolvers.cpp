//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "IterativeSolvers.h"
#include "IterativeSolvers.hpp"
//---------------------------------------------------------------------------
#include "LinearAlgebra.h"
#include "LinearAlgebra.hpp"
#include "Dynamiclinearalgebra.hpp"
#include "GeoXOutput.h"
#include "Timer.h"
//---------------------------------------------------------------------------



const int MDIM = 10;

void addEntry(SparseMatrix<double> &mat, int xdest, int ydest, int xsource, int ysource, double v, int size) {
   if (xsource<0 || xsource>=size) return;
   if (ysource<0 || ysource>=size) return;
   if (xdest<0 || xdest>=size) return;
   if (ydest<0 || ydest>=size) return;

   int source = xsource + ysource*size;
   int dest = xdest + ydest*size;

   mat[dest].setEntry(source, mat[dest].getEntry(source) + v);
}

void iterativeSolvers_unitTest() {
   // small matrix test
   {
      Vector4f test = makeVector4f(1.0f, 3.14f, 2.0f, 2.71f);
      Matrix4f mat = makeRotX4f(0.32f) * makeRotY4f(-0.39f) * makeRotZ4f(0.76f);
      mat += mat.transpose();
      mat += IDENTITY4F*5;
      Vector4f rhs = mat * test;
      Matrix4f invMat = invertMatrix(mat);
      Vector4f directFloat = invMat * rhs;

      DVectorD testD(4); for (card32 i=0; i<4; i++) testD[i] = test[i];
      DVectorD rhsD(4); for (card32 i=0; i<4; i++) rhsD[i] = rhs[i];
      SparseMatrixD matS(4);
      for (card32 r=0; r<4; r++) {
         for (card32 c=0; c<4; c++) {
            double v = mat[c][r];
            if (v != 0) {
               matS[r].setEntry(c, v);
            }
         }
      }

      output << "Matrix:\n" << mat.toString() << "\n";
      output << "true solution: " << test.toString() << "\n";
      output << "right hand side: " << rhs.toString() << "\n";
      output << "direct float prec. solution: " << directFloat.toString() << "\n\n";

      output << "Matrix:\n" << matS.toString() << "\n";
      output << "true solution: " << testD.toString() << "\n";
      output << "right hand side: " << rhsD.toString() << "\n\n";

      DVectorD x = nullDVector<double>(4);

	  card32 numIterations = 1000;
      gaussSeidelSolve(matS, x, rhsD, numIterations, 1E-20, 1E-5, false);
      output << "Gauss-Seidel solution: " << x.toString() << "\n\n";

	  numIterations = 1000;
      x = nullDVector<double>(4);
      steepestDescentSolve(matS, x, rhsD, numIterations, 1E-5, false);
      output << "Steepest-Descent solution: " << x.toString() << "\n\n";

	  numIterations = 1000;
      x = nullDVector<double>(4);
      conjugateGradientsSolve(matS, x, rhsD, numIterations, 1E-5, true);
      output << "CG solution: " << x.toString() << "\n\n";
   }

   // large matrix test
   {
      SparseMatrixD mat(MDIM*MDIM);
      for (int y=0; y<MDIM; y++) {
         for (int x=0; x<MDIM; x++) {
            addEntry(mat, x,y, x,y,  -4, MDIM);
            addEntry(mat, x,y, x+1,y, 1, MDIM);
            addEntry(mat, x,y, x-1,y, 1, MDIM);
            addEntry(mat, x,y, x,y+1, 1, MDIM);
            addEntry(mat, x,y, x,y-1, 1, MDIM);
         }
      }

      Timer T; card32 vt;

      DVectorD x = nullDVector<double>(MDIM*MDIM);
      DVectorD b = scalarDVector<double>(MDIM*MDIM, 1);
      DVectorD gs, sd, cg;


      x = nullDVector<double>(MDIM*MDIM);
      T.getDeltaValue();
	  card32 numIterations = 100000;
      conjugateGradientsSolve(mat, x, b, numIterations, 1E-7, false);
      vt = T.getDeltaValue();
      output << "CG time: " << vt << "\n\n";
      output << "Large sytem CG solution (LAPL(f) == 1):\n";
      for (int y=0; y<MDIM; y++) {
			for (int xp=0; xp<MDIM; xp++) {
            output << x[xp +y*MDIM] << "\t";
         }
         output.cr();
      }
      output.cr();
      cg = x;

      x = nullDVector<double>(MDIM*MDIM);
      T.getDeltaValue();
	  numIterations = 100000;
      gaussSeidelSolve(mat, x, b, numIterations, 1E-20, 1E-7, false);
      vt = T.getDeltaValue();
      output << "GS time: " << vt << "\n\n";
      output << "Large sytem Gauss-Seidel solution (LAPL(f) == 1):\n";
      for (int y=0; y<MDIM; y++) {
         for (int xp=0; xp<MDIM; xp++) {
           output << x[xp +y*MDIM] << "\t";
         }
        output.cr();
      }
      output.cr();

      gs = x;

      x = nullDVector<double>(MDIM*MDIM);
      T.getDeltaValue();
	  numIterations = 100000;
      steepestDescentSolve(mat, x, b, numIterations, 1E-7, false);
      vt = T.getDeltaValue();
      output << "SD time: " << vt << "\n\n";
      output << "Large sytem steepest descent solution (LAPL(f) == 1):\n";
      for (int y=0; y<MDIM; y++) {
         for (int xp=0; xp<MDIM; xp++) {
            output << (long double)x[xp +y*MDIM] << "\t";
         }
         output.cr();
      }
      output.cr();

      sd = x;

      x = nullDVector<double>(MDIM*MDIM);

      output << "|gs-sd|_2 " << (long double)norm(gs-sd) << "\n";
      output << "|gs-cg|_2 " << (long double)norm(gs-cg) << "\n";

      vector< DVectorD > eigenVects;
      vector< double > eigenvalues;
      DVectorD eigenvector;
      double eigenvalue;
      for (int i=0; i<10; i++) {
         powerIteration(mat, eigenVects, eigenvector, eigenvalue, 1.0001, 1000, false);
         eigenVects.push_back(eigenvector);
         eigenvalues.push_back(eigenvalue);
      }
      for (int i=0; i<10; i++) {
         output << i << " largest of large sytem eigenvector (LAPL(f) == 1):\n";
         for (int y=0; y<MDIM; y++) {
            for (int xp=0; xp<MDIM; xp++) {
               output << eigenVects[i][xp +y*MDIM] << "\t";
            }
            output.cr();
         }
         output.cr();
      }
      output << "eigenvalue:\n";
      for (int i=0; i<10; i++) {
         output << eigenvalues[i] << "\n";
      }

      T.getDeltaValue();

      eigenVects.clear();
      for (int i=0; i<10; i++) {
         GaussSeidelSolver<double> linSolve;
         inversePowerIteration(mat, eigenVects, eigenvector, eigenvalue, &linSolve, 1.0001, 1000, false);
         eigenvalues.push_back(eigenvalue);
         eigenVects.push_back(eigenvector);
      }

      output << "GS-PowerIt time: " << T.getDeltaValue() << "\n\n";

      eigenVects.clear();
      for (int i=0; i<10; i++) {
         CGSolver<double> linSolve;
         inversePowerIteration(mat, eigenVects, eigenvector, eigenvalue, &linSolve, 1.0001, 1000, false);
         eigenvalues.push_back(eigenvalue);
         eigenVects.push_back(eigenvector);
      }

      output << "CG-PowerIt time: " << T.getDeltaValue() << "\n\n";

      eigenVects.clear();
      for (int i=0; i<10; i++) {
         SteepestDescentSolver<double> linSolve;
         inversePowerIteration(mat, eigenVects, eigenvector, eigenvalue, &linSolve, 1.0001, 1000, false);
         eigenvalues.push_back(eigenvalue);
         eigenVects.push_back(eigenvector);
      }

      output << "Steepest descent-PowerIt time: " << T.getDeltaValue() << "\n\n";

      for (int i=0; i<10; i++) {
         output << i << " smallest of large sytem eigenvector (LAPL(f) == 1):\n";
         for (int y=0; y<MDIM; y++) {
            for (int xp=0; xp<MDIM; xp++) {
               output << eigenVects[i][xp +y*MDIM] << "\t";
            }
            output.cr();
         }
         output.cr();
      }
      output << "eigenvalues:\n";
      for (int i=0; i<10; i++) {
         output << eigenvalues[i] << "\n";
      }
        
   }
   {
      Timer T;
      output << "cg scaling test\n";
      for (unsigned i=1; i<21; i++) {
         const int size = i * 10;
         SparseMatrixD mat(size*size);
         for (int y=0; y<size; y++) {
            for (int x=0; x<size; x++) {
               addEntry(mat, x,y, x,y,  -4, size);
               addEntry(mat, x,y, x+1,y, 1, size);
               addEntry(mat, x,y, x-1,y, 1, size);
               addEntry(mat, x,y, x,y+1, 1, size);
               addEntry(mat, x,y, x,y-1, 1, size);
            }
         }

         Timer T; card32 vt;

         DVectorD x = nullDVector<double>(size*size);
         DVectorD b = scalarDVector<double>(size*size, 1);

         T.getDeltaValue();
		 card32 numIterations = 100000;
         conjugateGradientsSolve(mat, x, b, numIterations, 1E-7, false);
         vt = T.getDeltaValue();

         output << i << "\t" << vt << "\n";
      }
   }
   {
      Timer T;
      output << "gs scaling test\n";
      for (unsigned i=1; i<21; i++) {
         const int size = i * 10;
         SparseMatrixD mat(size*size);
         for (int y=0; y<size; y++) {
            for (int x=0; x<size; x++) {
               addEntry(mat, x,y, x,y,  -4, size);
               addEntry(mat, x,y, x+1,y, 1, size);
               addEntry(mat, x,y, x-1,y, 1, size);
               addEntry(mat, x,y, x,y+1, 1, size);
               addEntry(mat, x,y, x,y-1, 1, size);
            }
         }

         Timer T; card32 vt;

         DVectorD x = nullDVector<double>(size*size);
         DVectorD b = scalarDVector<double>(size*size, 1);

         T.getDeltaValue();
		 card32 numIterations = 100000;
         gaussSeidelSolve(mat, x, b, numIterations, 1E-20, 1E-7, false);
         vt = T.getDeltaValue();

         output << i << "\t" << vt << "\n";
      }
   }
   {
      Timer T;
      output << "sd scaling test\n";
      for (unsigned i=1; i<21; i++) {
         const int size = i * 10;
         SparseMatrixD mat(size*size);
         for (int y=0; y<size; y++) {
            for (int x=0; x<size; x++) {
               addEntry(mat, x,y, x,y,  -4, size);
               addEntry(mat, x,y, x+1,y, 1, size);
               addEntry(mat, x,y, x-1,y, 1, size);
               addEntry(mat, x,y, x,y+1, 1, size);
               addEntry(mat, x,y, x,y-1, 1, size);
            }
         }

         Timer T; card32 vt;

         DVectorD x = nullDVector<double>(size*size);
         DVectorD b = scalarDVector<double>(size*size, 1);

         T.getDeltaValue();
		 card32 numIterations = 100000;
         steepestDescentSolve(mat, x, b, numIterations, 1E-7, false);
         vt = T.getDeltaValue();

         output << i << "\t" << vt << "\n";
      }
   }

}



