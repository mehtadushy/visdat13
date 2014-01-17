//---------------------------------------------------------------------------
#ifndef LinearAlgebra_templCodeH
#define LinearAlgebra_templCodeH
//---------------------------------------------------------------------------
#include "LinearAlgebra.h"
//---------------------------------------------------------------------------
#include "Exceptions.h"
#include "PAssert.h"
#include <string>
#include <math.h>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------

#ifdef _DEBUG
#define SVECT_RANGE_CHECK
#endif



// ---------------------------------------------------------------------------
//
//                                 StaticVector
//
// ---------------------------------------------------------------------------



template <class FloatType, unsigned dim>
inline FloatType& StaticVector<FloatType, dim>::operator[](const unsigned &index) {
   #ifdef SVECT_RANGE_CHECK
   if (index >= dim) {
      throw ERangeCheck("StaticVector::operator[] - index exceeds bounds");
   }
   #endif
   return v[index];
}

template <class FloatType, unsigned dim>
inline const FloatType& StaticVector<FloatType, dim>::operator[](const unsigned &index) const{
   #ifdef SVECT_RANGE_CHECK
   if (index >= dim) {
      throw ERangeCheck("StaticVector::operator[] - index exceeds bounds");
   }
   #endif
   return v[index];
}

template <class FloatType, unsigned dim>
inline StaticVector<FloatType, dim> StaticVector<FloatType, dim>::operator+(const StaticVector<FloatType, dim> &op) const {
   StaticVector<FloatType, dim> result;
   for (unsigned i=0; i<dim; i++) {
      result[i] = v[i] + op.v[i];
   }
   return result;
}

template <class FloatType, unsigned dim>
inline StaticVector<FloatType, dim> StaticVector<FloatType, dim>::operator-(const StaticVector<FloatType, dim> &op) const {
   StaticVector<FloatType, dim> result;
   for (unsigned i=0; i<dim; i++) {
      result[i] = v[i] - op.v[i];
   }
   return result;
}

template <class FloatType, unsigned dim>
inline StaticVector<FloatType, dim> StaticVector<FloatType, dim>::operator-() const {
   StaticVector<FloatType, dim> result;
   for (unsigned i=0; i<dim; i++) {
      result[i] = -v[i];
   }
   return result;
}

template <class FloatType, unsigned dim>
inline FloatType StaticVector<FloatType, dim>::operator*(const StaticVector<FloatType, dim> op) const {
   FloatType result = 0.0;
   for (unsigned i=0; i<dim; i++) {
      result += v[i] * op.v[i];
   }
   return result;
}

template <class FloatType, unsigned dim>
inline StaticVector<FloatType, dim> StaticVector<FloatType, dim>::operator*(const FloatType &s) const {
   StaticVector<FloatType, dim> result;
   for (unsigned i=0; i<dim; i++) {
      result[i] = s * v[i];
   }
   return result;
}

template <class FloatType, unsigned dim>
inline StaticVector<FloatType, dim> StaticVector<FloatType, dim>::operator/(const FloatType &s) const {
   StaticVector<FloatType, dim> result;
   for (unsigned i=0; i<dim; i++) {
      result[i] = v[i] / s;
   }
   return result;
}

template <class FloatType, unsigned dim>
StaticVector<FloatType, dim> StaticVector<FloatType, dim>::crossProduct(const StaticVector<FloatType, dim> &op) const {
   throw PException("StaticVector<FloatType,dim>::crossProduct() is only defined for dim == 3");
}

template <class FloatType, unsigned dim>
inline StaticVector<FloatType, dim> StaticVector<FloatType, dim>::componentProduct(const StaticVector<FloatType, dim> &op) const {
   StaticVector<FloatType, dim> result;
   for (unsigned i=0; i<dim; i++) {
      result[i] = v[i] * op.v[i];
   }
   return result;
}


template <class FloatType, unsigned dim>
inline StaticVector<FloatType, dim> StaticVector<FloatType, dim>::operator+=(const StaticVector<FloatType, dim> &op) {
   for (unsigned i=0; i<dim; i++) {
      v[i] += op.v[i];
   }
   return *this;
}

template <class FloatType, unsigned dim>
inline StaticVector<FloatType, dim> StaticVector<FloatType, dim>::operator-=(const StaticVector<FloatType, dim> &op) {
   for (unsigned i=0; i<dim; i++) {
      v[i] -= op.v[i];
   }
   return *this;
}

template <class FloatType, unsigned dim>
inline StaticVector<FloatType, dim> StaticVector<FloatType, dim>::operator*=(const FloatType &s) {
   for (unsigned i=0; i<dim; i++) {
      v[i] *= s;
   }
   return *this;
}

template <class FloatType, unsigned dim>
inline StaticVector<FloatType, dim> StaticVector<FloatType, dim>::operator/=(const FloatType &s) {
   for (unsigned i=0; i<dim; i++) {
      v[i] /= s;
   }
   return *this;
}

template <class FloatType, unsigned dim>
inline bool StaticVector<FloatType, dim>::operator==(const StaticVector<FloatType, dim> op) const {
   for (unsigned i=0; i<dim; i++) {
      if (v[i] != op.v[i]) return false;
   }
   return true;
}

template <class FloatType, unsigned dim>
inline bool StaticVector<FloatType, dim>::operator!=(const StaticVector<FloatType, dim> op) const {
   for (unsigned i=0; i<dim; i++) {
      if (v[i] != op.v[i]) return true;
   }
   return false;
}

template <class FloatType, unsigned dim>
void StaticVector<FloatType, dim>::print() {
   cout << '(';
   for (unsigned i=0; i<dim-1; i++) {
      cout << v[i];
      cout << ',';
      cout << ' ';
   }
   cout << v[dim-1];
   cout << ')';
}

template <class FloatType, unsigned dim>
string StaticVector<FloatType, dim>::toString() const {
   string result;
   result += '(';
   char buffer[1000];
   for (unsigned i=0; i<dim-1; i++) {
      sprintf(buffer, "%g", v[i]);
      result += buffer;
      result += ',';
      result += ' ';
   }
   sprintf(buffer, "%g", v[dim-1]);
   result += buffer;
   result += ')';
   return result;
}

template <class FloatType, unsigned dim>
inline FloatType* StaticVector<FloatType, dim>::data() {
   return &(v[0]);
}

template <class FloatType, unsigned dim>
inline const FloatType* StaticVector<FloatType, dim>::data() const {
   return &(v[0]);
}

template <class FloatType, unsigned dim>
inline void StaticVector<FloatType, dim>::normalize() {
   FloatType norm = 0.0;
   for (int i=0; i<dim; i++) {
      norm += v[i] * v[i];
   }
   if ( norm > 1e-7 )
      (*this) = (*this) / sqrt( norm );
}

template <class FloatType, unsigned dim>
inline FloatType StaticVector<FloatType, dim>::getSqrNorm() const {
   FloatType norm = (FloatType)0.0;
   for (unsigned i=0; i<dim; i++) {
      norm += v[i] * v[i];
   }
   return norm;
}

template <class FloatType, unsigned dim>
inline StaticVector<FloatType, dim>& StaticVector<FloatType, dim>::setZero() {
   for (unsigned i=0; i<dim; i++)
      v[i] = (FloatType)0;
   return *this;
}

template<>
inline StaticVector<float32, 3> StaticVector<float32, 3>::crossProduct(const StaticVector<float32, 3> &op) const {
   StaticVector<float32, 3> result;
   result.v[0] = v[1]*op.v[2] - v[2]*op.v[1];
   result.v[1] = v[2]*op.v[0] - v[0]*op.v[2];
   result.v[2] = v[0]*op.v[1] - v[1]*op.v[0];
   return result;
}

