//---------------------------------------------------------------------------
#ifndef ExceptionHandlingH
#define ExceptionHandlingH
//---------------------------------------------------------------------------
#include "Exceptions.h"
//---------------------------------------------------------------------------
#include <exception>
//---------------------------------------------------------------------------



// -- helper functions that display exceptions on the console

void  displayPException(PException *e);

#ifdef _WIN32
void  displayOSException(unsigned int code, EXCEPTION_RECORD info);
#endif

void  displayGeneralException();

void  displaySTLException(std::exception e);


// -- structured win32/win64 exception wrappper

#ifdef _WIN32
class SE_Exception {
 public:
   unsigned int nSE;
	EXCEPTION_RECORD info;
	
   SE_Exception(unsigned int n, EXCEPTION_RECORD exceptionInfo) : nSE(n), info(exceptionInfo) {}
   ~SE_Exception() {}
};

 void SE_wrapper_trans_func( unsigned int u, _EXCEPTION_POINTERS* pExp );
#endif

/// call this once in main to setup handling of structured exception (i.e. WIN32-Exceptions)
 void setStructureExceptionHandler();


// -- macros for "global" exception handling (i.e. in qt-slots that receive input from qapp directly)


#define STD_GLOBAL_EXCEPTION_HANDLER_START \
try {\



#ifdef _WIN32

#define STD_GLOBAL_EXCEPTION_HANDLER_END \
} catch (PException &e) {\
	displayPException(&e);\
} catch (SE_Exception e) {\
	displayOSException(e.nSE, e.info);\
} catch (std::exception e) {\
	displaySTLException(e);\
} catch (...) {\
	displayGeneralException();\
}

#else

#define STD_GLOBAL_EXCEPTION_HANDLER_END \
} catch (PException &e) {\
	displayPException(&e);\
} catch (...) {\
	displayGeneralException();\
}

#endif


#endif
