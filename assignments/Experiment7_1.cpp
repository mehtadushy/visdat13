//---------------------------------------------------------------------------
#include "stdafx.h"
#include <cmath>
//---------------------------------------------------------------------------
#include "Experiment7_1.h"
//---------------------------------------------------------------------------
#include "Properties.h"
#include "GLGeometryViewer.h"
#include "GeoXOutput.h"
//---------------------------------------------------------------------------

#include <limits>
#include "Field2.hpp"

// This is an implementation of LIC and Fast LIC for texture mapping of vector fields


IMPLEMENT_GEOX_CLASS( Experiment7_1, 0)
{
   BEGIN_CLASS_INIT( Experiment7_1 );

   ADD_SEPARATOR("Vector Field")
   ADD_STRING_PROP(vector_filename,0)
   ADD_SEPARATOR("Texture File")
   ADD_STRING_PROP(texture_filename,0)
   ADD_SEPARATOR("RANDOM TEXTURE")
   ADD_INT32_PROP(texture_seed,0)  
   ADD_FLOAT32_PROP(texture_mean,0)  
   ADD_SEPARATOR("Not exactly variance, but b-a")
   ADD_FLOAT32_PROP(texture_variance,0)  
   ADD_SEPARATOR("2^x * 2^x")
   ADD_INT32_PROP(texture_size,0)  
   ADD_SEPARATOR("Gray or BW")
   ADD_BOOLEAN_PROP(bw_texture,0)
   
   ADD_SEPARATOR("Kernel Length L [-L,L-1]")
   ADD_INT32_PROP(kernel_size,0)
   
   ADD_SEPARATOR("Colored Texture?")
   ADD_BOOLEAN_PROP(color_texture,0)
   
   ADD_SEPARATOR("Color Factor")
   ADD_FLOAT32_PROP(color_factor,0)
   
   ADD_NOARGS_METHOD(Experiment7_1::LoadVectorField)
   ADD_NOARGS_METHOD(Experiment7_1::LoadTexture)
   ADD_NOARGS_METHOD(Experiment7_1::GenerateRandomTexture)
   ADD_NOARGS_METHOD(Experiment7_1::ShowFlowLIC)
   ADD_NOARGS_METHOD(Experiment7_1::ShowFlowFastLIC)
	
}
QWidget* Experiment7_1::createViewer()  
{
    viewer = new GLGeometryViewer();
    return viewer;
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
Experiment7_1::Experiment7_1()           //Constructor
{
    viewer = NULL; 
    texture_seed=2;
    texture_mean=0.5;
    texture_variance=0.9;
    texture_size=8;
    kernel_size=15;
    max_steps=1000;
    field_val_tolerance = 0.00005;
    bw_thresh=0.3;
    color_texture=false;
    bw_texture=false;
    color_factor=0.1;
}


Experiment7_1::~Experiment7_1() {}
void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v );

void Experiment7_1::LoadVectorField()
{
    if (!field.load(vector_filename))
    {
        output << "Error loading field file " << vector_filename << "\n";
        return;
    }	

        //Get the minimum/maximum value in that field
    field_min_mag = std::numeric_limits<float32>::max();
    field_max_mag = -std::numeric_limits<float32>::max();
    for(size_t j=0; j<field.dims()[1]; j++)
    {
        for(size_t i=0; i< field.dims()[0]; i++)
        {
	    Vector2f vec=field.sample(i,j);
            float32 val = sqrtf(vec.getSqrNorm());
	    if (val > 0)
	    { 
	       field_min_mag = val < field_min_mag ? val : field_min_mag;
               field_max_mag = val > field_max_mag ? val : field_max_mag;
	    }
        }
    }
}


