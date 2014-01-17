//---------------------------------------------------------------------------
#ifndef DynamicLinearAlgebra_templCodeH
#define DynamicLinearAlgebra_templCodeH
//---------------------------------------------------------------------------
#include "DynamicLinearAlgebra.h"
//---------------------------------------------------------------------------
#ifdef _DEBUG
 #define DVECT_RANGE_CHECK
#endif
//---------------------------------------------------------------------------
#include "LinearAlgebra.hpp"
#include "ObjectStreams.h"
//---------------------------------------------------------------------------

using namespace std;


// ---------------------------------------------------------------------------
//
//                                 DynamicVector
//
// ---------------------------------------------------------------------------


template <typename FloatType>
inline DynamicVector<FloatType>::DynamicVector(card32 dim) {
   this->dim = dim;
   v = new FloatType[dim];
}

template <typename FloatType>
inline card32 DynamicVector<FloatType>::getDim() const {
   return dim;
}

template <typename FloatType>
inline card32 DynamicVector<FloatType>::size() const {
   return dim;
}

template <typename FloatType>
inline void DynamicVector<FloatType>::setDim(const card32 dim) {
   this->dim = dim;
   delete[] v;
   v = new FloatType[dim];
}

template <typename FloatType>
inline void DynamicVector<FloatType>::setZero() {
   for (card32 i=0; i<dim; i++) {
      v[i] = (FloatType)0;
   }
}

template <typename FloatType>
inline DynamicVector<FloatType>::DynamicVector(const DynamicVector<FloatType> &init) {
   this->dim = init.dim;
   v = new FloatType[dim];
   for (unsigned i=0; i<dim; i++) {
      v[i] = init.v[i];
   }
}

template <typename FloatType>
inline FloatType& DynamicVector<FloatType>::operator[](const unsigned &index) {
   #ifdef DVECT_RANGE_CHECK
   if (index >= dim) {
      throw ERangeCheck("DynamicVector::operator[] - index exceeds bounds");
   }
   #endif
   return v[index];
}

template <typename FloatType>
inline const FloatType& DynamicVector<FloatType>::operator[](const unsigned &index) const{
   #ifdef DVECT_RANGE_CHECK
   if (index >= dim) {
      throw ERangeCheck("DynamicVector::operator[] - index exceeds bounds");
   }
   #endif
   return v[index];
}

template <typename FloatType>
inline DynamicVector<FloatType> DynamicVector<FloatType>::operator+(const DynamicVector<FloatType> &op) const {
   #ifdef DVECT_RANGE_CHECK
   if (dim != op.dim) {
      throw ERangeCheck("DynamicVector::operator+ - incompatible dimensions.");
   }
   #endif
   DynamicVector<FloatType> result(dim);
   for (unsigned i=0; i<dim; i++) {
      result[i] = v[i] + op.v[i];
   }
   return result;
}

template <typename FloatType>
inline DynamicVector<FloatType> DynamicVector<FloatType>::operator-(const DynamicVector<FloatType> &op) const {
   #ifdef DVECT_RANGE_CHECK
   if (dim != op.dim) {
      throw ERangeCheck("DynamicVector::operator- - incompatible dimensions.");
   }
   #endif
   DynamicVector<FloatType> result(dim);
   for (unsigned i=0; i<dim; i++) {
      result[i] = v[i] - op.v[i];
   }
   return result;
}

template <typename FloatType>
inline DynamicVector<FloatType> DynamicVector<FloatType>::operator-() const {
   DynamicVector<FloatType> result(dim);
   for (unsigned i=0; i<dim; i++) {
      result[i] = -v[i];
   }
   return result;
}

template <typename FloatType>
inline FloatType DynamicVector<FloatType>::operator*(const DynamicVector<FloatType> op) const {
   #ifdef DVECT_RANGE_CHECK
   if (dim != op.dim) {
      throw ERangeCheck("DynamicVector::operator* - incompatible dimensions.");
   }
   #endif
   FloatType result = 0.0;
   for (unsigned i=0; i<dim; i++) {
      result += v[i] * op.v[i];
   }
   return result;
}

