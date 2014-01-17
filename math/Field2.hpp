/*
 * Field2.hpp
 *
 *  Created on: Sep 24, 2013
 *      Author: martinez
 */

#ifndef FIELD2_HPP
#define FIELD2_HPP

#include "LinearAlgebra.h"
#include "GridData.hpp"
#include "AMLoader.hpp"

#include "GeoXOutput.h"

#include <assert.h>
#include <cmath>
#include <algorithm>

///A bilinear field class for two-dimensional domains.
template<size_t NUMCOMPONENTS>
class Field2 {
public:
  typedef StaticVector<float32,NUMCOMPONENTS> DataType;
  typedef StaticVector<float32,2>             PosType;

  Field2() {
    clear();
  }

  virtual ~Field2() {}

  const GridData& getData() const
  {
    return mGridData;
  }

  ///Inits the field with a uniform grid and sets all data to zero.
  void init(PosType boundMin_, PosType boundMax_, Vector2ui dims_) {
    mGridData.data.resize(NUMCOMPONENTS*dims_[0]*dims_[1]);
    mGridData.boundMin[0] = boundMin_[0]; mGridData.boundMin[1] = boundMin_[1]; mGridData.boundMin[2] = 0;
    mGridData.boundMax[0] = boundMax_[0]; mGridData.boundMax[1] = boundMax_[1]; mGridData.boundMax[2] = 0;
    mGridData.dims[0]     = dims_[0];     mGridData.dims[1]     = dims_[1];     mGridData.dims[2]     = 1;
    mGridData.numDataComponents = NUMCOMPONENTS;

    _updateFactors();
  }

  ///Frees all memory. The field and grid are empty after that.
  void clear() {
    mGridData.data  .clear();
    mInvScaleFactor .setZero();
    mHalfScaleFactor.setZero();
  }

  ///Initializes the memory to zero
  void setZero() {
    std::fill(mGridData.data.begin(), mGridData.data.end(), 0.f);
  }

  bool load(const std::string &fname_) {
    clear();

    if(!AMLoader::load(fname_, mGridData) ||
       mGridData.numDataComponents != NUMCOMPONENTS) {
      output << "The field could not be loaded.\n\t==> Does the file exist?\n\t==> Has it the right number of components?\n";
      return false;
    }

    _updateFactors();
    return true;
  }

  PosType boundMin() const {
    PosType min; min[0] = mGridData.boundMin[0]; min[1] = mGridData.boundMin[1];
    return min;
  }

  PosType boundMax() const {
    PosType max; max[0] = mGridData.boundMax[0]; max[1] = mGridData.boundMax[1];
    return max;
  }

  Vector2ui dims() const {
    Vector2ui dims; dims[0] = mGridData.dims[0]; dims[1] = mGridData.dims[1];
    return dims;
  }

  ///returns the closest node to the given position
  Vector2ui closestNode(const PosType &x_) const {
    //Rounded computation
    int ix = (int)(0.5f + (x_[0] - mGridData.boundMin[0]) * mInvScaleFactor[0]);
    int iy = (int)(0.5f + (x_[1] - mGridData.boundMin[1]) * mInvScaleFactor[1]);

    //Clamp
    if (ix < 0) ix = 0;
    if (ix > (int)mGridData.dims[0]-1) ix = mGridData.dims[0]-1;
    if (iy < 0) iy = 0;
    if (iy > (int)mGridData.dims[1]-1) iy = mGridData.dims[1]-1;

    return makeVector2ui((unsigned int)ix, (unsigned int)iy);
  }

  ///sample field bilinearly (with bounds check), returns vector
  DataType sample(const PosType &x_) const;
  DataType sample(const float32 x_, const float32 y_) const {
    PosType pos; pos[0] = x_; pos[1] = y_;
    return sample(pos);
  }

  DataType node(const card32 i_, const card32 j_) const;
  void  setNode(const card32 i_, const card32 j_, DataType data_);
  PosType  nodePosition(const card32 i_, const card32 j_) const;

  bool insideBounds(const PosType &x_) const {
    if(x_[0] < mGridData.boundMin[0] || x_[1] < mGridData.boundMin[1] ||
       x_[0] > mGridData.boundMax[0] || x_[1] > mGridData.boundMax[1])
      return false;
    return true;
  }

protected:
  GridData mGridData;
  Vector2f mInvScaleFactor, mHalfScaleFactor;

