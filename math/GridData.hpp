/*
 * GridData.hpp
 *
 *  Created on: Sep 24, 2013
 *      Author: martinez
 */

#ifndef GRIDDATA_HPP
#define GRIDDATA_HPP

#include "PTypes.h"
#include "LinearAlgebra.h"

#include <vector>

struct GridData {
  std::vector<float32> data;
  card32               numDataComponents;
  Vector3ui            dims;
  Vector3f             boundMin, boundMax;

  void clear() {
    data.clear();
    numDataComponents = 0;
    dims.setZero();
    boundMax.setZero(); boundMin.setZero();
  }
};

#endif // GRIDDATA_HPP