template <typename FloatType>
inline DynamicVector<FloatType> DynamicVector<FloatType>::operator*(const FloatType &s) const {
   DynamicVector<FloatType> result(dim);
   for (unsigned i=0; i<dim; i++) {
      result[i] = s * v[i];
   }
   return result;
}

template <typename FloatType>
inline DynamicVector<FloatType> DynamicVector<FloatType>::operator/(const FloatType &s) const {
   DynamicVector<FloatType> result(dim);
   for (unsigned i=0; i<dim; i++) {
      result[i] = v[i] / s;
   }
   return result;
}

template <typename FloatType>
inline DynamicVector<FloatType> DynamicVector<FloatType>::componentProduct(const DynamicVector<FloatType> &op) const {
   #ifdef DVECT_RANGE_CHECK
   if (dim != op.dim) {
      throw ERangeCheck("DynamicVector::componentProduct - incompatible dimensions.");
   }
   #endif
   DynamicVector<FloatType> result(dim);
   for (unsigned i=0; i<dim; i++) {
      result[i] = v[i] * op.v[i];
   }
   return result;
}

template <typename FloatType>
inline DynamicVector<FloatType> DynamicVector<FloatType>::componentAddScalar(const FloatType &s) const {
   DynamicVector<FloatType> result(dim);
   for (unsigned i=0; i<dim; i++) {
      result[i] = v[i] + s;
   }
   return result;
}

template <typename FloatType>
inline DynamicVector<FloatType> DynamicVector<FloatType>::componentSubstractScalar(const FloatType &s) const {
   DynamicVector<FloatType> result(dim);
   for (unsigned i=0; i<dim; i++) {
      result[i] = v[i] - s;
   }
   return result;
}

template <typename FloatType>
inline FloatType DynamicVector<FloatType>::componentSum() const {
   FloatType result = 0;
   for (unsigned i=0; i<dim; i++) {
      result += v[i];
   }
   return result;
}

template <typename FloatType>
inline FloatType DynamicVector<FloatType>::average() const {
   pAssert(dim > 0);
   return componentSum() / dim;
}

template <typename FloatType>
inline DynamicVector<FloatType> DynamicVector<FloatType>::operator+=(const DynamicVector<FloatType> &op) {
   #ifdef DVECT_RANGE_CHECK
   if (dim != op.dim) {
      throw ERangeCheck("DynamicVector::operator+= - incompatible dimensions.");
   }
   #endif
   for (unsigned i=0; i<dim; i++) {
      v[i] += op.v[i];
   }
   return *this;
}

template <typename FloatType>
inline DynamicVector<FloatType> DynamicVector<FloatType>::operator-=(const DynamicVector<FloatType> &op) {
   #ifdef DVECT_RANGE_CHECK
   if (dim != op.dim) {
      throw ERangeCheck("DynamicVector::operator-= - incompatible dimensions.");
   }
   #endif
   for (unsigned i=0; i<dim; i++) {
      v[i] -= op.v[i];
   }
   return *this;
}

template <typename FloatType>
inline DynamicVector<FloatType> DynamicVector<FloatType>::operator*=(const FloatType &s) {
   for (unsigned i=0; i<dim; i++) {
      v[i] *= s;
   }
   return *this;
}

template <typename FloatType>
inline DynamicVector<FloatType> DynamicVector<FloatType>::operator/=(const FloatType &s) {
   for (unsigned i=0; i<dim; i++) {
      v[i] /= s;
   }
   return *this;
}

template <typename FloatType>
inline void DynamicVector<FloatType>::operator=(const DynamicVector<FloatType> &op) {
   if (dim != op.dim) {
      delete[] v;
      v = new FloatType[op.dim];
   }
   this->dim = op.dim;
   for (unsigned i=0; i<op.dim; i++) {
      v[i] = op.v[i];
   }
}

template <typename FloatType>
inline bool DynamicVector<FloatType>::operator==(const DynamicVector<FloatType> op) const {
   if (dim != op.dim) {
      throw ERangeCheck("DynamicVector::operator== - incompatible dimensions.");
   }
   for (unsigned i=0; i<dim; i++) {
      if (v[i] != op.v[i]) return false;
   }
   return true;
}