void Experiment7_1::GenerateRandomTexture()
{
   //Assuming a uniform distribution between 0 to RAND_MAX coming from RAND, we scale the distribution 
   //according to the input mean and variance
   float32 factor=texture_variance/RAND_MAX;
   //Clear texture_field
   texture_field.clear();
   texture_field.init(field.boundMin(),field.boundMax(),makeVector2ui(pow(2,texture_size),pow(2,texture_size)));

   srand(texture_seed);
   for(int i=0; i<texture_field.dims()[0]; i++)
   {
      for(int j=0; j<texture_field.dims()[1]; j++)
      {
	      if(bw_texture)
	      {
		      float32 val=rand()*factor - texture_variance/2 + texture_mean;
		      if(val<bw_thresh)
			      val=0;
		      else
			      val=1;
		      texture_field.setNodeScalar(i,j,val); //Black and white texture
	      }
	      else
		      texture_field.setNodeScalar(i,j,rand()*factor - texture_variance/2 + texture_mean);
      }
   }

   viewer->setTextureGray(texture_field.getData());
   viewer->refresh();
}

void Experiment7_1::LoadTexture()
{
   //Load the texture using Qt
    QImage image(texture_filename.c_str());

    //Get its (original) dimensions. Used as bounds later.
    const float fWidth = (float)image.width();
    const float fHeight = (float)image.height();

    //Resize to power-of-two and mirror.
    image = image.scaled(NextPOT(image.width()), NextPOT(image.height()));

    //Get its new integer dimensions.
    const int iWidth = image.width();
    const int iHeight = image.height();

        
        //Create one gray color channel represented as a scalar field
        texture_field.clear();
        texture_field.init(field.boundMin(),field.boundMax(), makeVector2ui(iWidth, iHeight));

        //Set the values at the vertices
        for(size_t j=0; j<texture_field.dims()[1]; j++)
        {
            for(size_t i=0; i<texture_field.dims()[0]; i++)
            {
                texture_field.setNodeScalar(i, j, (float)(qGray(image.pixel(i, j))) / 255.0 );
            }
        }

   viewer->setTextureGray(texture_field.getData());
   viewer->refresh();
} 

void Experiment7_1::ShowFlowLIC()
{
    //Find the step size by figuring out the shorter dimension of each pixel
    float32 x_size=(field.boundMax()[0]-field.boundMin()[0])/texture_field.dims()[0];
    float32 y_size=(field.boundMax()[1]-field.boundMin()[1])/texture_field.dims()[1];
    step_size=fmin(x_size,y_size);
    
    arc_length=step_size;
    
      ScalarField2 Red;
      ScalarField2 Blue;
      
    if(color_texture)
    {
      Red.init(field.boundMin(),field.boundMax(),texture_field.dims()/*makeVector2ui(pow(2,texture_size),pow(2,texture_size))*/);
      Blue.init(field.boundMin(),field.boundMax(),texture_field.dims()/*makeVector2ui(pow(2,texture_size),pow(2,texture_size))*/);
    }

    
    
    //Start at the 0,0 index of the texture, use the position to access the vector field.

   float32 X_inc= (field.boundMax()[0]-field.boundMin()[0])/(texture_field.dims()[0]-1);
   float32 Y_inc= (field.boundMax()[1]-field.boundMin()[1])/(texture_field.dims()[1]-1);
   
   for(int i=0; i<texture_field.dims()[0]; i++)
   {
      for(int j=0; j<texture_field.dims()[1]; j++)
      {
	      
	      float pixel_mag;
	      int count=0;
	      float pixel_value=0;
	      Vector2f XSeed= field.boundMin()+makeVector2f(X_inc*i,Y_inc*j);
	      if(color_texture)
	      {
		      Vector2f vec= field.sample(XSeed);
		      pixel_mag=sqrtf(vec.getSqrNorm())-field_min_mag;
		      pixel_mag/=(field_max_mag-field_min_mag);
		      //pixel_mag-=0.5;
		      pixel_mag*=color_factor;
		      
	      }
	      //Integrating forward
	      for(int k=0; k<kernel_size; k++)
	      {
	        pixel_value+=texture_field.sampleScalar(XSeed);
		count++;
		if(!rk4_forward(XSeed))
		{
			break;
		}
	      }
	      //pixel_valuef/=kernel_size;


	      XSeed= field.boundMin()+makeVector2f(X_inc*i,Y_inc*j);
	      //Integrating backward
	      for(int k=0; k<kernel_size; k++)
	      {
		if(!rk4_back(XSeed))
		{
			break;
		}
	        pixel_value+=texture_field.sampleScalar(XSeed);
		count++;
	      }
	      pixel_value/=count;

	      	      
	      if(color_texture)
	      {
	        float v=pixel_value;
		float s=0.0001+pixel_mag;
		float h=120;
		float r,g,b;
                HSVtoRGB( &r, &g, &b, h, s, v );
	      	      
	       Blue.setNodeScalar(i,j,b);
	       Red.setNodeScalar(i,j, r);
	       texture_field.setNodeScalar(i,j,g);
	      }
	      else
	      {
	          texture_field.setNodeScalar(i,j,pixel_value);
	      }

      }
   }
      if(!color_texture)
	      viewer->setTextureGray(texture_field.getData());
      else
	      viewer->setTextureRGB(Red.getData(), texture_field.getData(), Blue.getData());
      viewer->refresh();

}