template<>
inline StaticVector<float64, 3> StaticVector<float64, 3>::crossProduct(const StaticVector<float64, 3> &op) const {
   StaticVector<float64, 3> result;
   result.v[0] = v[1]*op.v[2] - v[2]*op.v[1];
   result.v[1] = v[2]*op.v[0] - v[0]*op.v[2];
   result.v[2] = v[0]*op.v[1] - v[1]*op.v[0];
   return result;
}

template<>
inline StaticVector<int32, 3> StaticVector<int32, 3>::crossProduct(const StaticVector<int32, 3> &op) const {
   StaticVector<int32, 3> result;
   result.v[0] = v[1]*op.v[2] - v[2]*op.v[1];
   result.v[1] = v[2]*op.v[0] - v[0]*op.v[2];
   result.v[2] = v[0]*op.v[1] - v[1]*op.v[0];
   return result;
}


// ---- helper functions


template <class FloatTypeDest, class FloatTypeSource, int Dim>
StaticVector<FloatTypeDest, Dim> convertVector(const StaticVector<FloatTypeSource, Dim> &source) {
   StaticVector<FloatTypeDest, Dim> result;
   for (int i=0; i<Dim; i++) {
      result[i] = (FloatTypeDest)source[i];
   }
   return result;
}



// ---------------------------------------------------------------------------
//
//                                 StaticMatrix
//
// ---------------------------------------------------------------------------



template <class FloatType, unsigned columns, unsigned rows>
inline StaticVector<FloatType, rows>& StaticMatrix<FloatType, columns, rows>::operator[](const unsigned &index) {
   #ifdef SVECT_RANGE_CHECK
   if (index >= columns) {
      throw ERangeCheck("StaticMatrix::operator[] - index exceeds bounds");
   }
   #endif
   return theColumns[index];
}

template <class FloatType, unsigned columns, unsigned rows>
inline const StaticVector<FloatType, rows>& StaticMatrix<FloatType, columns, rows>::operator[](const unsigned &index) const {
   #ifdef SVECT_RANGE_CHECK
   if (index >= columns) {
      throw ERangeCheck("StaticMatrix::operator[] - index exceeds bounds");
   }
   #endif
   return theColumns[index];
}

template <class FloatType, unsigned columns, unsigned rows>
inline StaticMatrix<FloatType, columns, rows> StaticMatrix<FloatType, columns, rows>::operator+(const StaticMatrix<FloatType, columns, rows> &op) const {
   StaticMatrix<FloatType, columns, rows> result;
   for (unsigned i=0; i<columns; i++) {
      result[i] = theColumns[i] + op.theColumns[i];
   }
   return result;
}

template <class FloatType, unsigned columns, unsigned rows>
inline StaticMatrix<FloatType, columns, rows> StaticMatrix<FloatType, columns, rows>::operator-(const StaticMatrix<FloatType, columns, rows> &op) const {
   StaticMatrix<FloatType, columns, rows> result;
   for (unsigned i=0; i<columns; i++) {
      result[i] = theColumns[i] - op.theColumns[i];
   }
   return result;
}

template <class FloatType, unsigned columns, unsigned rows>
inline StaticMatrix<FloatType, columns, rows> StaticMatrix<FloatType, columns, rows>::operator-() const {
   StaticMatrix<FloatType, columns, rows> result;
   for (unsigned i=0; i<columns; i++) {
      result[i] = -theColumns[i];
   }
   return result;
}

template <class FloatType, unsigned columns, unsigned rows>
inline StaticMatrix<FloatType, columns, rows> StaticMatrix<FloatType, columns, rows>::operator*(const FloatType &s) const {
   StaticMatrix<FloatType, columns, rows> result;
   for (unsigned i=0; i<columns; i++) {
      result[i] = theColumns[i] * s;
   }
   return result;
}


/* This:    colums: columns
            rows: rows
   Operand: colums: colums2
            rows: columns
   Result:  colums: columns2
            rows: rows
*/
template <class FloatType, unsigned columns, unsigned rows>
template <unsigned columns2>
StaticMatrix<FloatType, columns2, rows> StaticMatrix<FloatType, columns, rows>::operator*( const StaticMatrix<FloatType, columns2, columns>& mat ) const
{
   StaticMatrix<FloatType, columns2, rows> res;
   for ( unsigned char row = 0; row < rows; row++ ){
      for ( unsigned char column = 0; column < columns2; column++ ){
         res[column][row] = 0;
         for ( unsigned char k = 0; k < columns; k++ )
            res[column][row] += theColumns[k][row] * mat[column][k];
      }
   }
   return res;
}

template <class FloatType, unsigned columns, unsigned rows>
inline StaticMatrix<FloatType, columns, rows> StaticMatrix<FloatType, columns, rows>::operator/(const FloatType &s) const {
   StaticMatrix<FloatType, columns, rows> result;
   for (unsigned i=0; i<columns; i++) {
      result[i] = theColumns[i] / s;
   }
   return result;
}

template <class FloatType, unsigned columns, unsigned rows>
inline StaticMatrix<FloatType, columns, rows> StaticMatrix<FloatType, columns, rows>::operator+=(const StaticMatrix<FloatType, columns, rows> &op) {
   for (unsigned i=0; i<columns; i++) {
      theColumns[i] += op.theColumns[i];
   }
   return *this;
}

template <class FloatType, unsigned columns, unsigned rows>
inline StaticMatrix<FloatType, columns, rows> StaticMatrix<FloatType, columns, rows>::operator-=(const StaticMatrix<FloatType, columns, rows> &op) {
   for (unsigned i=0; i<columns; i++) {
      theColumns[i] -= op.theColumns[i];
   }
   return *this;
}

template <class FloatType, unsigned columns, unsigned rows>
inline StaticMatrix<FloatType, columns, rows> StaticMatrix<FloatType, columns, rows>::operator*=(const FloatType &op) {
   for (unsigned i=0; i<columns; i++) {
      theColumns[i] *= op;
   }
   return *this;
}

template <class FloatType, unsigned columns, unsigned rows>
inline StaticMatrix<FloatType, columns, rows> StaticMatrix<FloatType, columns, rows>::operator*=(const StaticMatrix<FloatType, columns, rows> &op) {
   StaticMatrix<FloatType, columns, rows> result = (*this)*op;
   *this = result;
   return result;
}

template <class FloatType, unsigned columns, unsigned rows>
inline StaticMatrix<FloatType, columns, rows> StaticMatrix<FloatType, columns, rows>::operator/=(const FloatType &op) {
   for (unsigned i=0; i<columns; i++) {
      theColumns[i] /= op;
   }
   return *this;
}

template <class FloatType, unsigned columns, unsigned rows>
inline StaticMatrix<FloatType, columns, rows> StaticMatrix<FloatType, columns, rows>::operator=(const StaticMatrix<FloatType, columns, rows> &op) {
   for (unsigned i=0; i<columns; i++) {
      theColumns[i] = op.theColumns[i];
   }
   return *this;
}

template <class FloatType, unsigned columns, unsigned rows>
inline bool StaticMatrix<FloatType, columns, rows>::operator==(const StaticMatrix<FloatType, columns, rows> &op) const {
   for (unsigned i=0; i<columns; i++) {
      if (theColumns[i] != op.theColumns[i]) return false;
   }
   return true;
}

template <class FloatType, unsigned columns, unsigned rows>
inline bool StaticMatrix<FloatType, columns, rows>::operator!=(const StaticMatrix<FloatType, columns, rows> &op) const {
   for (unsigned i=0; i<columns; i++) {
      if (theColumns[i] != op.theColumns[i]) return true;
   }
   return false;
}