template <typename FloatType>
inline bool DynamicVector<FloatType>::operator!=(const DynamicVector<FloatType> op) const {
   #ifdef DVECT_RANGE_CHECK
   if (dim != op.dim) {
      throw ERangeCheck("DynamicVector::operator!= - incompatible dimensions.");
   }
   #endif
   for (unsigned i=0; i<dim; i++) {
      if (v[i] != op.v[i]) return true;
   }
   return false;
}

template <typename FloatType>
inline FloatType DynamicVector<FloatType>::maxComponent() const {
   pAssert(dim >= 1);
   FloatType result = v[0];
   for (card32 i=1; i<dim; i++) {
      if (v[i] > result) {
         result = v[i];
      }
   }
   return result;
}

template <typename FloatType>
inline FloatType DynamicVector<FloatType>::minComponent() const {
   pAssert(dim >= 1);
   FloatType result = v[0];
   for (card32 i=1; i<dim; i++) {
      if (v[i] < result) {
         result = v[i];
      }
   }
   return result;
}

template <typename FloatType>
inline DynamicVector<FloatType> DynamicVector<FloatType>::absoluteValues() const {
   DynamicVector<FloatType> result(dim);
   for (card32 i=0; i<dim; i++) {
      result[i] = fabs(v[i]);
   }
   return result;
}

template <typename FloatType>
inline bool DynamicVector<FloatType>::allComponentsLargerEqual(const DynamicVector<FloatType> &op) const {
   pAssert(dim == op.dim);
   for (card32 i=0; i<dim; i++) {
      if (v[i] < op.v[i]) {
         return false;
      }
   }
   return true;
}

template <typename FloatType>
inline bool DynamicVector<FloatType>::allComponentsLarger(const DynamicVector<FloatType> &op) const {
   pAssert(dim == op.dim);
   for (card32 i=0; i<dim; i++) {
      if (v[i] <= op.v[i]) {
         return false;
      }
   }
   return true;
}

template <typename FloatType>
inline bool DynamicVector<FloatType>::allComponentsSmallerEqual(const DynamicVector<FloatType> &op) const {
   pAssert(dim == op.dim);
   for (card32 i=0; i<dim; i++) {
      if (v[i] > op.v[i]) {
         return false;
      }
   }
   return true;
}

template <typename FloatType>
inline bool DynamicVector<FloatType>::allComponentsSmaller(const DynamicVector<FloatType> &op) const {
   pAssert(dim == op.dim);
   for (card32 i=0; i<dim; i++) {
      if (v[i] >= op.v[i]) {
         return false;
      }
   }
   return true;
}

template <typename FloatType>
inline float32 DynamicVector<FloatType>::getNorm() const {
//	pAssert(dim == op.dim);
	float32 result = 0;
	for (card32 i=0; i<dim; i++) {
		result +=  v[i] * v[i];
	}
	return sqrt( result );
}

template <typename FloatType>
void DynamicVector<FloatType>::print() {
   cout << '(';
   for (unsigned i=0; i<dim-1; i++) {
      cout << v[i];
      cout << ',';
      cout << ' ';
   }
   cout << v[dim-1];
   cout << ')';
}

template <typename FloatType>
string DynamicVector<FloatType>::toString() const {
   string result;
   result += '(';
   for (unsigned i=0; i<dim-1; i++) {
      char buffer[1000];
      sprintf(buffer, "%g", v[i]);
      result += buffer;
      result += ',';
      result += ' ';
   }
   char buffer[1000];
   sprintf(buffer, "%g", v[dim-1]);
   result += buffer;
   result += ')';
   return result;
}

template <typename FloatType>
inline FloatType* DynamicVector<FloatType>::data() {
   return &(v[0]);
}

template <typename FloatType>
inline const FloatType* DynamicVector<FloatType>::data() const {
   return &(v[0]);
}

template <typename FloatType>
inline FloatType* DynamicVector<FloatType>::dataPtr() {
   return &(v[0]);
}

template <typename FloatType>
inline const FloatType* DynamicVector<FloatType>::dataPtr() const {
   return &(v[0]);
}


