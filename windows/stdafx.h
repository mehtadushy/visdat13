// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifdef _WIN32

#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
  #define _CRT_SECURE_NO_WARNINGS
#endif

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <mmsystem.h>

#undef min
#undef max

namespace std {
  template <typename T> inline bool isfinite(T _x) { return _finite(_x) != 0; }
  template <typename T> inline bool isnan(T _x) { return _isnan(_x) != 0; }
  template <typename T> inline bool isinf(T _x) { return _finite(_x)==0 || _isnan(_x)!=0; }
}

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here
#define GEOX_PRECOMPILE_A_LOT
#ifdef GEOX_PRECOMPILE_A_LOT

   #include <gl/gl.h>
   #include <gl/glu.h>

   #include <stdlib.h>
   #include <stdio.h>
   #include <math.h>
   #include <values_vs.h>
   #include <iostream>
   #include <stack>
   #include <string>
   #include <list>
   #include <deque>
   #include <algorithm>
   #include <vector>
   #include <map>

   #include <QtCore/Qt>
   #include <QtCore/qnamespace.h>
   #include <QtCore/QDir>
   #include <QtCore/QString>
   #include <QtCore/QTimer>
   #include <QtGui/QWidget.h>
   #include <QtGui/QApplication>
   #include <QtGui/QBitmap>
   #include <QtGui/QBoxLayout>
   #include <QtGui/QComboBox>
   #include <QtGui/QDesktopWidget>
   #include <QtGui/QDialog>
   #include <QtGui/qevent.h>
   #include <QtGui/QFileDialog>
   #include <QtGui/QFrame>
   #include <QtGui/QHBoxLayout>
   #include <QtGui/QIcon>
   #include <QtGui/QImage>
   #include <QtGui/QInputDialog>
   #include <QtGui/QKeyEvent>
   #include <QtGui/QLabel>
   #include <QtGui/QLineEdit>
   #include <QtGui/QMenu>
   #include <QtGui/QMessageBox>
   #include <QtGui/QPalette>
   #include <QtGui/QPixmap>
   #include <QtGui/QPushButton>
   #include <QtGui/QScrollArea.h>
   #include <QtGui/QSpacerItem>
   #include <QtGui/QSplitter>
   #include <QtGui/QTreeWidget>
   #include <QtGui/QVBoxLayout>


   #include "HashTables.h"
   #include "Tools.h"
   #include "PTypes.h"
   #include "MetaClass.h"
   #include "Object.h"
   #include "Persistent.h"
   #include "LinearAlgebra.h"
   #include "LinearAlgebra.hpp"
   #include "Exceptions.h"
   #include "pAssert.h"
   #include "ObjectClassProperty.h"
   #include "EmbeddedObjectClassProperty.h"
   #include "StringHelper.h"
   #include "NumericalClassProperty.h"
   #include "BooleanClassProperty.h"
   #include "StringClassProperty.h"
   #include "FixedArrayClassProperty.h"
   #include "VariableArrayClassProperty.h"
   #include "HalfSpace.h"
   #include "SparseLinearAlgebra.h"
   #include "SparseLinearAlgebra.hpp"
   #include "DynamicLinearAlgebra.h"
   #include "DynamicLinearAlgebra.hpp"
   #include "ObjectStreams.h"
   #include "ObjectViewsTable.h"
   #include "GeoXOutput.h"
   #include "BoundingBox.h"
   #include "BoundingBox.hpp"

#endif

#endif