template <class FloatType, unsigned columns, unsigned rows>
inline StaticVector<FloatType, rows> StaticMatrix<FloatType, columns, rows>::operator*(const StaticVector<FloatType, columns> &v) const {
   StaticVector<FloatType, rows> result;
   for (unsigned y=0; y<rows; y++) {
      result[y] = 0.0;
      for (unsigned x=0; x<columns; x++) {
         result[y] += theColumns[x][y]*v[x];
      }
   }
   return result;
}

template <class FloatType, unsigned columns, unsigned rows>
inline StaticMatrix<FloatType, columns, rows>::StaticMatrix() {
   for (unsigned j=0; j<columns; j++) {
      for (unsigned i=0; i<rows; i++) {
         if (i == j)
            theColumns[j][i] = 1.0;
         else
            theColumns[j][i] = 0.0;
      }
   }
}

template <class FloatType, unsigned columns, unsigned rows>
inline void StaticMatrix<FloatType, columns, rows>::print() {
   for (unsigned i=0; i<columns; i++) {
      theColumns[i].print();
      cout << endl;
   }
}

template <class FloatType, unsigned columns, unsigned rows>
string StaticMatrix<FloatType, columns, rows>::toString() const {
   string result;
   for (unsigned r=0; r<rows; r++) {
      result += '(';
      char buffer[1000];
      for (unsigned c=0; c<columns-1; c++) {
         sprintf(buffer, "%.8f", theColumns[c][r]);
         result += buffer;
         result += ',';
         result += ' ';
      }
      sprintf(buffer, "%.8f", theColumns[columns-1][r]);
      result += buffer;
      result += ") \n";
   }
   return result;
}

template <class FloatType, unsigned columns, unsigned rows>
inline FloatType* StaticMatrix<FloatType, columns, rows>::data() {
   return &(theColumns[0][0]);
}

template <class FloatType, unsigned columns, unsigned rows>
inline const FloatType* StaticMatrix<FloatType, columns, rows>::data() const {
   return &(theColumns[0][0]);
}

template <class FloatType, unsigned columns, unsigned rows>
inline void StaticMatrix<FloatType, columns, rows>::changeRows(unsigned row1, unsigned row2) {
   StaticVector<FloatType, columns> h;
   unsigned i;
   for (i=0; i<columns; i++) {
      h[i] = theColumns[i][row2];
   }
   for (i=0; i<columns; i++) {
      theColumns[i][row2] = theColumns[i][row1];
   }
   for (i=0; i<columns; i++) {
      theColumns[i][row1] = h[i];
   }
}

template <class FloatType, unsigned columns, unsigned rows>
inline void StaticMatrix<FloatType, columns, rows>::multRow(unsigned row, FloatType value) {
   for (unsigned i=0; i<columns; i++) {
      theColumns[i][row] *= value;
   }
}

template <class FloatType, unsigned columns, unsigned rows>
inline void StaticMatrix<FloatType, columns, rows>::combineRows(unsigned row, unsigned with, FloatType by) {
   for (unsigned i=0; i<columns; i++) {
      theColumns[i][row] += theColumns[i][with] * by;
   }
}

template <class FloatType, unsigned columns, unsigned rows>
inline void StaticMatrix<FloatType, columns, rows>::addRows(unsigned row, unsigned with) {
   for (unsigned i=0; i<columns; i++) {
      theColumns[i][row] += theColumns[i][with];
   }
}

template <class FloatType, unsigned columns, unsigned rows>
inline StaticMatrix<FloatType, rows, columns> StaticMatrix<FloatType, columns, rows>::transpose() const {
   StaticMatrix<FloatType, rows, columns> result;
   for (unsigned r=0; r<rows; r++) {
      for (unsigned c=0; c<columns; c++) {
         result[r][c] = theColumns[c][r];
      }
   }
   return result;
}

template <class FloatType, unsigned columns, unsigned rows>
inline StaticMatrix<FloatType, columns, rows>& StaticMatrix<FloatType, columns, rows>::setZero() {
   for (unsigned i=0; i<columns; i++) {
      for (unsigned j=0; j<rows; j++) {
         theColumns[i][j] = (FloatType)0;
      }
   }

   return *this;
}

template <class FloatType, unsigned columns, unsigned rows>
inline void StaticMatrix<FloatType, columns, rows>::setIdentity() {
   setZero();
   unsigned minDim = min(columns, rows);
   for (unsigned i=0; i<minDim; i++) {
      theColumns[i][i] = (FloatType)1;
   }
}

template <class FloatType, unsigned columns, unsigned rows>
inline FloatType StaticMatrix<FloatType, columns, rows>::getDeterminant() const {
   if ( ( rows == 2 ) && ( columns == 2 ) )
      return (*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0];
   else if ( ( rows == 3 ) && ( columns == 3 ) )
      return (*this)[0][0] * ( (*this)[1][1] * (*this)[2][2] - (*this)[1][2] * (*this)[2][1] ) -
             (*this)[0][1] * ( (*this)[1][0] * (*this)[2][2] - (*this)[1][2] * (*this)[2][0] ) +
             (*this)[0][2] * ( (*this)[1][0] * (*this)[2][1] - (*this)[1][1] * (*this)[2][0] );
   else if ( rows == 1 && columns == 1)
      return (*this)[0][0];
   else
      // limitiation: does not work for dimensions higher larger 3
      pAssert(false);
}

/**
 *  Helping structure for jacobi iteration
 */
#define STATIC_MATRIX_ROTATE(a,i,j,k,l) g=a[i][j];\
                                        h=a[k][l];\
                                        a[i][j]=g-s*(h+g*tau);\
                                        a[k][l]=h+s*(g-h*tau);

template <class FloatType, unsigned columns, unsigned rows>
void StaticMatrix<FloatType, columns, rows>::eigenSort( StaticVector<FloatType,rows>& eigenValues, StaticMatrix<FloatType,rows,columns>& eigenVectors ) const
{
   int i,j,k;
   FloatType p;
   for ( i = 0; i < rows-1; i++ ){
      p = eigenValues[k=i];
      for( j = i+1; j < rows; j++ )
         if( eigenValues[j] >= p ) p = eigenValues[k=j];
      if( k != i ){
         eigenValues[k] = eigenValues[i];
         eigenValues[i] = p;
         for( j = 0; j < rows; j++ ){
            p = eigenVectors[j][i];
            eigenVectors[j][i] = eigenVectors[j][k];
            eigenVectors[j][k] = p;
         }
      }
   }
}