template <typename FloatType>
inline DynamicVector<FloatType> DynamicVector<FloatType>::copyElements(const card32 &howMany) const {
   #ifdef DVECT_RANGE_CHECK
   if (howMany > dim) {
      throw ERangeCheck("DynamicVector::copyElements - incompatible dimensions.");
   }
   #endif
   DynamicVector<FloatType> result(howMany);
   for (unsigned i=0; i<howMany; i++) {
      result[i] = v[i];
   }
   return result;
}


template <typename FloatType>
inline void DynamicVector<FloatType>::setDataPointer(FloatType* x, const card32 dim){
   delete[] v;
   this->dim = dim;
   v=x;
}

template <typename FloatType>
inline DynamicVector<FloatType>::~DynamicVector() {
   delete[] v;
}


template <typename FloatType>
void DynamicVector<FloatType>::write(OutputObjectStream *s) const {
	s->write(dim);
	s->writeBuffer(v, dim*sizeof(FloatType));
}

template <typename FloatType>
void DynamicVector<FloatType>::read(InputObjectStream *s) {
	delete[] v;
	s->read(dim);
	v = new FloatType[dim];
	s->readBuffer(v, dim*sizeof(FloatType));
}




// ---------------------------------------------------------------------------
//
//                                 DynamicMatrix
//
// ---------------------------------------------------------------------------



template <typename FloatType>
inline DynamicMatrix<FloatType>::DynamicMatrix( card32 c, card32 r, bool initAsIdentity) {
   columns = c;
   rows = r;
   theColumns = new DynamicVector<FloatType>[columns];
   for ( unsigned int n = 0; n < columns; n++ )
      theColumns[n] = DynamicVector<FloatType>(rows);
   for (unsigned j=0; j<columns; j++) {
      for (unsigned i=0; i<rows; i++) {
         if (i == j && initAsIdentity) {
            theColumns[j][i] = 1.0;
         } else {
            theColumns[j][i] = 0.0;
         }
      }
   }
}

template <typename FloatType>
inline void DynamicMatrix<FloatType>::setDimension(card32 columns, card32 rows, bool initAsIdentity) {
   delete[] theColumns;
   this->rows = rows;
   this->columns = columns;
   theColumns = new DynamicVector<FloatType>[columns];
   for ( unsigned int n = 0; n < columns; n++ )
      theColumns[n] = DynamicVector<FloatType>(rows);

   for (unsigned j=0; j<columns; j++) {
      for (unsigned i=0; i<rows; i++) {
         if (i == j && initAsIdentity) {
            theColumns[j][i] = 1.0;
         } else {
            theColumns[j][i] = 0.0;
         }
      }
   }
}

template <typename FloatType>
inline card32 DynamicMatrix<FloatType>::getRowsDim() const{
     return rows;
}

template <typename FloatType>
inline card32 DynamicMatrix<FloatType>::getColsDim() const{
     return columns;
}

template <typename FloatType>
inline void DynamicMatrix<FloatType>::setZero() {
   for (unsigned j=0; j<columns; j++)
      for (unsigned i=0; i<rows; i++)
            theColumns[j][i] = 0.0;
}

template <typename FloatType>
inline card32 DynamicMatrix<FloatType>::getRows() const {
   return rows;
}

template <typename FloatType>
inline card32 DynamicMatrix<FloatType>::getColumns() const {
   return columns;
}

template <typename FloatType>
inline DynamicMatrix<FloatType>::DynamicMatrix(const DynamicMatrix<FloatType> &init) {
   this->columns = init.columns;
   this->rows = init.rows;
   theColumns = new DynamicVector<FloatType>[columns];
   for (unsigned i=0; i<columns; i++) {
      theColumns[i] = init.theColumns[i];
   }
}

template <typename FloatType>
inline DynamicVector<FloatType>& DynamicMatrix<FloatType>::operator[](const unsigned &index) {
   #ifdef DVECT_RANGE_CHECK
   if (index >= columns) {
      throw ERangeCheck("DynamicMatrix::operator[] - index exceeds bounds");
   }
   #endif
   return theColumns[index];
}

template <typename FloatType>
inline const DynamicVector<FloatType>& DynamicMatrix<FloatType>::operator[](const unsigned &index) const {
   #ifdef DVECT_RANGE_CHECK
   if (index >= columns) {
      throw ERangeCheck("DynamicMatrix::operator[] - index exceeds bounds");
   }
   #endif
   return theColumns[index];
}

