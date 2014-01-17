/*
 * AMLoader.hpp
 *
 *  Created on: Sep 24, 2013
 *      Author: martinez
 */

#ifndef AMLOADER_HPP
#define AMLOADER_HPP

#include "GridData.hpp"

class AMLoader {
public:
  static bool load(const std::string &fname_, GridData &data_);
};

#endif //AMLOADER_HPP