template <class FloatType, unsigned columns, unsigned rows>
void StaticMatrix<FloatType, columns, rows>::solveEigenProblemSymmetric( StaticVector<FloatType,rows>& eigenValues, StaticMatrix<FloatType,rows,columns>& eigenVectors ) const
{
   //#pragma option push -w-8008
   //#pragma option push -w-8066
   if ( rows != columns ){
      cout << "solveEigenProblemSymmetric: Only squared matrices allowed!" << endl;
      return;
   }
   //#pragma option pop
   //#pragma option pop
   StaticMatrix<FloatType,rows,columns> a = *this;
   int j,iq,ip,i;
   FloatType tresh,theta,tau,t,s,h,g,c,sm = 0;
   StaticVector<FloatType,rows> b;
   StaticVector<FloatType,rows> z;
   for ( ip = 0; ip < rows; ip++) {
      for ( iq = 0; iq < rows; iq++ )
         eigenVectors[ip][iq] = 0.0;
      eigenVectors[ip][ip] = 1.0;
   }
   for ( ip = 0; ip < rows; ip++ ) {
      b[ip] = eigenValues[ip] = a[ip][ip];
      z[ip] = 0.0;
   }
   int nrot = 0;
   for ( i = 1; i <= 100; i++ ) {
      sm = 0;
      for ( ip = 0; ip < rows-1; ip++ ) {
         for ( iq = ip + 1; iq < rows; iq++ )
            sm += fabs( a[ip][iq] );
      }
      if ( sm == 0 ) {
         eigenSort( eigenValues, eigenVectors );
         eigenVectors = eigenVectors.transpose();
         return;
      }
      if ( i < 4 )
         tresh = 0.2f * sm / (FloatType)( rows * rows );
      else
         tresh = 0.0;
      for ( ip = 0; ip < rows - 1; ip++ ) {
         for ( iq = ip + 1; iq < rows; iq++ ) {
            g = (FloatType) 100.0f * fabs( (FloatType) a[ip][iq] );
            if ( ( i > 4 ) &&
               ( (float)( fabs( eigenValues[ip] ) + g ) == (float)fabs( eigenValues[ip] ) ) &&
               ( (float)( fabs( eigenValues[iq] ) + g ) == (float)fabs( eigenValues[iq] ) ) )
               a[ip][iq] = 0.0;
            else if ( fabs( a[ip][iq] ) > tresh ) {
               h = eigenValues[iq] - eigenValues[ip];
               if ( (float)(fabs( h ) + g ) == (float)fabs( h ) )
                  t = ( a[ip][iq] ) / h;
               else {
                  theta = 0.5f * h / ((FloatType) a[ip][iq] );
                  t = 1.0f / (fabs( theta ) + sqrt( 1.0f + theta*theta ));
                  if ( theta < 0.0f ) t = -t;
               }
               c = 1.0f / (FloatType)sqrt( 1.0f + t * t );
               s = t * c;
               tau = s / ( 1.0f + c );
               h = t * a[ip][iq];
               z[ip] -= h;
               z[iq] += h;
               eigenValues[ip] -= h;
               eigenValues[iq] += h;
               a[ip][iq] = 0.0;
               for ( j = 0; j <= ip - 1; j++ ) {
                  STATIC_MATRIX_ROTATE( a, j, ip, j, iq )
               }
               for ( j = ip + 1; j <= iq - 1; j++ ) {
                  STATIC_MATRIX_ROTATE( a, ip, j, j, iq )
               }
               for ( j = iq + 1; j < rows; j++ ) {
                  STATIC_MATRIX_ROTATE( a, ip, j, iq, j )
               }
               for ( j = 0; j < rows; j++ ) {
                  STATIC_MATRIX_ROTATE( eigenVectors, j, ip, j, iq )
               }
               ++nrot;
            }
         }
      }
      for ( ip = 0; ip < rows; ip++ ) {
         b[ip] += z[ip];
         eigenValues[ip] = b[ip];
         z[ip] = 0.0;
      }
   }
   cout << "Too many iterations in routine get Eigenvalues/vectors" << endl;
}


#include <tnt/jama_eig.h>

template <class FloatType, unsigned columns, unsigned rows>
void StaticMatrix<FloatType, columns, rows>::solveEigenProblem(StaticVector< FloatType, rows >& EigenValuesReal,
                                                               StaticVector< FloatType, rows >& EigenValuesImaginary,
                                                               StaticMatrix< FloatType, rows, columns >& EigenVectors) const
{
    //While JAMA can also deal with non-quadratic matrices, I do not want to code this here.
    if (rows != columns)
    {
        cout << "solveEigenProblem: Only quadratic matrices allowed!" << endl;
        return;
    }

    //Copy into a TNT Array2D, which is row-major (in contrast, StaticMatrix is column-major)
    TNT::Array2D< FloatType > MatrixJAMA(rows, columns);
    // - run over each col of StaticMatrix
    for(unsigned int idxColumn=0;idxColumn<columns;idxColumn++)
    {
        const StaticVector< FloatType, rows >& ThisColumn = theColumns[idxColumn];

        //Copy into the rows
        for(unsigned int idxRow=0;idxRow<rows;idxRow++)
        {
            MatrixJAMA[idxRow][idxColumn] = ThisColumn[idxRow];
        }
    }

    //Calculate EigenValues
    JAMA::Eigenvalue< FloatType > EigenValueJAMA(MatrixJAMA);

    //Get the EigenValues and EigenVectors
    Array1D< FloatType > RealPartJAMA(rows);
    EigenValueJAMA.getRealEigenvalues(RealPartJAMA);
    Array1D< FloatType > ImagPartJAMA(rows);
    EigenValueJAMA.getImagEigenvalues(ImagPartJAMA);
    Array2D< FloatType > EigenVectorsJAMA(rows, rows);
    EigenValueJAMA.getV(EigenVectorsJAMA);

    //Copy them to our data structures
    for(unsigned int i=0;i<rows;i++)
    {
        EigenValuesReal[i] = RealPartJAMA[i];
        EigenValuesImaginary[i] = ImagPartJAMA[i];

        for(unsigned int j=0;j<columns;j++)
        {
            EigenVectors[j][i] = EigenVectorsJAMA[i][j];
        }
    }
}


// --- explicit specialization for dim 2,3,4 cases:


#ifdef LINEAR_ALGEBRA_EXPLICIT_LOOP_UNROOLING


template<>
inline StaticVector<float32, 4> StaticVector<float32, 4>::operator+(const StaticVector<float32, 4> &op) const {
   StaticVector<float32, 4> result;
   result.v[0] = v[0] + op.v[0];
   result.v[1] = v[1] + op.v[1];
   result.v[2] = v[2] + op.v[2];
   result.v[3] = v[3] + op.v[3];
   return result;
}
template<>
inline StaticVector<float32, 3> StaticVector<float32, 3>::operator+(const StaticVector<float32, 3> &op) const {
   StaticVector<float32, 3> result;
   result.v[0] = v[0] + op.v[0];
   result.v[1] = v[1] + op.v[1];
   result.v[2] = v[2] + op.v[2];
   return result;
}
template<>
inline StaticVector<float32, 2> StaticVector<float32, 2>::operator+(const StaticVector<float32, 2> &op) const {
   StaticVector<float32, 2> result;
   result.v[0] = v[0] + op.v[0];
   result.v[1] = v[1] + op.v[1];
   return result;
}

template<>
inline StaticVector<float32, 4> StaticVector<float32, 4>::operator-(const StaticVector<float32, 4> &op) const {
   StaticVector<float32, 4> result;
   result.v[0] = v[0] - op.v[0];
   result.v[1] = v[1] - op.v[1];
   result.v[2] = v[2] - op.v[2];
   result.v[3] = v[3] - op.v[3];
   return result;
}
template<>
inline StaticVector<float32, 3> StaticVector<float32, 3>::operator-(const StaticVector<float32, 3> &op) const {
   StaticVector<float32, 3> result;
   result.v[0] = v[0] - op.v[0];
   result.v[1] = v[1] - op.v[1];
   result.v[2] = v[2] - op.v[2];
   return result;
}
template<>
inline StaticVector<float32, 2> StaticVector<float32, 2>::operator-(const StaticVector<float32, 2> &op) const {
   StaticVector<float32, 2> result;
   result.v[0] = v[0] - op.v[0];
   result.v[1] = v[1] - op.v[1];
   return result;
}

template<>
inline StaticVector<float32, 4> StaticVector<float32, 4>::operator-() const {
   StaticVector<float32, 4> result;
   result.v[0] = -v[0];
   result.v[1] = -v[1];
   result.v[2] = -v[2];
   result.v[3] = -v[3];
   return result;
}
template<>
inline StaticVector<float32, 3> StaticVector<float32, 3>::operator-() const {
   StaticVector<float32, 3> result;
   result.v[0] = -v[0];
   result.v[1] = -v[1];
   result.v[2] = -v[2];
   return result;
}
template<>
inline StaticVector<float32, 2> StaticVector<float32, 2>::operator-() const {
   StaticVector<float32, 2> result;
   result.v[0] = -v[0];
   result.v[1] = -v[1];
   return result;
}