void Experiment7_1::ShowFlowFastLIC()
{
    //Find the step size by figuring out the shorter dimension of each pixel
    float32 x_size=(field.boundMax()[0]-field.boundMin()[0])/texture_field.dims()[0];
    float32 y_size=(field.boundMax()[1]-field.boundMin()[1])/texture_field.dims()[1];
    step_size=fmin(x_size,y_size);
    
    arc_length=step_size;
    
    //Start at the 0,0 index of the texture, use the position to access the vector field.

   float32 X_inc= (field.boundMax()[0]-field.boundMin()[0])/(texture_field.dims()[0]-1);
   float32 Y_inc= (field.boundMax()[1]-field.boundMin()[1])/(texture_field.dims()[1]-1);
   
   for(int i=0; i<texture_field.dims()[0]; i++)
   {
      for(int j=0; j<texture_field.dims()[1]; j++)
      {
	      
	      int count=0;
	      float32 pixel_value=0;
	      Vector2f XSeed= field.boundMin()+makeVector2f(X_inc*i,Y_inc*j);
	      //Integrating forward
	      for(int k=0; k<kernel_size; k++)
	      {
	        pixel_value+=texture_field.sampleScalar(XSeed);
		count++;
		if(!rk4_forward(XSeed))
		{break;
		}
	      }
	      //pixel_valuef/=kernel_size;


	      //float32 pixel_valueb=0;
	      XSeed= field.boundMin()+makeVector2f(X_inc*i,Y_inc*j);
	      //Integrating backward
	      for(int k=0; k<kernel_size; k++)
	      {
		if(!rk4_back(XSeed))
		{ break;
		}
	        pixel_value+=texture_field.sampleScalar(XSeed);
		count++;
	      }
	      pixel_value/=count;

	      texture_field.setNodeScalar(i,j,pixel_value);
      }
   }
      viewer->setTextureGray(texture_field.getData());
      viewer->refresh();

}		      




