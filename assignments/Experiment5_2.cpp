//---------------------------------------------------------------------------
#include "stdafx.h"
#include <cmath>
//---------------------------------------------------------------------------
#include "Experiment5_2.h"
//---------------------------------------------------------------------------
#include "Properties.h"
#include "GLGeometryViewer.h"
#include "GeoXOutput.h"
//---------------------------------------------------------------------------

#include <limits>
#include "Field2.hpp"

IMPLEMENT_GEOX_CLASS( Experiment5_2, 0)
{
    BEGIN_CLASS_INIT( Experiment5_2 );

	ADD_SEPARATOR("Vectorfield")
    ADD_STRING_PROP(VectorfieldFilename, 0)
    ADD_FLOAT32_PROP(ArrowScale, 0)
    ADD_NOARGS_METHOD(Experiment5_2::LoadVectorField)
	ADD_NOARGS_METHOD(Experiment5_2::draw_stream_rk4)
    ADD_NOARGS_METHOD(Experiment5_2::DrawVectorField)
	
    ADD_SEPARATOR("Start Position")
	ADD_FLOAT32_PROP(XStart, 0)
	ADD_FLOAT32_PROP(YStart, 0)

	ADD_SEPARATOR("RK4 PARAMETERS")
    ADD_FLOAT32_PROP(step_size,0)
   
	ADD_SEPARATOR("Stream Alpha")
    ADD_FLOAT32_PROP(stream_alpha,0)

	ADD_SEPARATOR("Direction")
    ADD_BOOLEAN_PROP(direction,0)
	
	ADD_SEPARATOR("Field Type")
    ADD_BOOLEAN_PROP(field_type,0)

	ADD_SEPARATOR("Max Steps")
    ADD_INT32_PROP(max_steps,0)

	ADD_SEPARATOR("Arc Length")
    ADD_FLOAT32_PROP(arc_length,0)

	ADD_SEPARATOR("Velocity Tolerance")
    ADD_FLOAT32_PROP(velocity_tolerance,0)	
	
}

Experiment5_2::Experiment5_2()
{
	direction=true;
	velocity_tolerance=0.005;
    viewer = NULL;
    ScalarfieldFilename = "";
    VectorfieldFilename = "";
    ArrowScale = 0.1;
	stream_alpha=0.5;
    ImageFilename = "";
    bColoredTexture = true;
	field_type=true;
	XStart=-0.2;
	YStart=0;
	stream_width=1;
	step_size=0.01;
	max_steps=1000;
	arc_length=1.0;
    arc_length_change=0;
	stream_color=makeVector4f(1,0,0,stream_alpha);
}

void Experiment5_2::LoadVectorField()
{
    if (!field.load(VectorfieldFilename))
    {
        output << "Error loading field file " << VectorfieldFilename << "\n";
        return;
    }	
}

Vector2f Experiment5_2::rk4_forward(Vector2f x1)
{
    Vector2f x2,v1,v2,v3,v4;
    if(field_type==true) //Using the actual vector field
    {
       v1=func_v(x1);
       v2=func_v(x1 + v1*step_size/2);
       v3=func_v(x1 + v2*step_size/2);
       v4=func_v(x1 + v3*step_size);
       stream_color=makeVector4f(0,0,1,stream_alpha);
       stream_width=1;
    }
    else       //Using the direction field
    {
       v1=func_v(x1);
       v1.normalize();
       v2=func_v(x1 + v1*step_size/2);
       v2.normalize();
       v3=func_v(x1 + v2*step_size/2);
       v3.normalize();
       v4=func_v(x1 + v3*step_size);
       v4.normalize();
       stream_color=makeVector4f(0,1,0,stream_alpha);
       stream_width=3;
    }
       
	Vector2f Vec=(v1/6 + v2/3 + v3/3 + v4/6) * step_size;
    x2= x1 + Vec;
    viewer->addLine(x1,x2,stream_color,stream_width);
	arc_length_change += sqrtf(Vec.getSqrNorm());
	return x2;
}