template<>
inline float32 StaticVector<float32, 4>::operator*(const StaticVector<float32, 4> op) const {
   float32 result = 0.0;
   result += v[0] * op.v[0];
   result += v[1] * op.v[1];
   result += v[2] * op.v[2];
   result += v[3] * op.v[3];
   return result;
}
template<>
inline float32 StaticVector<float32, 3>::operator*(const StaticVector<float32, 3> op) const {
   float32 result = 0.0;
   result += v[0] * op.v[0];
   result += v[1] * op.v[1];
   result += v[2] * op.v[2];
   return result;
}
template<>
inline float32 StaticVector<float32, 2>::operator*(const StaticVector<float32, 2> op) const {
   float32 result = 0.0;
   result += v[0] * op.v[0];
   result += v[1] * op.v[1];
   return result;
}

template<>
inline StaticVector<float32, 4> StaticVector<float32, 4>::operator*(const float32 &s) const {
   StaticVector<float32, 4> result;
   result.v[0] = s * v[0];
   result.v[1] = s * v[1];
   result.v[2] = s * v[2];
   result.v[3] = s * v[3];
   return result;
}
template<>
inline StaticVector<float32, 3> StaticVector<float32, 3>::operator*(const float32 &s) const {
   StaticVector<float32, 3> result;
   result.v[0] = s * v[0];
   result.v[1] = s * v[1];
   result.v[2] = s * v[2];
   return result;
}
template<>
inline StaticVector<float32, 2> StaticVector<float32, 2>::operator*(const float32 &s) const {
   StaticVector<float32, 2> result;
   result.v[0] = s * v[0];
   result.v[1] = s * v[1];
   return result;
}

template<>
inline StaticVector<float32, 4> StaticVector<float32, 4>::operator/(const float32 &s) const {
   StaticVector<float32, 4> result;
   result.v[0] = v[0] / s;
   result.v[1] = v[1] / s;
   result.v[2] = v[2] / s;
   result.v[3] = v[3] / s;
   return result;
}
template<>
inline StaticVector<float32, 3> StaticVector<float32, 3>::operator/(const float32 &s) const {
   StaticVector<float32, 3> result;
   result.v[0] = v[0] / s;
   result.v[1] = v[1] / s;
   result.v[2] = v[2] / s;
   return result;
}
template<>
inline StaticVector<float32, 2> StaticVector<float32, 2>::operator/(const float32 &s) const {
   StaticVector<float32, 2> result;
   result.v[0] = v[0] / s;
   result.v[1] = v[1] / s;
   return result;
}

template<>
inline StaticVector<float32, 4> StaticVector<float32, 4>::componentProduct(const StaticVector<float32, 4> &op) const {
   StaticVector<float32, 4> result;
   result.v[0] = v[0] * op.v[0];
   result.v[1] = v[1] * op.v[1];
   result.v[2] = v[2] * op.v[2];
   result.v[3] = v[3] * op.v[3];
   return result;
}
template<>
inline StaticVector<float32, 3> StaticVector<float32, 3>::componentProduct(const StaticVector<float32, 3> &op) const {
   StaticVector<float32, 3> result;
   result.v[0] = v[0] * op.v[0];
   result.v[1] = v[1] * op.v[1];
   result.v[2] = v[2] * op.v[2];
   return result;
}
template<>
inline StaticVector<float32, 2> StaticVector<float32, 2>::componentProduct(const StaticVector<float32, 2> &op) const {
   StaticVector<float32, 2> result;
   result.v[0] = v[0] * op.v[0];
   result.v[1] = v[1] * op.v[1];
   return result;
}

template<>
inline StaticVector<float32, 4> StaticVector<float32, 4>::operator+=(const StaticVector<float32, 4> &op) {
   v[0] += op.v[0];
   v[1] += op.v[1];
   v[2] += op.v[2];
   v[3] += op.v[3];
   return *this;
}
template<>
inline StaticVector<float32, 3> StaticVector<float32, 3>::operator+=(const StaticVector<float32, 3> &op) {
   v[0] += op.v[0];
   v[1] += op.v[1];
   v[2] += op.v[2];
   return *this;
}
template<>
inline StaticVector<float32, 2> StaticVector<float32, 2>::operator+=(const StaticVector<float32, 2> &op) {
   v[0] += op.v[0];
   v[1] += op.v[1];
   return *this;
}

template<>
inline StaticVector<float32, 4> StaticVector<float32, 4>::operator-=(const StaticVector<float32, 4> &op) {
   v[0] -= op.v[0];
   v[1] -= op.v[1];
   v[2] -= op.v[2];
   v[3] -= op.v[3];
   return *this;
}
template<>
inline StaticVector<float32, 3> StaticVector<float32, 3>::operator-=(const StaticVector<float32, 3> &op) {
   v[0] -= op.v[0];
   v[1] -= op.v[1];
   v[2] -= op.v[2];
   return *this;
}
template<>
inline StaticVector<float32, 2> StaticVector<float32, 2>::operator-=(const StaticVector<float32, 2> &op) {
   v[0] -= op.v[0];
   v[1] -= op.v[1];
   return *this;
}

template<>
inline StaticVector<float32, 4> StaticVector<float32, 4>::operator*=(const float32 &s) {
   v[0] *= s;
   v[1] *= s;
   v[2] *= s;
   v[3] *= s;
   return *this;
}
template<>
inline StaticVector<float32, 3> StaticVector<float32, 3>::operator*=(const float32 &s) {
   v[0] *= s;
   v[1] *= s;
   v[2] *= s;
   return *this;
}
template<>
inline StaticVector<float32, 2> StaticVector<float32, 2>::operator*=(const float32 &s) {
   v[0] *= s;
   v[1] *= s;
   return *this;
}

template<>
inline StaticVector<float32, 4> StaticVector<float32, 4>::operator/=(const float32 &s) {
   v[0] /= s;
   v[1] /= s;
   v[2] /= s;
   v[3] /= s;
   return *this;
}
template<>
inline StaticVector<float32, 3> StaticVector<float32, 3>::operator/=(const float32 &s) {
   v[0] /= s;
   v[1] /= s;
   v[2] /= s;
   return *this;
}
template<>
inline StaticVector<float32, 2> StaticVector<float32, 2>::operator/=(const float32 &s) {
   v[0] /= s;
   v[1] /= s;
   return *this;
}

template<>
inline bool StaticVector<float32, 4>::operator==(const StaticVector<float32, 4> op) const {
   if (v[0] != op.v[0]) return false;
   if (v[1] != op.v[1]) return false;
   if (v[2] != op.v[2]) return false;
   if (v[3] != op.v[3]) return false;
   return true;
}
template<>
inline bool StaticVector<float32, 3>::operator==(const StaticVector<float32, 3> op) const {
   if (v[0] != op.v[0]) return false;
   if (v[1] != op.v[1]) return false;
   if (v[2] != op.v[2]) return false;
   return true;
}
template<>
inline bool StaticVector<float32, 2>::operator==(const StaticVector<float32, 2> op) const {
   if (v[0] != op.v[0]) return false;
   if (v[1] != op.v[1]) return false;
   return true;
}

template<>
inline bool StaticVector<float32, 4>::operator!=(const StaticVector<float32, 4> op) const {
   if (v[0] != op.v[0]) return true;
   if (v[1] != op.v[1]) return true;
   if (v[2] != op.v[2]) return true;
   if (v[3] != op.v[3]) return true;
   return false;
}
template<>
inline bool StaticVector<float32, 3>::operator!=(const StaticVector<float32, 3> op) const {
   if (v[0] != op.v[0]) return true;
   if (v[1] != op.v[1]) return true;
   if (v[2] != op.v[2]) return true;
   return false;
}
template<>
inline bool StaticVector<float32, 2>::operator!=(const StaticVector<float32, 2> op) const {
   if (v[0] != op.v[0]) return true;
   if (v[1] != op.v[1]) return true;
   return false;
}



