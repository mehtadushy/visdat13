//---------------------------------------------------------------------------
#ifndef ImageLoaderH
#define ImageLoaderH
//---------------------------------------------------------------------------
#include "GLGeometryViewer.h"
//---------------------------------------------------------------------------

/**
* Load an image from given file. Add pixels as points to the viewer.
* xdim and ydim parameters specify the dimensions of the plane of points put
* to the viewer. The plane is always centered at origin.
* return true on success otherwise false.
*/
extern bool loadImage(const std::string& fileName, GLGeometryViewer* viewer, float scale = 1);

#endif
