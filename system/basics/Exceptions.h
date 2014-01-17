//---------------------------------------------------------------------------
#ifndef ExceptionsH
#define ExceptionsH
//---------------------------------------------------------------------------
#include <string>
#include <iostream>
#include <stdio.h>
//---------------------------------------------------------------------------


using namespace std;

class  PException {
 protected:
   string Msg;
 public:
   PException(const string msg) {
      this->Msg = msg;
      #ifdef _DEBUG
      cerr << msg.c_str();
      #endif
   }
   string getMsg() {return Msg;}
   void print() {cerr << Msg.c_str();}
   virtual string getTypeString() {return "PException";}
   virtual ~PException() {}
};

class  ERangeCheck : public PException {
 public:
   ERangeCheck(const string msg) : PException(msg) {};
   virtual string getTypeString() {return "ERangeCheck";}
};

class  EIOException : public PException {
 public:
   EIOException(const string msg) : PException(msg) {};
   virtual string getTypeString() {return "EIOException";}
};

class  EMath : public PException {
 public:
   EMath(const string msg) : PException(msg) {};
   virtual string getTypeString() {return "EMath";}
};

class  EInvalidState : public PException {
 public:
   EInvalidState(const string msg) : PException(msg) {};
   virtual string getTypeString() {return "EInvalidState";}
};

class  ETypeMismatch : public PException {
 public:
   ETypeMismatch(const string msg) : PException(msg) {};
   virtual string getTypeString() {return "ETypeMismatch";}
};



#endif