template<>
inline StaticMatrix<float, 2, 2>::StaticMatrix() {
   theColumns[0][0] = 1.0f; theColumns[0][1] = 0.0f;
   theColumns[1][0] = 0.0f; theColumns[1][1] = 1.0f;
}

template<>
inline StaticMatrix<float, 3, 3>::StaticMatrix() {
   theColumns[0][0] = 1.0f; theColumns[0][1] = 0.0f; theColumns[0][2] = 0.0f;
   theColumns[1][0] = 0.0f; theColumns[1][1] = 1.0f; theColumns[1][2] = 0.0f;
   theColumns[2][0] = 0.0f; theColumns[2][1] = 0.0f; theColumns[2][2] = 1.0f;
}

template<>
inline StaticMatrix<float, 4, 4>::StaticMatrix() {
   theColumns[0][0] = 1.0f; theColumns[0][1] = 0.0f; theColumns[0][2] = 0.0f; theColumns[0][3] = 0.0f;
   theColumns[1][0] = 0.0f; theColumns[1][1] = 1.0f; theColumns[1][2] = 0.0f; theColumns[1][3] = 0.0f;
   theColumns[2][0] = 0.0f; theColumns[2][1] = 0.0f; theColumns[2][2] = 1.0f; theColumns[2][3] = 0.0f;
   theColumns[3][0] = 0.0f; theColumns[3][1] = 0.0f; theColumns[3][2] = 0.0f; theColumns[3][3] = 1.0f;
}

template<>
inline StaticMatrix<float, 2, 3>::StaticMatrix() {
   theColumns[0][0] = 0.0f; theColumns[0][1] = 0.0f;
   theColumns[1][0] = 0.0f; theColumns[1][1] = 0.0f;
   theColumns[2][0] = 0.0f; theColumns[2][1] = 0.0f;
}

template<>
inline StaticMatrix<float32, 2, 2> StaticMatrix<float32, 2, 2>::operator+(const StaticMatrix<float32, 2, 2> &op) const {
   StaticMatrix<float32, 2, 2> result;
   result.theColumns[0][0] = theColumns[0][0] + op.theColumns[0][0];
   result.theColumns[0][1] = theColumns[0][1] + op.theColumns[0][1];
   result.theColumns[1][0] = theColumns[1][0] + op.theColumns[1][0];
   result.theColumns[1][1] = theColumns[1][1] + op.theColumns[1][1];
   return result;
}

template<>
inline StaticMatrix<float32, 2, 2> StaticMatrix<float32, 2, 2>::operator*(const float32 &s) const {
   StaticMatrix<float32, 2, 2> result;
   result.theColumns[0][0] = theColumns[0][0] * s;
   result.theColumns[0][1] = theColumns[0][1] * s;
   result.theColumns[1][0] = theColumns[1][0] * s;
   result.theColumns[1][1] = theColumns[1][1] * s;
   return result;
}

template<>
inline StaticVector<float32, 4> StaticMatrix<float32, 4, 4>::operator*(const StaticVector<float32, 4> &v) const {
   StaticVector<float32, 4> result;
   result[0] = theColumns[0][0]*v[0] + theColumns[1][0]*v[1] + theColumns[2][0]*v[2] + theColumns[3][0]*v[3];
   result[1] = theColumns[0][1]*v[0] + theColumns[1][1]*v[1] + theColumns[2][1]*v[2] + theColumns[3][1]*v[3];
   result[2] = theColumns[0][2]*v[0] + theColumns[1][2]*v[1] + theColumns[2][2]*v[2] + theColumns[3][2]*v[3];
   result[3] = theColumns[0][3]*v[0] + theColumns[1][3]*v[1] + theColumns[2][3]*v[2] + theColumns[3][3]*v[3];
   return result;
}
template<>
inline StaticVector<float32, 3> StaticMatrix<float32, 3, 3>::operator*(const StaticVector<float32, 3> &v) const {
   StaticVector<float32, 3> result;
   result[0] = theColumns[0][0]*v[0] + theColumns[1][0]*v[1] + theColumns[2][0]*v[2];
   result[1] = theColumns[0][1]*v[0] + theColumns[1][1]*v[1] + theColumns[2][1]*v[2];
   result[2] = theColumns[0][2]*v[0] + theColumns[1][2]*v[1] + theColumns[2][2]*v[2];
   return result;
}
template<>
inline StaticVector<float32, 2> StaticMatrix<float32, 2, 2>::operator*(const StaticVector<float32, 2> &v) const {
   StaticVector<float32, 2> result;
   result[0] = theColumns[0][0]*v[0] + theColumns[1][0]*v[1];
   result[1] = theColumns[0][1]*v[0] + theColumns[1][1]*v[1];
   return result;
}
template<>
inline StaticVector<float32, 2> StaticMatrix<float32, 3, 2>::operator*(const StaticVector<float32, 3> &v) const {
   StaticVector<float32, 2> result;
   result[0] = theColumns[0][0]*v[0] + theColumns[1][0]*v[1] + theColumns[2][0]*v[2];
   result[1] = theColumns[0][1]*v[0] + theColumns[1][1]*v[1] + theColumns[2][1]*v[2];
   return result;
}

template<>
inline float normQuad(StaticVector<float, 3> v) {
   return v[0]*v[0]+v[1]*v[1]+v[2]*v[2];
};

template<>
inline float normQuad(StaticVector<float, 2> v) {
   return v[0]*v[0]+v[1]*v[1];
};


// --- partial specialization (efficiency)


template <class FloatTypeDest, class FloatTypeSource>
StaticVector<FloatTypeDest, 2> convertVector(const StaticVector<FloatTypeSource, 2> &source) {
   result[0] = (FloatTypeDest)source[0];
   result[1] = (FloatTypeDest)source[1];
   return result;
}

template <class FloatTypeDest, class FloatTypeSource>
StaticVector<FloatTypeDest, 3> convertVector(const StaticVector<FloatTypeSource, 3> &source) {
   result[0] = (FloatTypeDest)source[0];
   result[1] = (FloatTypeDest)source[1];
   result[2] = (FloatTypeDest)source[2];
   return result;
}

template <class FloatTypeDest, class FloatTypeSource>
StaticVector<FloatTypeDest, 4> convertVector(const StaticVector<FloatTypeSource, 4> &source) {
   result[0] = (FloatTypeDest)source[0];
   result[1] = (FloatTypeDest)source[1];
   result[2] = (FloatTypeDest)source[2];
   result[3] = (FloatTypeDest)source[3];
   return result;
}

template<>
inline float norm(StaticVector<float, 3> v) {
   return sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
};

template<>
inline float norm(StaticVector<float, 2> v) {
   return sqrt(v[0]*v[0]+v[1]*v[1]);
};

template<>
inline StaticVector<float32,1> makeVector<float32,1>( const float32 * data )
{
   StaticVector<float32,1> ret;
   ret[0] = data[0];
   return ret;
}


template<>
inline StaticVector<float32,2> makeVector<float32,2>( const float32 * data )
{
   StaticVector<float32,2> ret;
   ret[0] = data[0];
   ret[1] = data[1];
   return ret;
}

template<>
inline StaticVector<float32,3> makeVector<float32,3>( const float32 * data )
{
   StaticVector<float32,3> ret;
   ret[0] = data[0];
   ret[1] = data[1];
   ret[2] = data[2];
   return ret;
}

