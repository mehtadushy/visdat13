/*
 * AMLoader.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: martinez
 */

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
#endif

//---------------------------------------------
#include "stdafx.h"
//---------------------------------------------
#include "AMLoader.hpp"

#include "GeoXOutput.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

const char* FindAndJump(const char* buffer, const char* SearchString) {
  const char* FoundLoc = strstr(buffer, SearchString);
  if (FoundLoc) return FoundLoc + strlen(SearchString);
  return buffer;
}

bool AMLoader::load(const std::string &fname_, GridData &data_) {
  //based on http://www.mpi-inf.mpg.de/~weinkauf/notes/amiramesh.html

  FILE* fp = fopen(fname_.c_str(), "rb");
  if (!fp) {
    output << "AMLoader: " << "Error: Could not find " << fname_.c_str() << "\n";
    return false;
  }

  output << "AMLoader: " << "Reading " << fname_.c_str() << "\n";

  //We read the first 2k bytes into memory to parse the header.
  //The fixed buffer size looks a bit like a hack, and it is one, but it gets the job done.
  char buffer[2048];
  fread(buffer, sizeof(char), 2047, fp);
  buffer[2047] = '\0'; //The following string routines prefer null-terminated strings

  if (!strstr(buffer, "# AmiraMesh BINARY-LITTLE-ENDIAN 2.1")) {
    output << "AMLoader: " << "Error: Not a proper AmiraMesh file.\n";
    fclose(fp);
    return false;
  }

  //Find the Lattice definition, i.e., the dimensions of the uniform grid
  int xDim(0), yDim(0), zDim(0);
  sscanf(FindAndJump(buffer, "define Lattice"), "%d %d %d", &xDim, &yDim, &zDim);
  output << "AMLoader: " << "\tGrid Dimensions: " << xDim << " " << yDim << " " << zDim << "\n";

  data_.dims[0] = xDim;
  data_.dims[1] = yDim;
  data_.dims[2] = zDim;

  //Find the BoundingBox
  float xmin(1.0f), ymin(1.0f), zmin(1.0f);
  float xmax(-1.0f), ymax(-1.0f), zmax(-1.0f);
  sscanf(FindAndJump(buffer, "BoundingBox"), "%g %g %g %g %g %g", &xmin, &xmax, &ymin, &ymax, &zmin, &zmax);
  output << "AMLoader: " << "\tBoundingBox in x-Direction: [" << xmin << " ... " << xmax << "]\n";
  output << "AMLoader: " << "\tBoundingBox in y-Direction: [" << ymin << " ... " << ymax << "]\n";
  output << "AMLoader: " << "\tBoundingBox in z-Direction: [" << zmin << " ... " << zmax << "]\n";

  data_.boundMin[0]   = xmin; data_.boundMax[0] = xmax;
  data_.boundMin[1]   = ymin; data_.boundMax[1] = ymax;
  data_.boundMin[2]   = zmin; data_.boundMax[2] = zmax;

  //Is it a uniform grid? We need this only for the sanity check below.
  const bool bIsUniform = (strstr(buffer, "CoordType \"uniform\"") != NULL);
  output << "AMLoader: " << "\tGridType " << (bIsUniform ? "uniform" : "UNKNOWN") << "\n";

  //Type of the field: scalar, vector
  int NumComponents(0);
  if (strstr(buffer, "Lattice { float Data }")) {
    //Scalar field
    NumComponents = 1;
  } else {
    //A field with more than one component, i.e., a vector field
    sscanf(FindAndJump(buffer, "Lattice { float["), "%d", &NumComponents);
  }
  output << "AMLoader: " << "\tNumber of Components: " << NumComponents << "\n";

  data_.numDataComponents = NumComponents;

  //Sanity check
  if (xDim <= 0 || yDim <= 0 || zDim <= 0
      || xmin > xmax || ymin > ymax || zmin > zmax
      || !bIsUniform || NumComponents <= 0) {
    output << "AMLoader: " << "Error: Something went wrong!\n";
    fclose(fp);
    return false;
  }

  data_.data.clear();

  //Find the beginning of the data section
  const long idxStartData = strstr(buffer, "# Data section follows") - buffer;
  if (idxStartData > 0) {
    //Set the file pointer to the beginning of "# Data section follows"
    fseek(fp, idxStartData, SEEK_SET);
    //Consume this line, which is "# Data section follows"
    fgets(buffer, 2047, fp);
    //Consume the next line, which is "@1"
    fgets(buffer, 2047, fp);

    //Read the data
    // - how much to read
    const size_t NumToRead = xDim * yDim * zDim * NumComponents;

    data_.data.resize(NumToRead);

    // - do it
    const size_t ActRead = fread((void*)&data_.data[0], sizeof(float), NumToRead, fp);
    // - ok?
    if (NumToRead != ActRead) {
      output << "AMLoader: " << "Error: Something went wrong while reading the binary data section.\nPremature end of file?\n";
      data_.data.clear();
      fclose(fp);
      return false;
    }
  }

  fclose(fp);
  return true;
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
