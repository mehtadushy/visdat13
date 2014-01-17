//---------------------------------------------------------------------------
#ifndef PTypesH
#define PTypesH
//---------------------------------------------------------------------------

/*--------------------------------------------------------------------------------*/
/* ptypes.h: elementary datatypes to enable portability between                   */
/*           multiple platforms                                                   */
/*--------------------------------------------------------------------------------*/


//---------------------------------------------------------------------------
typedef signed   char            int8;
typedef signed   short int       int16;
typedef signed   int             int32;

typedef unsigned char            card8;
typedef unsigned short int       card16;
typedef unsigned int             card32;

typedef float                    float32;
typedef double                   float64;

#ifdef _WIN32
typedef signed   __int64         int64;
typedef unsigned __int64         card64;
#else
typedef signed   long long       int64;
typedef unsigned long long       card64;
#endif

// native platform int-types, maximum sized ints that can be used for addressing (indexing, pointers)
typedef unsigned long           max_platform_card;
typedef signed int              max_platform_int;

typedef long int		pointer_diff;
typedef long int		int_pointer;
//typedef ptrdiff_t		pointer_diff;
//typedef intptr_t		int_pointer;


// shortcuts
typedef max_platform_card mpcard;
typedef max_platform_int  mpint;



#endif