template <typename FloatType>
inline DynamicMatrix<FloatType> DynamicMatrix<FloatType>::operator+(const DynamicMatrix<FloatType> &op) const {
   #ifdef DVECT_RANGE_CHECK
   if (columns != op.columns || rows != op.rows) {
      throw ERangeCheck("DynamicMatrix::operator+ - incompatible dimensions.");
   }
   #endif
   DynamicMatrix<FloatType> result(columns, rows);
   for (unsigned i=0; i<columns; i++) {
      result[i] = theColumns[i] + op.theColumns[i];
   }
   return result;
}

template <typename FloatType>
inline DynamicMatrix<FloatType> DynamicMatrix<FloatType>::operator-(const DynamicMatrix<FloatType> &op) const {
   #ifdef DVECT_RANGE_CHECK
   if (columns != op.columns || rows != op.rows) {
      throw ERangeCheck("DynamicMatrix::operator- - incompatible dimensions.");
   }
   #endif
   DynamicMatrix<FloatType> result(columns, rows);
   for (unsigned i=0; i<columns; i++) {
      result[i] = theColumns[i] - op.theColumns[i];
   }
   return result;
}

template <typename FloatType>
inline DynamicMatrix<FloatType> DynamicMatrix<FloatType>::operator-() const {
   DynamicMatrix<FloatType> result(columns, rows);
   for (unsigned i=0; i<columns; i++) {
      result[i] = -theColumns[i];
   }
   return result;
}

template <typename FloatType>
inline DynamicMatrix<FloatType> DynamicMatrix<FloatType>::operator*(const FloatType &s) const {
   DynamicMatrix<FloatType> result(columns, rows);
   for (unsigned i=0; i<columns; i++) {
      result[i] = theColumns[i] * s;
   }
   return result;
}

template <typename FloatType>
inline DynamicMatrix<FloatType> DynamicMatrix<FloatType>::operator*(const DynamicMatrix<FloatType> &op) const {
   if ( columns != op.rows ) throw
      PException("DynamicMatrix::operator* matrix dimensions do not match.");

   DynamicMatrix<FloatType> result( op.columns, rows );
   for ( unsigned r = 0; r < rows; r++ ){
      for ( unsigned c = 0; c < op.columns; c++ ){
         result[c][r] = 0.0;
         for ( unsigned i = 0; i < columns; i++ ) {
            result[c][r] += (*this)[i][r] * op[c][i];
         }
      }
   }
   return result;
}

template <typename FloatType>
inline DynamicMatrix<FloatType> DynamicMatrix<FloatType>::operator/(const FloatType &s) const {
   DynamicMatrix<FloatType> result(columns, rows);
   for (unsigned i=0; i<columns; i++) {
      result[i] = theColumns[i] / s;
   }
   return result;
}

template <typename FloatType>
inline DynamicMatrix<FloatType> DynamicMatrix<FloatType>::operator+=(const DynamicMatrix<FloatType> &op) {
   #ifdef DVECT_RANGE_CHECK
   if (columns != op.columns || rows != op.rows) {
      throw ERangeCheck("DynamicMatrix::operator+= - incompatible dimensions.");
   }
   #endif
   for (unsigned i=0; i<columns; i++) {
      theColumns[i] += op.theColumns[i];
   }
   return *this;
}

template <typename FloatType>
inline DynamicMatrix<FloatType> DynamicMatrix<FloatType>::operator-=(const DynamicMatrix<FloatType> &op) {
   #ifdef DVECT_RANGE_CHECK
   if (columns != op.columns || rows != op.rows) {
      throw ERangeCheck("DynamicMatrix::operator-= - incompatible dimensions.");
   }
   #endif
   for (unsigned i=0; i<columns; i++) {
      theColumns[i] -= op.theColumns[i];
   }
   return *this;
}

template <typename FloatType>
inline DynamicMatrix<FloatType> DynamicMatrix<FloatType>::operator*=(const FloatType &op) {
   for (unsigned i=0; i<columns; i++) {
      theColumns[i] *= op;
   }
   return *this;
}