bool Experiment7_1::rk4_forward(Vector2f &x1)
{
    float32 current_arc_length=0;
    for(int i=0; i<max_steps; i++)
    {

          if( !field.insideBounds(x1) )
	  {return false;
	  }
	   
          //Terminate execution if the field value is too low
          Vector2f Vec=field.sample(x1);
          if(Vec.getSqrNorm()< (field_val_tolerance* field_val_tolerance))
           return false;
	  bool arc_len_crossed=0;
          Vector2f v1,v2,v3,v4;
             v1=func_v(x1);
       v1.normalize();
             v2=func_v(x1 + v1*step_size/2);
       v2.normalize();
             v3=func_v(x1 + v2*step_size/2);
       v3.normalize();
             v4=func_v(x1 + v3*step_size);
       v4.normalize();
	  
	  Vector2f inc =   (v1/6 + v2/3 + v3/3 + v4/6) * step_size;
	  if((current_arc_length + sqrtf(inc.getSqrNorm())) > arc_length)
	  {
	      inc-=(v4/6) * step_size;
	      if((current_arc_length + sqrtf(inc.getSqrNorm())) > arc_length)
	      {
	         inc-=(v3/3) * step_size;
	         if((current_arc_length + sqrtf(inc.getSqrNorm())) > arc_length)
	         {
	            inc-=(v2/3) * step_size;
	            if((current_arc_length + sqrtf(inc.getSqrNorm())) > arc_length)
	            {
	                    arc_len_crossed=1;
	            }
	            else
	            {
	                    inc+=(v2/3)*step_size;
	                    arc_len_crossed=1;
	            }
	         }
	         else
	         {
	                 inc+=(v3/3)*step_size;
	                 arc_len_crossed=1;
	         }

	      }
	      else
	      {
		      inc+=(v4/6)*step_size;
		      arc_len_crossed=1;
	      }
	  }

             
          x1 += inc;
	  current_arc_length+=sqrtf(inc.getSqrNorm());
	  if(arc_len_crossed)
	return true;
    }
    
    return true;
}
bool Experiment7_1::rk4_back(Vector2f &x1)
{
    float32 current_arc_length=0;
    for(int i=0; i<max_steps; i++)
    {

          if( !field.insideBounds(x1) )
           return false;
          //Terminate execution if the field value is too low
          Vector2f Vec=field.sample(x1);
          if(Vec.getSqrNorm()< (field_val_tolerance* field_val_tolerance))
            return false;
	  bool arc_len_crossed=0;
          Vector2f v1,v2,v3,v4;
             v1=func_v(x1);
       v1.normalize();
             v2=func_v(x1 - v1*step_size/2);
       v2.normalize();
             v3=func_v(x1 - v2*step_size/2);
       v3.normalize();
             v4=func_v(x1 - v3*step_size);
       v4.normalize();
	  Vector2f inc =   (v1/6 + v2/3 + v3/3 + v4/6) * step_size;
	  if((current_arc_length + sqrtf(inc.getSqrNorm())) > arc_length)
	  {
	      inc-=(v4/6) * step_size;
	      if((current_arc_length + sqrtf(inc.getSqrNorm())) > arc_length)
	      {
	         inc-=(v3/3) * step_size;
	         if((current_arc_length + sqrtf(inc.getSqrNorm())) > arc_length)
	         {
	            inc-=(v2/3) * step_size;
	            if((current_arc_length + sqrtf(inc.getSqrNorm())) > arc_length)
	            {
	                    arc_len_crossed=1;
	            }
	            else
	            {
	                    inc+=(v2/3)*step_size;
	                    arc_len_crossed=1;
	            }
	         }
	         else
	         {
	                 inc+=(v3/3)*step_size;
	                 arc_len_crossed=1;
	         }

	      }
	      else
	      {
		      inc+=(v4/6)*step_size;
		      arc_len_crossed=1;
	      }
	  }

             
          x1 -= inc;
	  current_arc_length+=sqrtf(inc.getSqrNorm());
	  if(arc_len_crossed)
		 return true;
    }
    return true;
    
}



Vector2f Experiment7_1::func_v(Vector2f x)
{
   //Bilinear Interpolation of the vector field to get the field value at this point
   return field.sample(x);  	
}



// r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
//		if s == 0, then h = -1 (undefined)


void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v )
{
	int i;
	float f, p, q, t;

	if( s == 0 ) {
		// achromatic (grey)
		*r = *g = *b = v;
		return;
	}

	h /= 60;			// sector 0 to 5
	i = floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );

	switch( i ) {
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;
		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;
		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;
		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;
		default:		// case 5:
			*r = v;
			*g = p;
			*b = q;
			break;
	}

}


//  /home/dushy/Desktop/VisDat13/Assignments/Assignment07/Data/Cylinderclose2CT10.am

//   /home/dushy/Desktop/VisDat13/Assignments/Assignment07/Data/Sink.am