  card32 linearIndex(const card32 i_, const card32 j_) const {
    return i_ + mGridData.dims[0] * j_;
  }

  void _updateFactors() {
    mInvScaleFactor[0]  = float32(mGridData.dims[0]-1) / (mGridData.boundMax[0] - mGridData.boundMin[0]);
    mInvScaleFactor[1]  = float32(mGridData.dims[1]-1) / (mGridData.boundMax[1] - mGridData.boundMin[1]);

    mHalfScaleFactor[0] = 1 / (2*mInvScaleFactor[0]);
    mHalfScaleFactor[1] = 1 / (2*mInvScaleFactor[1]);
  }
};

template<size_t NUMCOMPONENTS>
typename Field2<NUMCOMPONENTS>::DataType
Field2<NUMCOMPONENTS>::sample(const PosType &x_) const {
  assert(mGridData.data.size());

  //check bounds
  if(!insideBounds(x_))
    return DataType().setZero();

  //get uniform parametrization location
  PosType localPos;
  localPos[0] = (x_[0] - mGridData.boundMin[0]) * mInvScaleFactor[0];
  localPos[1] = (x_[1] - mGridData.boundMin[1]) * mInvScaleFactor[1];

  //get grid indices
  const card32  i0 = card32(std::floor(localPos[0])),
                j0 = card32(std::floor(localPos[1]));

  const card32  i1 = min(mGridData.dims[0]-1, i0+1),
                j1 = min(mGridData.dims[1]-1, j0+1);

  //get uniform parametrization
  const float32 u = localPos[0] - float32(i0),
                v = localPos[1] - float32(j0);

  //perform bilinear interpolation for each data component
  const float32 b00 = (1-u)*(1-v),
                b10 =     u*(1-v),
                b01 = (1-u)*v,
                b11 =     u*v;

  DataType out;

  for(size_t k=0; k<NUMCOMPONENTS; ++k)
    out[k] = b00 * mGridData.data[NUMCOMPONENTS*linearIndex(i0, j0) + k] +
             b10 * mGridData.data[NUMCOMPONENTS*linearIndex(i1, j0) + k] +
             b01 * mGridData.data[NUMCOMPONENTS*linearIndex(i0, j1) + k] +
             b11 * mGridData.data[NUMCOMPONENTS*linearIndex(i1, j1) + k];

  return out;
}

template<size_t NUMCOMPONENTS>
typename Field2<NUMCOMPONENTS>::DataType
Field2<NUMCOMPONENTS>::node(const card32 i_, const card32 j_) const {
  DataType out;
  for(size_t k=0; k<NUMCOMPONENTS; ++k)
    out[k] = mGridData.data[NUMCOMPONENTS*linearIndex(i_, j_) + k];
  return out;
}

template<size_t NUMCOMPONENTS>
void Field2<NUMCOMPONENTS>::setNode(const card32 i_, const card32 j_,
                                    DataType data_) {
  for(size_t k=0; k<NUMCOMPONENTS; ++k)
    mGridData.data[NUMCOMPONENTS*linearIndex(i_, j_) + k] = data_[k];
}

template<size_t NUMCOMPONENTS>
typename Field2<NUMCOMPONENTS>::PosType
Field2<NUMCOMPONENTS>::nodePosition(const card32 i_, const card32 j_) const {
  PosType out;
  out[0] = mGridData.boundMin[0] + i_ * (1.0f / mInvScaleFactor[0]);
  out[1] = mGridData.boundMin[1] + j_ * (1.0f / mInvScaleFactor[1]);
  return out;
}

class ScalarField2 : public Field2<1> {
public:
  typedef Field2<1>::DataType DataType;
  typedef Field2<1>::PosType  PosType;

  //scalar methods
  float32 sampleScalar(const PosType &x_) const {
    return Field2<1>::sample(x_)[0];
  }
  float32 sampleScalar(const float32 x_, const float32 y_) const {
    return Field2<1>::sample(x_, y_)[0];
  }
  float32 nodeScalar(const card32 i_, const card32 j_) const {
    return Field2<1>::node(i_, j_)[0];
  }
  void    setNodeScalar(const card32 i_, const card32 j_, float32 data_) {
    DataType data; data[0] = data_;
    Field2<1>::setNode(i_, j_, data);
  }

