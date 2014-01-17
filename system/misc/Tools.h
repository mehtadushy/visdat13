//---------------------------------------------------------------------------
#ifndef ToolsH
#define ToolsH
//---------------------------------------------------------------------------
#include <string>
//---------------------------------------------------------------------------


using namespace std;

class Object;

#ifdef _DEBUG
   template <class T>
   void forget(T) {}
#else
	#ifdef _WIN32
   #pragma warning(disable:4101)
   #endif
   #define forget(T)
#endif

extern void  notImplemented();

string  makeObjectDescription(const Object *obj, bool shortDescr = false);

extern  bool FileExists( const char * filename );



#endif