template <typename FloatType>
inline DynamicMatrix<FloatType> DynamicMatrix<FloatType>::operator*=(const DynamicMatrix<FloatType> &op) {
   #ifdef DVECT_RANGE_CHECK
   if (columns != op.columns || rows != op.rows) {
      throw ERangeCheck("DynamicMatrix::operator*= - incompatible dimensions.");
   }
   #endif
   DynamicMatrix<FloatType> result = (*this)*op;
   *this = result;
   return result;
}

template <typename FloatType>
inline DynamicMatrix<FloatType> DynamicMatrix<FloatType>::operator/=(const FloatType &op) {

   for (unsigned i=0; i<columns; i++) {
      theColumns[i] /= op;
   }
   return *this;
}

template <typename FloatType>
inline void DynamicMatrix<FloatType>::operator=(const DynamicMatrix<FloatType> &op) {
   if (columns != op.columns || rows != op.rows) {
      delete[] theColumns;
      theColumns = new DynamicVector<FloatType>[op.columns];
   }
   this->columns = op.columns;
   this->rows = op.rows;
   for (unsigned i=0; i<op.columns; i++) {
      theColumns[i] = op.theColumns[i];
   }
}

template <typename FloatType>
inline bool DynamicMatrix<FloatType>::operator==(const DynamicMatrix<FloatType> &op) const {
   #ifdef DVECT_RANGE_CHECK
   if (columns != op.columns || rows != op.rows) {
      throw ERangeCheck("DynamicMatrix::operator== - incompatible dimensions.");
   }
   #endif
   for (unsigned i=0; i<columns; i++) {
      if (theColumns[i] != op.theColumns[i]) return false;
   }
   return true;
}

template <typename FloatType>
inline bool DynamicMatrix<FloatType>::operator!=(const DynamicMatrix<FloatType> &op) const {
   #ifdef DVECT_RANGE_CHECK
   if (columns != op.columns || rows != op.rows) {
      throw ERangeCheck("DynamicMatrix::operator!= - incompatible dimensions.");
   }
   #endif
   for (unsigned i=0; i<columns; i++) {
      if (theColumns[i] != op.theColumns[i]) return true;
   }
   return false;
}

template <typename FloatType>
inline DynamicVector<FloatType> DynamicMatrix<FloatType>::operator*(const DynamicVector<FloatType> &v) const {
   #ifdef DVECT_RANGE_CHECK
   if (v.getDim() != columns) {
      throw ERangeCheck("DynamicMatrix::operator* (vect) - incompatible dimensions.");
   }
   #endif
   DynamicVector<FloatType> result(rows);
   for (unsigned y=0; y<rows; y++) {
      result[y] = 0.0;
      for (unsigned x=0; x<columns; x++) {
         result[y] += theColumns[x][y]*v[x];
      }
   }
   return result;
}

template <typename FloatType>
inline void DynamicMatrix<FloatType>::print() {
   for (unsigned i=0; i<columns; i++) {
      theColumns[i].print();
      cout << endl;
   }
}