  Vector2f sampleGradient(const PosType &x_) {
    //check bounds
    if(!insideBounds(x_))
      return PosType().setZero();

    float32 dx, dy, vHi, vLo;
    PosType pHi, pLo;

    {
      //dx derivative
      pHi[0] = x_[0] + mInvScaleFactor[0]; pHi[1] = x_[1];
      pLo[0] = x_[0] - mInvScaleFactor[0]; pLo[1] = x_[1];

      vHi = sampleScalar(pHi),
        vLo = sampleScalar(pLo);

      if(!insideBounds(pHi)) {
        //backward difference
        dx = sampleScalar(x_);
        dx = (dx - vLo)  / mInvScaleFactor[0];
      } else if(!insideBounds(pLo)) {
        //forward difference
        dx = sampleScalar(x_);
        dx = (vHi - dx)  / mInvScaleFactor[0];
      } else {
        //central difference
        dx = (vHi - vLo) / (2*mInvScaleFactor[0]);
      }
    }

    {
      //dy derivative
      pHi[0] = x_[0]; pHi[1] = x_[1] + mHalfScaleFactor[1];
      pLo[0] = x_[0]; pLo[1] = x_[1] - mHalfScaleFactor[1];

      vHi = sampleScalar(pHi),
        vLo = sampleScalar(pLo);

      if(!insideBounds(pHi)) {
        //backward difference
        dy = sampleScalar(x_);
        dy = (dy - vLo) / mHalfScaleFactor[1];
      } else if(!insideBounds(pLo)) {
        //forward difference
        dy = sampleScalar(x_);
        dy = (vHi - dy)  / mHalfScaleFactor[1];
      } else {
        //central difference
        dy = (vHi - vLo) / (2*mHalfScaleFactor[1]);
      }
    }

    PosType out; out[0] = dx; out[1] = dy;
    return out;
  }

  Vector2f sampleGradient(const float32 x_, const float32 y_) {
    PosType pos; pos[0] = x_; pos[1] = y_;
    return sampleGradient(pos);
  }
};

class VectorField2 : public Field2<2> {
public:
  typedef Field2<2>::DataType DataType;
  typedef Field2<2>::PosType  PosType;

  Matrix2f sampleJacobian(const PosType &x_) {
    //check bounds
    if(!insideBounds(x_))
      return Matrix2f().setZero();

    DataType dx, dy, vHi, vLo;
    PosType pHi, pLo;

    {
      //dx derivative
      pHi[0] = x_[0] + mHalfScaleFactor[0]; pHi[1] = x_[1];
      pLo[0] = x_[0] - mHalfScaleFactor[0]; pLo[1] = x_[1];

      vHi = sample(pHi),
      vLo = sample(pLo);

      if(!insideBounds(pHi)) {
        //backward difference
        dx = sample(x_);
        dx = (dx - vLo)  / mHalfScaleFactor[0];
      } else if(!insideBounds(pLo)) {
        //forward difference
        dx = sample(x_);
        dx = (vHi - dx)  / mHalfScaleFactor[0];
      } else {
        //central difference
        dx = (vHi - vLo) / (2*mHalfScaleFactor[0]);
      }
    }

    {
      //dy derivative
      pHi[0] = x_[0]; pHi[1] = x_[1] + mHalfScaleFactor[1];
      pLo[0] = x_[0]; pLo[1] = x_[1] - mHalfScaleFactor[1];

      vHi = sample(pHi),
      vLo = sample(pLo);

      if(!insideBounds(pHi)) {
        //backward difference
        dy = sample(x_);
        dy = (dy - vLo) / mHalfScaleFactor[1];
      } else if(!insideBounds(pLo)) {
        //forward difference
        dy = sample(x_);
        dy = (vHi - dy)  / mHalfScaleFactor[1];
      } else {
        //central difference
        dy = (vHi - vLo) / (2*mHalfScaleFactor[1]);
      }
    }

    Matrix2f out;
    out[0][0] = dx[0]; out[0][1] = dx[1];
    out[1][0] = dy[0]; out[1][1] = dy[1];

    return out;
  }


  Matrix2f sampleJacobian(const float32 x_, const float32 y_) {
    PosType pos; pos[0] = x_; pos[1] = y_;
    return sampleJacobian(pos);
  }
};


#endif // FIELD2_HPP
