#pragma once

#define BITSPERBYTE 8
#define MAXSHORT    0x7fff

#undef MAXINT
#define MAXINT      0x7fffffff
#define HIBITS      0x80000000
#define HIBITI      0x80000000

#define MAXLONG     0x7fffffff
#define HIBITL      0x80000000

#define DMAXEXP     308
#define FMAXEXP     38
#define DMINEXP     -307
#define FMINEXP     -37

#define MAXDOUBLE   1.7976931348623158E+308
#define MAXFLOAT    3.4E+38F
#define MINDOUBLE   2.2250738585072014E-308
#define MINFLOAT    1.17549435E-38F
#define MAXLDOUBLE  1.1897314953572317649E+4932L
#define MINLDOUBLE  3.362103143112094E-4917L 

#define DSIGNIF     53
#define FSIGNIF     24

#define DMAXPOWTWO  0x3FF
#define FMAXPOWTWO  0x7F
#define DEXPLEN     11
#define FEXPLEN     8
#define EXPBASE     2
#define IEEE        1
#define LENBASE     1
#define HIDDENBIT   1
#define LN_MAXDOUBLE    7.0978E+2
#define LN_MINDOUBLE    -7.0840E+2

#ifndef M_PI
	#define M_PI 3.1415926535897932384626433832795
#endif