template <typename FloatType>
string DynamicMatrix<FloatType>::toString() const {
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

template <typename FloatType>
inline void DynamicMatrix<FloatType>::copyDataToArray(FloatType* target) const{
   unsigned count = 0;
   for (unsigned j=0; j<columns; j++)
      for (unsigned i=0; i<rows; i++){
         target[count] = theColumns[j][i];
         count++;
      }
}

template <typename FloatType>
inline void DynamicMatrix<FloatType>::copyDataFromArray(FloatType* source){
   unsigned count = 0;
   for (unsigned j=0; j<columns; j++)
      for (unsigned i=0; i<rows; i++){
         theColumns[j][i]= source[count];
         count++;
      }
}

template <typename FloatType>
inline Matrix3f DynamicMatrix<FloatType>::getMatrix3f() {
   Matrix3f res;
   for ( unsigned char i = 0; i < 3; i++ )
      for ( unsigned char j = 0; j < 3; j++ )
         res[i][j] = theColumns[i][j];

   return res;
}

template <typename FloatType>
inline void DynamicMatrix<FloatType>::changeRows(unsigned row1, unsigned row2) {
   DynamicVector<FloatType> h(columns);
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

template <typename FloatType>
inline void DynamicMatrix<FloatType>::multRow(unsigned row, FloatType value) {
   for (unsigned i=0; i<columns; i++) {
      theColumns[i][row] *= value;
   }
}

template <typename FloatType>
inline void DynamicMatrix<FloatType>::combineRows(unsigned row, unsigned with, FloatType by) {
   for (unsigned i=0; i<columns; i++) {
      theColumns[i][row] += theColumns[i][with] * by;
   }
}

template <typename FloatType>
inline void DynamicMatrix<FloatType>::addRows(unsigned row, unsigned with) {
   for (unsigned i=0; i<columns; i++) {
      theColumns[i][row] += theColumns[i][with];
   }
}

template <typename FloatType>
inline DynamicMatrix<FloatType> DynamicMatrix<FloatType>::transpose() const {
   DynamicMatrix<FloatType> result(rows, columns);
   for (unsigned r=0; r<rows; r++) {
      for (unsigned c=0; c<columns; c++) {
         result[r][c] = theColumns[c][r];
      }
   }
   return result;
}

template <typename FloatType>
inline DynamicMatrix<FloatType> DynamicMatrix<FloatType>::copyColumns(unsigned startColumn, unsigned numColumns) {
   pAssert(startColumn < columns);
   pAssert(startColumn + numColumns < columns);
   DynamicMatrix<FloatType> result(numColumns, rows, false);
   for (unsigned i=0; i<numColumns; i++) {
      result[i] = theColumns[i+startColumn];
   }
   return result;
}

template <typename FloatType>
inline FloatType DynamicMatrix<FloatType>::getDeterminant() const {
   if ( ( rows == 2 ) && ( columns == 2 ) ) {
      return (*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0];
   } else if ( ( rows == 3 ) && ( columns == 3 ) ) {
      return (*this)[0][0] * ( (*this)[1][1] * (*this)[2][2] - (*this)[1][2] * (*this)[2][1] ) -
             (*this)[0][1] * ( (*this)[1][0] * (*this)[2][2] - (*this)[1][2] * (*this)[2][0] ) +
             (*this)[0][2] * ( (*this)[1][0] * (*this)[2][1] - (*this)[1][1] * (*this)[2][0] );
   } else if (rows == 1 && columns == 1) {
      return (*this)[0][0];
   } else {
      // currently does not support higher dimensions
      pAssert(false);
   }
}


// ---- destructor
template <typename FloatType>
inline DynamicMatrix<FloatType>::~DynamicMatrix() {
   delete[] theColumns;
}


// ---------------------------------------------------------------------------
//
//                                 aux. routines
//
// ---------------------------------------------------------------------------


template <typename FloatType>
inline DynamicVector<FloatType> scalarDVector(card32 dim, const FloatType &s) {
   DynamicVector<FloatType> result(dim);
   for (card32 i=0; i<dim; i++) {
      result[i] = s;
   }
   return result;
}

template <typename FloatType>
inline DynamicVector<FloatType> nullDVector(card32 dim) {
   return scalarDVector(dim, (FloatType)0);
}

template <typename FloatType>
inline DynamicVector<FloatType> makeDVector(card32 dim, const FloatType * dataptr)
{
	DynamicVector<FloatType> ret(dim);
	for( card32 i=0;i<dim;i++)
		ret[i] = dataptr[i];

	return ret;
}

template <typename FloatType1, typename FloatType2>
inline void convertDVectorType(const DynamicVector<FloatType1> &source, DynamicVector<FloatType2> &dest) {
   if (source.getDim() != dest.getDim()) {
      dest.setDim(source.getDim());
   }
   for (card32 i=0; i<source.getDim(); i++) {
      dest[i] = (FloatType1)source[i];
   }
}

template <typename FloatType>
inline DynamicMatrix<FloatType> scalarDMatrix(card32 c, card32 r, const FloatType &s) {
   DynamicMatrix<FloatType> result(c,r);
   for (card32 i=0; i<c; i++) {
      result[i] = scalarDVector(r, s);
   }
   return result;
}

template <typename FloatType>
inline DynamicMatrix<FloatType> nullDMatrix(card32 c, card32 r) {
   return scalarDMatrix(c, r, (FloatType)0);
}

template <typename FloatType>
inline DynamicMatrix<FloatType> outerProduct( const DynamicVector<FloatType> &v1,const DynamicVector<FloatType> &v2 )
{
	DynamicMatrix<FloatType> ret( v1.getDim(), v2.getDim() );
	for( card32 i=0;i<v1.getDim();i++ )
	{
		for( card32 j=0;j<v2.getDim();j++ )
		{
			ret[i][j] = v1[i] * v2[j];
		}
	}
	return ret;
}


template <typename FloatType>
inline FloatType normQuad(const DynamicVector<FloatType> &v) {
   FloatType result = 0.0;
   card32 dim = v.getDim();
   for (unsigned i=0; i<dim; i++) {
      result+=v[i]*v[i];
   }
   return result;
};

template <typename FloatType>
inline FloatType norm(const DynamicVector<FloatType> &v) {
   return sqrt(normQuad(v));
};

template <typename FloatType>
inline FloatType normInfinity(const DynamicVector<FloatType> &v){
	FloatType max = -1e10;
	for(card32 index = 0; index < v.getDim(); index++)
	{
		if(fabs(v[index]) > max)
		{
			max = fabs(v[index]);
		}
	}
	return max;
}


template <typename FloatType>
inline unsigned searchPivot(const DynamicMatrix<FloatType> &mat, unsigned step) {
   if (mat.getColumns() != mat.getRows()) {
      throw ERangeCheck("searchPivot - symmetric matrix needed.");
   }
   const card32 size = mat.getRows();
   const double MAX_PIVOT_SIZE = 0.0;
   unsigned result = step;
   FloatType max = fabs(mat[step][step]);
   for (unsigned p = step+1; p<size; p++) {
      if (fabs(mat[step][p]) > max) {
         max = fabs(mat[step][p]);
         result = p;
      }
   }
   if (max <= MAX_PIVOT_SIZE) {
      throw PException("searchPivot: no pivot element found (matrix singular?).");
   } else {
      return result;
   }
};

template <typename FloatType>
inline DynamicMatrix<FloatType> invertMatrix(const DynamicMatrix<FloatType> &mat) {
   if (mat.getColumns() != mat.getRows()) {
      throw ERangeCheck("searchPivot - symmetric matrix needed.");
   }
   const card32 size = mat.getRows();
   DynamicMatrix<FloatType> result(size, size);
   DynamicMatrix<FloatType> mCopy = mat;
   for (unsigned step=0; step<size; step++) {
      unsigned pivot = searchPivot(mCopy, step);
      if (step != pivot) {
         mCopy.changeRows(step, pivot);
         result.changeRows(step, pivot);
      }
      if (mCopy[step][step] == 0) throw PException("invertMatrix: Matrix is singular.");
      FloatType m = 1.0/mCopy[step][step];
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
   return result;
};


template <typename FloatType>
void DynamicMatrix<FloatType>::write(OutputObjectStream *s) const {
	s->write(rows);
	s->write(columns);
	for (card32 i=0; i<columns; i++) {
		theColumns[i].write(s);
	}
}

template <typename FloatType>
void DynamicMatrix<FloatType>::read(InputObjectStream *s) {
	delete[] theColumns;
	s->read(rows);
	s->read(columns);
	theColumns = new DynamicVector<FloatType>[columns];
	for (card32 i=0; i<columns; i++) {
		theColumns[i].read(s);
	}
}


template <typename FloatType>
inline DynamicVector<FloatType> maxVector( const DynamicVector<FloatType> &v1,const DynamicVector<FloatType> &v2)
{
	pAssert( v1.size() == v2.size() );
	DynamicVector<FloatType> ret(v1.size());
	for( unsigned i=0;i<v1.size();i++ )
	{
		ret[i] = max( v1[i], v2[i] );
	}
	return ret;
}


template <typename FloatType>
inline DynamicVector<FloatType> minVector( const DynamicVector<FloatType> &v1,const DynamicVector<FloatType> &v2)
{
	pAssert( v1.size() == v2.size() );
	DynamicVector<FloatType> ret(v1.size());
	for( unsigned i=0;i<v1.size();i++ )
	{
		ret[i] = min( v1[i], v2[i] );
	}
	return ret;
}




#endif