template<>
inline StaticVector<float32,4> makeVector<float32,4>( const float32 * data )
{
   StaticVector<float32,4> ret;
   ret[0] = data[0];
   ret[1] = data[1];
   ret[2] = data[2];
   ret[3] = data[3];
   return ret;
}


#endif  // end of explicit loop unrolling




// ----  helper routines


inline Vector2i makeVector2i(int x, int y) {
   Vector2i v;
   v[0] = x; v[1] = y;
   return v;
}

inline Vector2ui makeVector2ui(unsigned int x, unsigned int y) {
   Vector2ui v;
   v[0] = x; v[1] = y;
   return v;
}

inline Vector2f makeVector2f(float x, float y) {
   Vector2f v;
   v[0] = x; v[1] = y;
   return v;
}

inline Vector2d makeVector2d(double x, double y) {
   Vector2d v;
   v[0] = x; v[1] = y;
   return v;
}

inline Vector3f makeVector3f(float x, float y, float z) {
   Vector3f v;
   v[0] = x; v[1] = y; v[2] = z;
   return v;
}

inline Vector3d makeVector3d(double x, double y, double z) {
   Vector3d v;
   v[0] = x; v[1] = y; v[2] = z;
   return v;
}


inline Vector3i makeVector3i(int x, int y, int z) {
   Vector3i v;
   v[0] = x; v[1] = y; v[2] = z;
   return v;
}

inline Vector4f makeVector4f(float x, float y, float z, float w) {
   Vector4f v;
   v[0] = x; v[1] = y; v[2] = z; v[3] = w;
   return v;
}

inline Vector6f makeVector6f(float x, float y, float z, float u, float v, float w) {
   Vector6f vec;
   vec[0] = x; vec[1] = y; vec[2] = z; vec[3] = u; vec[4] = v; vec[5] = w;
   return vec;
}

inline Vector4i makeVector4i(int x, int y, int z, int w) {
   Vector4i v;
   v[0] = x; v[1] = y; v[2] = z; v[3] = w;
   return v;
}

inline Vector4d makeVector4d(double x, double y, double z, double w) {
   Vector4d v;
   v[0] = x; v[1] = y; v[2] = z; v[3] = w;
   return v;
}

inline Vector4ub makeVector4ub(card8 x, card8 y, card8 z, card8 w) {
   Vector4ub v;
   v[0] = x; v[1] = y; v[2] = z; v[3] = w;
   return v;
}

inline Vector3ub makeVector3ub(card8 x, card8 y, card8 z) {
   Vector3ub v;
   v[0] = x; v[1] = y; v[2] = z;
   return v;
}

inline Vector2ub makeVector2ub(card8 x, card8 y) {
   Vector2ub v;
   v[0] = x; v[1] = y;
   return v;
}

template<class FloatType,unsigned dim>
inline StaticVector<FloatType,dim> makeVector( const FloatType *data )
{
   StaticVector<FloatType,dim> ret;
   for( card32 i=0;i<dim;i++ )
      ret[i] = data[i];

   return ret;
}

inline Vector2f makeVector2fv( const float32 * data )
{
   return makeVector<float32,2>( data );
}

inline Vector3f makeVector3fv( const float32 * data )
{
   return makeVector<float32,3>( data );
}

inline Vector4f makeVector4fv( const float32 * data )
{
   return makeVector<float32,4>( data );
}


inline Matrix4f makeMatrix4f(float m00, float m10, float m20, float m30,
                             float m01, float m11, float m21, float m31,
                             float m02, float m12, float m22, float m32,
                             float m03, float m13, float m23, float m33) {
   Matrix4f m;
   m[0][0] = m00;  m[1][0] = m10;  m[2][0] = m20;  m[3][0] = m30;
   m[0][1] = m01;  m[1][1] = m11;  m[2][1] = m21;  m[3][1] = m31;
   m[0][2] = m02;  m[1][2] = m12;  m[2][2] = m22;  m[3][2] = m32;
   m[0][3] = m03;  m[1][3] = m13;  m[2][3] = m23;  m[3][3] = m33;
   return m;
}

inline Matrix6f makeMatrix6f(float m00, float m10, float m20, float m30, float m40, float m50,
                             float m01, float m11, float m21, float m31, float m41, float m51,
                             float m02, float m12, float m22, float m32, float m42, float m52,
                             float m03, float m13, float m23, float m33, float m43, float m53,
               float m04, float m14, float m24, float m34, float m44, float m54,
               float m05, float m15, float m25, float m35, float m45, float m55) {
   Matrix6f m;
   m[0][0] = m00;  m[1][0] = m10;  m[2][0] = m20;  m[3][0] = m30;  m[4][0] = m40;  m[5][0] = m50;
   m[0][1] = m01;  m[1][1] = m11;  m[2][1] = m21;  m[3][1] = m31;  m[4][1] = m41;  m[5][1] = m51;
   m[0][2] = m02;  m[1][2] = m12;  m[2][2] = m22;  m[3][2] = m32;  m[4][2] = m42;  m[5][2] = m52;
   m[0][3] = m03;  m[1][3] = m13;  m[2][3] = m23;  m[3][3] = m33;  m[4][3] = m43;  m[5][3] = m53;
   m[0][4] = m04;  m[1][4] = m14;  m[2][4] = m24;  m[3][4] = m34;  m[4][4] = m44;  m[5][4] = m54;
   m[0][5] = m05;  m[1][5] = m15;  m[2][5] = m25;  m[3][5] = m35;  m[4][5] = m45;  m[5][5] = m55;
   return m;
}

inline Matrix3f makeMatrix3f(float m00, float m10, float m20,
                             float m01, float m11, float m21,
                             float m02, float m12, float m22) {
   Matrix3f m;
   m[0][0] = m00;  m[1][0] = m10;  m[2][0] = m20;
   m[0][1] = m01;  m[1][1] = m11;  m[2][1] = m21;
   m[0][2] = m02;  m[1][2] = m12;  m[2][2] = m22;
   return m;
}

inline Matrix3d makeMatrix3d(double m00, double m10, double m20,
               double m01, double m11, double m21,
               double m02, double m12, double m22) {
                 Matrix3d m;
                 m[0][0] = m00;  m[1][0] = m10;  m[2][0] = m20;
                 m[0][1] = m01;  m[1][1] = m11;  m[2][1] = m21;
                 m[0][2] = m02;  m[1][2] = m12;  m[2][2] = m22;
                 return m;
}

inline Matrix2f makeMatrix2f(float m00, float m01,
                             float m10, float m11) {
   Matrix2f m;
   m[0][0] = m00;  m[1][0] = m10;
   m[0][1] = m01;  m[1][1] = m11;
   return m;
}

inline Vector4f expand3To4(Vector3f v) {
   return makeVector4f(v[0], v[1], v[2], 1.0f);
}

inline Matrix4f expand3To4(Matrix3f v) {
   Matrix4f result;
   for (int x = 0; x<3; x++) {
      for (int y = 0; y<3; y++) {
         result[x][y] = v[x][y];
      }
   }
   for (int i = 0; i<3; i++) {
      result[3][i] = 0.0;
   }
   for (int i = 0; i<3; i++) {
      result[i][3] = 0.0;
   }
   result[3][3] = 1.0;
   return result;
}

inline Vector3f shrink4To3(Vector4f v) {
   return makeVector3f(v[0], v[1], v[2]);
}
inline Matrix3f shrink4To3(Matrix4f v) {
   Matrix3f result;
   for (int x = 0; x<3; x++) {
      for (int y = 0; y<3; y++) {
         result[x][y] = v[x][y];
      }
   }
   return result;
}