Vector2f Experiment5_2::rk4_backward(Vector2f x1)
{
    Vector2f x2,v1,v2,v3,v4;
    if(field_type==true) //Using the actual vector field
    {
       v1=func_v(x1);
       v2=func_v(x1 - v1*step_size/2);
       v3=func_v(x1 - v2*step_size/2);
       v4=func_v(x1 - v3*step_size);
       stream_color=makeVector4f(0,0,1,stream_alpha);
       stream_width=1;
    }
    else       //Using the direction field
    {
       v1=func_v(x1);
       v1.normalize();
       v2=func_v(x1 - v1*step_size/2);
       v2.normalize();
       v3=func_v(x1 - v2*step_size/2);
       v3.normalize();
       v4=func_v(x1 - v3*step_size);
       v4.normalize();
       stream_color=makeVector4f(0,1,0,stream_alpha);
       stream_width=3;
    }
       
	Vector2f Vec=(v1/6 + v2/3 + v3/3 + v4/6) * step_size;
    x2= x1 - Vec;
	viewer->addLine(x1,x2,stream_color,stream_width);
    arc_length_change += sqrtf(Vec.getSqrNorm());
	return x2;
}

void Experiment5_2::draw_stream_rk4()
{
	viewer -> clear();
	arc_length_change=0;
	Vector2f X_Seed;
    X_Seed[0]=XStart;
	X_Seed[1]=YStart;
	Point2D SeedPt;
	SeedPt.position=X_Seed;
	SeedPt.color=makeVector4f(1,1,0,1);
	viewer->addPoint(SeedPt);     
    for(int i=0; i<max_steps; i++)
      {
           if(direction == true)  //Forward
              X_Seed=rk4_forward(X_Seed);
           else                  //Backward
              X_Seed=rk4_backward(X_Seed);

          //Terminate execution on hitting the domain boundaries
          if( (X_Seed[0] > (field.boundMax()[0]-0.0005)) || (X_Seed[0] < (field.boundMin()[0]+0.0005)) || (X_Seed[1] > (field.boundMax()[1]-0.0005)) || (X_Seed[1] < (field.boundMin()[1] + 0.0005)) )
           break;
          //Terminate execution if the field value is too low
          Vector2f Vec=field.sample(X_Seed);
		  float field_val_tolerance = 0.00005;
          if(sqrtf(Vec.getSqrNorm())< (field_val_tolerance))
           break;
		  //appear on interface
		  if(sqrtf(Vec.getSqrNorm())< (velocity_tolerance))
           break;
		  //Terminate after selected arc length
		  if(arc_length_change>arc_length)
			  break;
     }
     viewer->refresh();
}

Vector2f Experiment5_2::func_v(Vector2f x)
{
   //Bilinear Interpolation of the vector field to get the field value at this point
   return field.sample(x);  	
}


QWidget* Experiment5_2::createViewer()
{
    viewer = new GLGeometryViewer();
    return viewer;
}

Experiment5_2::~Experiment5_2() {}

void Experiment5_2::DrawVectorField()
{
    viewer->clear();

    //Load the vector field
    VectorField2 field;
    if (!field.load(VectorfieldFilename))
    {
        output << "Error loading field file " << VectorfieldFilename << "\n";
        return;
    }

    //Draw vector directions (constant length)
    for(float32 x=field.boundMin()[0]; x<=field.boundMax()[0]; x+=0.2)
    {
		
        for(float32 y=field.boundMin()[1]; y<=field.boundMax()[1]; y+=0.2)
        {
		    Vector2f vec = field.sample(x,y);
            vec.normalize();

            viewer->addLine(x, y, x + ArrowScale*vec[0], y + ArrowScale*vec[1]);
        }
    }
	Vector2f x1 = makeVector2f(XStart,YStart);
	Point2D p1; 
    p1.position=x1;
	p1.color=makeVector4f(0,1,0,0.7);
	p1.size=3;
	viewer -> addPoint(p1); 
    viewer->refresh();
}


namespace
{
    ///Returns the next power-of-two
    int32 NextPOT(int32 n)
    {
        n--;
        n |= n >> 1;   // Divide by 2^k for consecutive doublings of k up to 32,
        n |= n >> 2;   // and then or the results.
        n |= n >> 4;
        n |= n >> 8;
        n |= n >> 16;
        n++;           // The result is a number of 1 bits equal to the number
                       // of bits in the original number, plus 1. That's the
                       // next highest power of 2.
        return n;
    }
}

