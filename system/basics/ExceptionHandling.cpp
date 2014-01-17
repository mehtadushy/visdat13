//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ExceptionHandling.h"
//---------------------------------------------------------------------------
#include "GeoXOutput.h"
//---------------------------------------------------------------------------

#ifdef _WIN32
   #pragma warning(disable:4996)
#endif

void displayPException(PException *e) {
	try {
		string msg;
		msg += "GeoX Exception, TYPE: ";
		msg += e->getTypeString();
		msg += "\n\nmessage: ";
		msg += e->getMsg();
		msg += "\n\n";
		error(msg);
	} catch (...) {}
}


#ifdef _WIN32
string translateOSException(unsigned int code) {
	string a,b;
	switch (code) {
		case EXCEPTION_ACCESS_VIOLATION: a = "EXCEPTION_ACCESS_VIOLATION"; b = "The thread tried to read from or write to a virtual address for which it does not have the appropriate access. "; break;
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: a = "EXCEPTION_ARRAY_BOUNDS_EXCEEDED"; b = "The thread tried to access an array element that is out of bounds and the underlying hardware supports bounds checking. "; break;
		case EXCEPTION_BREAKPOINT: a = "EXCEPTION_BREAKPOINT"; b = "A breakpoint was encountered. "; break;
		case EXCEPTION_DATATYPE_MISALIGNMENT: a = "EXCEPTION_DATATYPE_MISALIGNMENT"; b = "The thread tried to read or write data that is misaligned on hardware that does not provide alignment. For example, 16-bit values must be aligned on 2-byte boundaries; 32-bit values on 4-byte boundaries, and so on. "; break;
		case EXCEPTION_FLT_DENORMAL_OPERAND: a = "EXCEPTION_FLT_DENORMAL_OPERAND"; b = "One of the operands in a floating-point operation is denormal. A denormal value is one that is too small to represent as a standard floating-point value. "; break;
		case EXCEPTION_FLT_DIVIDE_BY_ZERO: a = "EXCEPTION_FLT_DIVIDE_BY_ZERO"; b = "The thread tried to divide a floating-point value by a floating-point divisor of zero. "; break;
		case EXCEPTION_FLT_INEXACT_RESULT: a = "EXCEPTION_FLT_INEXACT_RESULT"; b = "The result of a floating-point operation cannot be represented exactly as a decimal fraction. "; break;
		case EXCEPTION_FLT_INVALID_OPERATION: a = "EXCEPTION_FLT_INVALID_OPERATION"; b = "This exception represents any floating-point exception not included in this list. "; break;
		case EXCEPTION_FLT_OVERFLOW: a = "EXCEPTION_FLT_OVERFLOW"; b = "The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type. "; break;
		case EXCEPTION_FLT_STACK_CHECK: a = "EXCEPTION_FLT_STACK_CHECK"; b = "The stack overflowed or underflowed as the result of a floating-point operation. "; break;
		case EXCEPTION_FLT_UNDERFLOW: a = "EXCEPTION_FLT_UNDERFLOW"; b = "The exponent of a floating-point operation is less than the magnitude allowed by the corresponding type. "; break;
		case EXCEPTION_ILLEGAL_INSTRUCTION: a = "EXCEPTION_ILLEGAL_INSTRUCTION"; b = "The thread tried to execute an invalid instruction. "; break;
		case EXCEPTION_IN_PAGE_ERROR: a = "EXCEPTION_IN_PAGE_ERROR"; b = "The thread tried to access a page that was not present, and the system was unable to load the page. For example, this exception might occur if a network connection is lost while running a program over the network. "; break;
		case EXCEPTION_INT_DIVIDE_BY_ZERO: a = "EXCEPTION_INT_DIVIDE_BY_ZERO"; b = "The thread tried to divide an integer value by an integer divisor of zero. "; break;
		case EXCEPTION_INT_OVERFLOW: a = "EXCEPTION_INT_OVERFLOW"; b = "The result of an integer operation caused a carry out of the most significant bit of the result. "; break;
		case EXCEPTION_INVALID_DISPOSITION: a = "EXCEPTION_INVALID_DISPOSITION"; b = "An exception handler returned an invalid disposition to the exception dispatcher. Programmers using a high-level language such as C should never encounter this exception. "; break;
		case EXCEPTION_NONCONTINUABLE_EXCEPTION: a = "EXCEPTION_NONCONTINUABLE_EXCEPTION"; b = "The thread tried to continue execution after a noncontinuable exception occurred. "; break;
		case EXCEPTION_PRIV_INSTRUCTION: a = "EXCEPTION_PRIV_INSTRUCTION"; b = "The thread tried to execute an instruction whose operation is not allowed in the current machine mode. "; break;
		case EXCEPTION_SINGLE_STEP: a = "EXCEPTION_SINGLE_STEP"; b = "A trace trap or other single-instruction mechanism signaled that one instruction has been executed. "; break;
		case EXCEPTION_STACK_OVERFLOW: a = "EXCEPTION_STACK_OVERFLOW"; b = "The thread used up its stack. "; break;
		default: a = "unknown error code"; b = "no description."; break;
	}
	return a + " - " + b;
}


void displayOSException(unsigned int code, EXCEPTION_RECORD info) {
	try {
		string msg;
		msg += "OS Exception, code 0x";
		char buffer[100];
		itoa(code, buffer, 16);
		msg += buffer;
		msg += " at instruction addr.: ";
		_i64toa((__int64)info.ExceptionAddress, buffer, 16);
		msg += buffer;
		if (code == EXCEPTION_ACCESS_VIOLATION) {
			if (info.NumberParameters >= 1) {
				if (info.ExceptionInformation[0] == 0) {
					msg += ": illegal read access ";
				} else if (info.ExceptionInformation[0] == 1) {
					msg += ": illegal write access ";
				}
			}
			if (info.NumberParameters >= 2) {
				msg += "at address ";
				_i64toa(info.ExceptionInformation[1], buffer, 16);
				msg += buffer;
			}
		}
		msg += "\n\n";
		msg += translateOSException(code);
		msg += "\n"; 
		error(msg);
	} catch (...) {}
}

void SE_wrapper_trans_func( unsigned int u, _EXCEPTION_POINTERS* pExp ) {
   throw SE_Exception( u, *pExp->ExceptionRecord );
}
#endif

void setStructureExceptionHandler() {
	#ifdef _WIN32
   _set_se_translator( SE_wrapper_trans_func );
   #endif
}


void displaySTLException(std::exception e) {
	try {
		string msg;
		msg += "STL/std Exception, message: ";
		msg += e.what();
		msg += "\n\n";
		error(msg);
	} catch (...) {}
}


void displayGeneralException() {
	try {
		string msg;
		msg += "An unknown \"(...)\" exception occured.";
		msg += "\n";
		error(msg);
	} catch (...) {}
}
