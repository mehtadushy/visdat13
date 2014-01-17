//---------------------------------------------------------------------------
#ifndef ClassMethodsH
#define ClassMethodsH
//---------------------------------------------------------------------------
#include "Object.h"
//---------------------------------------------------------------------------


class ClassProperty;

class  ClassMethod : public Object {
 private:
	string name;
   string description;
   string icon;
   bool visible;
   
 public:
   ClassMethod(string name, string description = "", string icon = "", bool visible = true);
   string getName() const;
   string getDescription() const;
   string getIcon() const;
   bool getVisible() const;
   virtual void call(Object *obj) const = 0;
   virtual ClassMethod *copyMethod() const = 0;
};


/// representing a function with no arguments
class  NoArgsClassMethod : public ClassMethod {
 public:
   typedef void (Object::* MethodPointer)();

 private:
   MethodPointer method;

 public:
   NoArgsClassMethod(MethodPointer method, string name, string description = "", string icon = "", bool visible = true);
   virtual void call(Object *obj) const;
   virtual ClassMethod *copyMethod() const;
};


// /// Representing a general method with parameters, reference parameters and return value.
// /// Concept: Parameters and return values are described by a ClassProperty (use StructClassProperty for multiple values).
// /// Then the method can be invoked by call; arguments are read from a plain memory buffer (the layout of which is described by
// /// the properties). Results are written to a similar buffer. Use the class property mechanisms to parse input and output.
// class  GeneralClassMethod : public ClassMethod {
//  public:
// 	/// a proxy method for calling the method - non member to facilitate automatic creation via macros
//    typedef void (*CallingFunctionPtr)(Object *thisPtr, ClassProperty *parameters, ClassProperty *result);
// 	/// buffer for method parameters
//    typedef void *ParameterBuffer;
//  
//  private:
// 	ClassProperty *parameters;
// 	ClassProperty *result;
// 
//  public:
// 	/// constructor - use GENERAL_CLASS_METHOD macro as simplified interface
// 	GeneralClassMethod(CallingFunctionPtr caller,
// 							 ClassProperty *parameters, ClassProperty *result, 
// 	                   string name, string description = "", string icon = "");
// 	
//    virtual void call(ParameterBuffer params, ParameterBuffer result) const;
//    virtual ClassMethod *copyMethod() const;
// 
// 	/// allocate (and zero-initialize) a memory block storing method parameters
// 	virtual ParameterBuffer createParameterBuffer();
// 	/// allocate (and zero-initialize) a memory block storing method return values
// 	virtual ParameterBuffer createResultBuffer();
// 	/// free buffer memory again
// 	virtual void deleteBuffer(ParameterBuffer buf);
// };


typedef NoArgsClassMethod::MethodPointer NoArgsMethodPointer;


#define ADD_NOARGS_METHOD_EX( method, name, desc, icon, visible ) \
	((MetaClass*)getClass())->addMethod( new NoArgsClassMethod( \
		(NoArgsMethodPointer) & method , name, desc, icon, visible ) );

#define ADD_NOARGS_METHOD( method )							ADD_NOARGS_METHOD_EX ( method, #method, "", "", true )
#define ADD_NOARGS_METHOD_DESC( method, desc )			ADD_NOARGS_METHOD_EX ( method, #method, desc, "", true )
#define ADD_NOARGS_METHOD_ICON( method, desc, icon )	ADD_NOARGS_METHOD_EX ( method, #method, desc, icon, true )
#define ADD_NOARGS_METHOD_INVISIBLE( method )			ADD_NOARGS_METHOD_EX ( method, #method, "", "", false )



#endif
