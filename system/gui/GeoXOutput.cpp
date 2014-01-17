//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "GeoXOutput.h"
//---------------------------------------------------------------------------
#include "LinearAlgebra.hpp"
#include "Dynamiclinearalgebra.hpp"
#include "SparseLinearAlgebra.hpp"
#include "GeoXMainWindow.h"
//---------------------------------------------------------------------------
//#include "DebugWindows.h"
//---------------------------------------------------------------------------


using namespace std;

GeoXOutput output;


GeoXOutput& GeoXOutput::operator<<(bool b) {
   mainWindow->output(b);
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(int i)  {
   mainWindow->output(i);
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(unsigned int i)  {
   mainWindow->output(i);
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(long unsigned int i)  {
   mainWindow->output(i);
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(int64 i)  {
   mainWindow->output(i);
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(card64 i)  {
   mainWindow->output(i);
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(long double d)  {
   mainWindow->output(d);
   return *this;
}


GeoXOutput& GeoXOutput::operator<<(float f)  {
   mainWindow->output((long double)f);
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(double d)  {
   mainWindow->output((long double)d);
   return *this;
}


GeoXOutput& GeoXOutput::operator<<(const string& s)
{
  mainWindow->output(s);
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(const char* s)
{
   string tmpS = s;
   *this << tmpS;
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(const Vector2f &v)  {
  (*this) << "(" << (long double)v[0] << " / " << (long double)v[1] << ")";
  return *this;
}

GeoXOutput& GeoXOutput::operator<<(const Vector3f &v)  {
  (*this) << "(" << (long double)v[0] << " / " << (long double)v[1] << " / " << (long double)v[2] << ")";
  return *this;
}

GeoXOutput& GeoXOutput::operator<<(const Vector3i &v)  {
  (*this) << "(" << int32(v[0]) << " / " << (int32)v[1] << " / " << (int32)v[2] << ")";
  return *this;
}

GeoXOutput& GeoXOutput::operator<<(const Vector4f &v)  {
  (*this) << "(" << (long double)v[0] << " / " << (long double)v[1] << " / " << (long double)v[2] << " / " << (long double)v[3] << ")";
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(const Vector6f &v)  {
  (*this) << "(" << (long double)v[0] << " / " << (long double)v[1] << " / " << (long double)v[2] << " / " << (long double)v[3] << " / " << (long double)v[4] <<" / " << (long double)v[5] <<")";
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(const Matrix4f &m)  {
   (*this) << "         " << m[0][0] << "  " << m[0][1] << "  " << m[0][2]<< "  " << m[0][3];
   (*this).cr();
   (*this) << "         " << m[1][0] << "  " << m[1][1] << "  " << m[1][2]<< "  " << m[1][3];
   (*this).cr();
   (*this) << "         " << m[2][0] << "  " << m[2][1] << "  " << m[2][2]<< "  " << m[2][3];
   (*this).cr();
   (*this) << "         " << m[3][0] << "  " << m[3][1] << "  " << m[3][2]<< "  " << m[3][3];
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(const Matrix3f &m)  {
   (*this) << " " << m[0][0] << "  " << m[0][1] << "  " << m[0][2];
   (*this).cr();
   (*this) << " " << m[1][0] << "  " << m[1][1] << "  " << m[1][2];
   (*this).cr();
   (*this) << " " << m[2][0] << "  " << m[2][1] << "  " << m[2][2];

   return *this;
}

GeoXOutput& GeoXOutput::operator<<(const Matrix2f &m)  {
   (*this) << " " << m[0][0] << "  " << m[0][1]
   << "\n"
		   << " " << m[1][0] << "  " << m[1][1];

   return *this;
}


GeoXOutput& GeoXOutput::operator<<(const BoundingBox3f &bbox )  {
  (*this) << "(" << bbox.lowerCorner[0] << "," << bbox.lowerCorner[1] << "," << bbox.lowerCorner[2] << ")/";
  (*this) << "(" << bbox.upperCorner[0] << "," << bbox.upperCorner[1] << "," << bbox.upperCorner[2] << ")";
  return *this;
}

GeoXOutput& GeoXOutput::operator<<(const StaticVector<float32,7> &v)
{
  (*this) << "(" << v[0] << "," << v[1] << "," << v[2] << "," << v[3] << "," << v[4] << "," << v[5] << "," << v[6] << ")";
  return *this;
}


GeoXOutput& GeoXOutput::operator<<(const DynamicVector<float> &v)
{
  (*this) << "(";
  for( unsigned i=0;i<v.size();i++ )
  {
    if( i )
      (*this) << ",";
    (*this) << v[i];
  }
  (*this) << ")";

  return *this;
}

GeoXOutput& GeoXOutput::operator<<(const DynamicMatrix<float> &m)
{
   for ( unsigned int r = 0; r < m.getRows(); r++ ){
      for ( unsigned int c = 0; c < m.getColumns(); c++ ){
         (*this) << m[c][r] << " ";
      }
      (*this).cr();
   }
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(SparseMatrixF &A)
{
  float32 value;
  card32 index;
  for ( unsigned int r = 0; r < A.getRows(); r++ ){
    SparseVectorF& row = A[r];
    for ( unsigned int i = 0; i < row.getNumSparseEntries(); i++ ){
      row.getSparseEntry( i, index, value );
      (*this) << index << ": " << value << ", ";
    }
    (*this).cr();
  }
  return *this;
}


void GeoXOutput::writeIdented(unsigned ident, string s) {
   string identsp;
   for (unsigned i = 0; i<ident; i++) identsp += " ";
   mainWindow->output(identsp + s);
}

void GeoXOutput::cr()  {
   mainWindow->output("\n");
}

void warning(const string& msg) {
   mainWindow->warningMessage(msg);
}

void error(const string& msg) {
   mainWindow->errorMessage(msg);
}

void message(const string& msg) {
   output << msg;
}
