//--------------------------------------------- // 
#include "stdafx.h"                             // <--- this include is necessary in order to use precompiled headers in visual studio. 
//--------------------------------------------- //      it must be the first commmand in each cpp file
#include "ExampleExperiment.h"                  // <--- include your header file
//--------------------------------------------- // 
#include "Properties.h"                         // <--- this include file will import all properties available (used below)
#include "GeoXOutput.h"                         // <--- this include lets you output text in the main window
//--------------------------------------------- //
                                                //
                                                //
IMPLEMENT_GEOX_CLASS( ExampleExperiment, 0 )    // <--- names the class, sets version number to zero.
{                                               //      increment later if you add properties and still want to read old example files
   BEGIN_CLASS_INIT( ExampleExperiment );       // <--- starts the properties declartion

   ADD_SEPARATOR("Numerical Values")            // <--- adds a separator - looks cleaner ;-)
   ADD_INT32_PROP( numberA, 0 )                 // <--- in version 0 of the class, there is an int32 named numberA
   ADD_FLOAT32_PROP( numberB, 0 )               // <--- in version 0 of the class, there is also an float32 named numberB
                                                //
   ADD_SEPARATOR("Vectors & Matrices")          // 
   ADD_VECTOR3F_PROP(vector1, 0)                // <---
   ADD_VECTOR3F_PROP(vector2, 0)                // <--- vectors & matrices in version 0...
   ADD_MATRIX3F_PROP(matrix, 0)                 // <---
                                                //
   ADD_SEPARATOR("Other Stuff")                 //
   ADD_STRING_PROP(someString, 0)               // <--- other types
   ADD_BOOLEAN_PROP(someBoolean, 0)             // <---
                                                //
   ADD_NOARGS_METHOD(ExampleExperiment::computeAPlusB)             // <--- register method 1   (no version required, there is no data to be stored)
   ADD_NOARGS_METHOD(ExampleExperiment::crossProduct)              // <--- register method 2
   ADD_NOARGS_METHOD(ExampleExperiment::matrixVectorProduct)       // <--- register method 3
}                                               //
                                                //
                                                // remark: do not forget to register your class in InitGeoX.cpp
                                                //         (have a look at that file for details)
                                                //
                                                //
ExampleExperiment::ExampleExperiment()          // <--- constructor - set reasonable default parameters
{                                               //
   // important number                          //
   numberA = 42;                                //
   // suspicious number (float)                 //
   numberB = 23.0f;                             //
   // example vector -- x-axis                  //
   vector1 = makeVector3f(1,0,0);               //
   // example vector -- y-axis                  //
   vector2 = makeVector3f(0,1,0);               //
   // rotation by 90°                           //
   matrix = makeMatrix3f(0,-1, 0,               //
                         1, 0, 0,               //
                         0, 0, 1);              //
}                                               //
                                                //
ExampleExperiment::~ExampleExperiment()         // <--- destructor - often empty for our experiments
{                                               //
}                                               //
                                                //
void ExampleExperiment::computeAPlusB()         // <--- first experiment
{                                               //
   output << "Adding numberA (" << numberA      //      just provide some code
          << ") and numberB (" << numberB       //
          << ") yields: " << numberA+numberB    //
          << "\n";                              //
}                                               //
                                                //
void ExampleExperiment::crossProduct()          // <--- second experiment
{                                               //
   output << "the crossproduct of vector1 and " //
     << "vector 2 is: "                         //
     << vector1.crossProduct(vector2).toString()//
     << "\n";                                   //
}                                               //
                                                // 
void ExampleExperiment::matrixVectorProduct()   // <--- third experiment
{                                               //
   output << "matrix * vector1 leads to...\n"   //
     << (matrix*vector1).toString()             //
     << "\n";                                   //
}                                               //

