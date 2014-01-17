//---------------------------------------------------------------------------
#ifndef DelegateH
#define DelegateH
//---------------------------------------------------------------------------
#include "Object.h"
//---------------------------------------------------------------------------

class QWidget;

template <typename ArgumentsType, typename ResultType, class BaseType = Object>
class DelegateTempl {
 public:
   typedef ResultType (BaseType::*MethodPtr)(ArgumentsType);
 private:
   BaseType *obj;
   MethodPtr method;
     
 public:
   DelegateTempl(const DelegateTempl<ArgumentsType, ResultType, BaseType> &other) {
      obj = other.obj;
      method = other.method;
   }
   DelegateTempl(int mustBeNull) {
		pAssert(mustBeNull == NULL);
      obj = NULL;
      method = NULL;
   }
   template <typename AnyMethodType>
   DelegateTempl(BaseType *obj, AnyMethodType method) {
      this->obj = obj;
      this->method = (MethodPtr)method;
   }
   ResultType operator()(ArgumentsType args) {
		pAssert(obj);
		pAssert(method);
      return (obj->*method)(args);
   }
   void callIfNotNull(ArgumentsType args) {
		if (obj && method) {
			(obj->*method)(args);
		}
   }   
   void operator=(const DelegateTempl<ArgumentsType, ResultType, BaseType> &other) {
      obj = other.obj;
      method = other.method;
   }
   void operator=(int mustBeNull) {
		pAssert(mustBeNull == NULL);
      obj = NULL;
      method = NULL;
   }
   bool operator==(const DelegateTempl<ArgumentsType, ResultType, BaseType> &other) {
      return obj == other.obj && method == other.method;
   }
   bool operator==(int mustBeNull) {
		pAssert(mustBeNull == NULL);
      return obj == NULL;
   }
   bool operator!=(const DelegateTempl<ArgumentsType, ResultType, BaseType> &other) {
      return obj != other.obj && method != other.method;
   }
   bool operator!=(int mustBeNull) {
		pAssert(mustBeNull == NULL);
      return obj != NULL;
   }
};


template <typename ResultType, class BaseType>
class DelegateTempl<void, ResultType, BaseType> {
 public:
   typedef ResultType (BaseType::*MethodPtr)();
 private:
   BaseType *obj;
   MethodPtr method;
     
 public:
   DelegateTempl(const DelegateTempl<void, ResultType, BaseType> &other) {
      obj = other.obj;
      method = other.method;
   }
   DelegateTempl(int mustBeNull) {
		pAssert(mustBeNull == NULL);
      obj = NULL;
      method = NULL;
   }
   template <typename AnyMethodType>
   DelegateTempl(BaseType *obj, AnyMethodType method) {
      this->obj = obj;
      this->method = (MethodPtr)method;
   }
   ResultType operator()() {
      return (obj->*method)();
   }
   void callIfNotNull() {
		if (obj && method) {
			(obj->*method)();
		}
   }
   void operator=(const DelegateTempl<void, ResultType, BaseType> &other) {
      obj = other.obj;
      method = other.method;
   }
   void operator=(int mustBeNull) {
		pAssert(mustBeNull == NULL);
      obj = NULL;
      method = NULL;
   }
   bool operator==(const DelegateTempl<void, ResultType, BaseType> &other) {
      return obj == other.obj && method == other.method;
   }
   bool operator==(int mustBeNull) {
		pAssert(mustBeNull == NULL);
      return obj == NULL;
   }
   bool operator!=(const DelegateTempl<void, ResultType, BaseType> &other) {
      return obj != other.obj && method != other.method;
   }
   bool operator!=(int mustBeNull) {
		pAssert(mustBeNull == NULL);
      return obj != NULL;
   }
};

typedef DelegateTempl<void, void, Object> Delegate;
typedef DelegateTempl<void, void, QWidget> QDelegate;





#endif