inline Vector3f projectHomogeneous4To3(Vector4f v) {
   return makeVector3f(v[0]/v[3], v[1]/v[3], v[2]/v[3]);
}

template <class FloatType, unsigned dim>
inline StaticMatrix<FloatType,dim,dim> outerProduct(const StaticVector<FloatType,dim> &v1,const StaticVector<FloatType,dim> &v2)
{
  StaticMatrix<FloatType,dim,dim> ret;
  for( int i=0;i<dim;i++)
  {
    for( int j=0;j<dim;j++ )
    {
      ret[i][j] = v1[i]*v2[j];
    }
  }
  return ret;
}


template <class FloatType, unsigned dim>
inline FloatType normQuad(StaticVector<FloatType, dim> v) {
   FloatType result = 0;
   for (unsigned i=0; i<dim; i++) {
      result+=v[i]*v[i];
   }
   return result;
};

template <class FloatType, unsigned dim>
inline FloatType norm(StaticVector<FloatType, dim> v) {
   return sqrt(normQuad(v));
};

template <class FloatType, unsigned dim>
inline StaticVector<FloatType, dim> normalize(const StaticVector<FloatType, dim> &v) {
   return v / norm(v);
}




inline Matrix3f makeRotX3f(Radian angle) {
   return makeMatrix3f (
              1.0,        0.0,        0.0,

              0.0, cos(angle), sin(angle),

              0.0,-sin(angle), cos(angle)
   );
}
inline Matrix4f makeRotX4f(Radian angle) {
   return expand3To4(makeRotX3f(angle));
}

inline Matrix3f makeRotY3f(Radian angle) {
   return makeMatrix3f (
       cos(angle),        0.0, sin(angle),

              0.0,        1.0,        0.0,

      -sin(angle),        0.0, cos(angle)
   );
}
inline Matrix4f makeRotY4f(Radian angle) {
   return expand3To4(makeRotY3f(angle));
}

inline Matrix3f makeRotZ3f(Radian angle) {
   return makeMatrix3f (
       cos(angle), sin(angle),        0.0,

      -sin(angle), cos(angle),        0.0,

              0.0,        0.0,        1.0
   );
}
inline Matrix4f makeRotZ4f(Radian angle) {
   return expand3To4(makeRotZ3f(angle));
}

inline Matrix3f makeRotVector3f(Vector3f vec, float32 angle) {
   float32 cosA    = cos(angle),
           sinA    = sin(angle),
           invCosA = 1.0f - cosA;
   return makeMatrix3f( invCosA*vec[0]*vec[0] + cosA,
                        invCosA*vec[0]*vec[1] + sinA*vec[2],
                        invCosA*vec[0]*vec[2] - sinA*vec[1],

                        invCosA*vec[0]*vec[1] - sinA*vec[2],
                        invCosA*vec[1]*vec[1] + cosA,
                        invCosA*vec[1]*vec[2] + sinA*vec[0],

                        invCosA*vec[0]*vec[2] + sinA*vec[1],
                        invCosA*vec[1]*vec[2] - sinA*vec[0],
                        invCosA*vec[2]*vec[2] + cosA).transpose();
}

inline Matrix4f makeRotVector4f(Vector3f vec, float32 angle) {
   return expand3To4(makeRotVector3f(vec, angle));
}

inline Matrix4f makeTranslation4f(const Vector3f &t) {
   return makeMatrix4f (
      1.0,  0.0,  0.0, t[0],
      0.0,  1.0,  0.0, t[1],
      0.0,  0.0,  1.0, t[2],
      0.0,  0.0,  0.0,  1.0
   );
}

inline Matrix4f makeTranslation4f(float32 x, float32 y, float32 z) {
   return makeMatrix4f (
      1.0,  0.0,  0.0,   x,
      0.0,  1.0,  0.0,   y,
      0.0,  0.0,  1.0,   z,
      0.0,  0.0,  0.0,  1.0
   );
}

inline Matrix3f makeScale3f(const Vector3f &s) {
   return makeMatrix3f (
     s[0],  0.0,  0.0,
      0.0, s[1],  0.0,
      0.0,  0.0, s[2]
   );
}
inline Matrix4f makeScale4f(const Vector3f &s) {
   return expand3To4(makeScale3f(s));
}
inline Matrix3f makeScale3f(float32 x, float32 y, float32 z) {
   return makeScale3f(makeVector3f(x,y,z));
}
inline Matrix4f makeScale4f(float32 x, float32 y, float32 z) {
   return makeScale4f(makeVector3f(x,y,z));
}

inline Vector3f transformVector3f(const Matrix4f &M, const Vector3f &v) {
   return shrink4To3(M*(expand3To4(v)));
}

inline Vector3f expand2To3(Vector2f v) {
  return makeVector3f(v[0], v[1],  1.0f);
}

inline Vector2f projectHomogeneous3To2(Vector3f v) {
  return makeVector2f(v[0]/v[2], v[1]/v[2]);
}

Vector2f projectiveTransformVector2f(const Matrix3f &M, const Vector2f &v )
{
 return projectHomogeneous3To2(M*(expand2To3(v)));
}

inline Vector3f projectiveTransformVector3f(const Matrix4f &M, const Vector3f &v) {
   return projectHomogeneous4To3(M*(expand3To4(v)));
}

inline Matrix3f makeNewCoordPrjMatrix3f(Vector3f v1, Vector3f v2, Vector3f v3) {
   Matrix3f result;
   result[0] = v1;
   result[1] = v2;
   result[2] = v3;
   return result;
}


template<class FloatType, unsigned size>
inline unsigned searchPivot(const StaticMatrix<FloatType, size, size> &mat, unsigned step, bool *success, FloatType pivotEps) {
   unsigned result = step;
   FloatType max = fabs(mat[step][step]);
   for (unsigned p = step+1; p<size; p++) {
      if (fabs(mat[step][p]) > max) {
         max = fabs(mat[step][p]);
         result = p;
      }
   }
   if (max <= pivotEps) {
    if (success != NULL) {
      *success = false;
      return 0;
    } else {
        throw PException("searchPivot: no pivot element found (matrix singular?).");
     }
   } else {
    if (success != NULL) {
      *success = true;
    }
      return result;
   }
};

template<class FloatType, unsigned size>
inline StaticMatrix<FloatType, size, size> invertMatrix(const StaticMatrix<FloatType, size, size> &mat, bool *success, FloatType pivotEps) {
   StaticMatrix<FloatType, size, size> result;
   StaticMatrix<FloatType, size, size> mCopy = mat;
   for (unsigned step=0; step<size; step++) {
      unsigned pivot = searchPivot(mCopy, step, success, pivotEps);
      if (success != NULL) {
      if (!(*success)) {
        return result;
      }
      }
      if (step != pivot) {
         mCopy.changeRows(step, pivot);
         result.changeRows(step, pivot);
      }
      if (mCopy[step][step] == 0) throw PException("invertMatrix: Matrix is singular.");
      FloatType m = (FloatType)1.0/mCopy[step][step];
      mCopy.multRow(step, m);
      result.multRow(step, m);
      for (unsigned row=step+1; row<size; row++) {
         FloatType m = -mCopy[step][row];
         mCopy.combineRows(row, step, m);
         result.combineRows(row, step, m);
      }
   }
   for (int step=size-1; step>=0; step--) {
      for (int row=step-1; row>=0; row--) {
         FloatType m = -mCopy[step][row];
         //mCopy.combineRows(row, step, m);
         result.combineRows(row, step, m);
      }
   }
   if (success != NULL) {
    *success = true;
   }
   return result;
};


Matrix4f invertFrame( const Matrix4f &m )
{
  return expand3To4( shrink4To3(m).transpose()) * makeTranslation4f( -shrink4To3(m[3]));
}



#endif
