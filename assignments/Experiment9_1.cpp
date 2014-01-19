//---------------------------------------------------------------------------
#include "stdafx.h"
#include <cmath>
//---------------------------------------------------------------------------
#include "Experiment9_1.h"
//---------------------------------------------------------------------------
#include "Properties.h"
#include "GLGeometryViewer.h"
#include "GeoXOutput.h"
//---------------------------------------------------------------------------

#include <limits>
#include "Field2.hpp"

// Feature extraction from 2D Vector Fields / Topology of 2D Vector Fields



IMPLEMENT_GEOX_CLASS( Experiment9_1, 0)
{
   BEGIN_CLASS_INIT( Experiment9_1 );
   ADD_SEPARATOR("Vector Field")
   ADD_STRING_PROP(vector_filename,0)
   ADD_SEPARATOR("Scalar Field")
   ADD_STRING_PROP(scalar_filename,0)   
   ADD_SEPARATOR("---")
   ADD_SEPARATOR("LIC Texture")
   ADD_INT32_PROP(texture_seed,0)  
   ADD_FLOAT32_PROP(texture_mean,0)  
   ADD_SEPARATOR("b-a")
   ADD_FLOAT32_PROP(texture_variance,0)  
   ADD_SEPARATOR("2^x * 2^x")
   ADD_INT32_PROP(texture_size,0)  
   ADD_SEPARATOR("Kernel Length L [-L,L-1]")
   ADD_INT32_PROP(kernel_size,0)
   ADD_SEPARATOR("---")
   ADD_SEPARATOR("Streamline Seeding")
   ADD_INT32_PROP(num_seeds,0) 
   ADD_FLOAT32_PROP(stream_alpha,0)
   ADD_SEPARATOR("---")
   ADD_SEPARATOR("Display Properties")
   ADD_BOOLEAN_PROP(persist_on,0)

   
   ADD_NOARGS_METHOD(Experiment9_1::LoadVectorField)        //Load vector field and reset "critical_pt_avail" flag
   ADD_NOARGS_METHOD(Experiment9_1::LoadScalarField)        //Load scalar field, compute its gradient and reset "critical_pt_avail" flag
   ADD_NOARGS_METHOD(Experiment9_1::ShowCriticalPoints)     //If critical points have not been computed, compute them, set the
                                                            //"critical_pt_avail" flag and display the points. Otherwise just
                                                            //display the points.
   //ADD_NOARGS_METHOD(Experiment9_1::ShowSeparatrices)       //If critical points have not been computed, compute them, set 
                                                            //the"critical_pt_avail" flag and compute & display the separatrices.
                                                            //Otherwise just compute and show the separatrices 
   ADD_NOARGS_METHOD(Experiment9_1::GenerateRandomTexture)  //Generate a random texture for LIC
   ADD_NOARGS_METHOD(Experiment9_1::ShowFlowLIC)            //Display the result of LIC
   //ADD_NOARGS_METHOD(Experiment9_1::DrawStreamRandom)       //Draw streamlines seeded by a random distribution of points
	
}
QWidget* Experiment9_1::createViewer()  
{
    viewer = new GLGeometryViewer();
    return viewer;
}

Experiment9_1::Experiment9_1()           //Constructor
{
    viewer = NULL; 
    max_steps=1000;
    field_val_tolerance = 0.00005;

    //LIC Related Stuff
    bw_thresh=0.3;
    bw_texture=false;
    texture_seed=2;
    texture_mean=0.5;
    texture_variance=0.9;
    texture_size=8;
    kernel_size=15;

}


Experiment9_1::~Experiment9_1() {}
void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v );

void Experiment9_1::LoadVectorField()
{
    if (!field.load(vector_filename))
    {
        output << "Error loading field file " << vector_filename << "\n";
        return;
    }	

    critical_pt_avail = false;

}

void Experiment9_1::LoadScalarField()
{
    ScalarField2 scalar_field;
    if (!scalar_field.load(scalar_filename))
    {
        output << "Error loading scalar field file " << scalar_filename << "\n";
        return;
    }	

    //Initialise the vector field to have the same dimensions as the scalar field.
    field.init(scalar_field.boundMin(),scalar_field.boundMax(),scalar_field.dims());
    //Compute the gradient of the scalar field and populate the vector field with the gradients
    
    for(int i=0; i<scalar_field.dims()[0]; i++)
    {
       for(int j=0; j<scalar_field.dims()[1]; j++)
       {
           field.setNode(i,j,scalar_field.sampleGradient(scalar_field.nodePosition(i,j)));
       }
    }
    critical_pt_avail = false;
}


void Experiment9_1::ShowCriticalPoints()
{
   if(persist_on == false)
   {
     viewer->clear();
   }

   if(critical_pt_avail ==false)
   {
     get_critical_points();
     classify_critical_points();
   }

   //Now we come down to the display business of critical points
   //Assuming the critical point enumeration is as follows: source, repelling focus, saddle, center, sink, attracting focus
   //Defining colors as red, orange, yellow, green, blue, violet
   vector <Vector4f> cp_color;
   float cp_alpha = 0.7;
   cp_color.resize(6);
   cp_color[0]=makeVector4f(1,0,0,cp_alpha);     //Red   
   cp_color[1]=makeVector4f(1,0.2,0,cp_alpha);   //Orange
   cp_color[2]=makeVector4f(1,1,0,cp_alpha);     //Yellow
   cp_color[3]=makeVector4f(0,1,0,cp_alpha);     //Green
   cp_color[4]=makeVector4f(0,0,1,cp_alpha);     //Blue
   cp_color[5]=makeVector4f(.5,.1,.9,cp_alpha);  //Violet

   //Iterate through the vector containing critical points and plot them
   //for(std::vector<Vector3f>::iterator it = CriticalPoints.begin();
   //     	   it != CriticalPoints.end(); ++it)
   //{
   //           Point2D CrPt;
   //           CrPt.position=makeVector2f((*it)[0],(*it)[1]);    //Read X and Y coordinate out
   //           CrPt.size=10.0;
   //           CrPt.color=cp_color[<int>((*it)[2])];               //Get color from the critical point classification
   //           viewer->addPoint(CrPt); 
   //}
   for(int i=0; i<CriticalPoints.size(); i++)
   {
	   if((static_cast<int>(CriticalPoints[i][2])) != 10)
	   {
              Point2D CrPt;
              CrPt.position=makeVector2f(CriticalPoints[i][0],CriticalPoints[i][1]);    //Read X and Y coordinate out
              CrPt.size=10.0;
              CrPt.color=cp_color[static_cast<int>(CriticalPoints[i][2])];               //Get color from the critical point classification
              viewer->addPoint(CrPt); 
	   }
   }  
   viewer->refresh();
}

void Experiment9_1::get_critical_points()
{
   //Go from cell to cell in the vector field, doing the sign test. If critical point exists in 
   // a cell, find it using bisection/domain decomposition

   //Cells are being addressed by the top left corner

   for(int i=0; i<field.dims()[0]-1; i++)
   {
      for(int j=0; j<field.dims()[1]-1; j++)
      {
	   vector <Vector2f>  bound_box(2, makeVector2f(0,0));
	   bound_box[0]= field.nodePosition(i,j);
	   bound_box[1]= field.nodePosition(i+1,j+1);

	   vector <vector<Vector2f> >  OpenNodes; //A maximum of 4 critical points may exist in a cell
	   OpenNodes.resize(4);

	   if(sign_test(bound_box))
	   {   OpenNodes.push_back(bound_box);
	   }

	   while(!OpenNodes.empty())
	   {
	     bound_box=OpenNodes.back();
	     OpenNodes.pop_back();
             
	     //If the x and y difference is below a certain threshold, mark it a critical point 
	     //and don't bisect further.
	     float xdiff, ydiff;
	     xdiff=bound_box[1][0]-bound_box[0][0];
	     ydiff=bound_box[1][1]-bound_box[0][1];

	     float thresh=1e-6;

	     if( (fabs(xdiff)<thresh) && (fabs(ydiff)<thresh))
	     {
	       CriticalPoints.push_back(makeVector3f(bound_box[0][0],bound_box[0][1],0));	     
	     }
	     else
	     {
	        vector <Vector2f>  temp_bound_box(2, makeVector2f(0,0));

		temp_bound_box[0]=bound_box[0];
		temp_bound_box[1]=makeVector2f(bound_box[0][0]+ xdiff/2.0, bound_box[0][1]+ ydiff/2.0);
	        if(sign_test(temp_bound_box))
	        {   OpenNodes.push_back(temp_bound_box);
	        }
		
		temp_bound_box[0]=makeVector2f(bound_box[0][0],bound_box[0][1]+ydiff/2.0);
		temp_bound_box[1]=makeVector2f(bound_box[0][0]+ xdiff/2.0, bound_box[1][1]);
	        if(sign_test(temp_bound_box))
	        {   OpenNodes.push_back(temp_bound_box);
	        }
		
		temp_bound_box[1]=bound_box[1];
		temp_bound_box[0]=makeVector2f(bound_box[0][0]+ xdiff/2.0, bound_box[0][1]+ ydiff/2.0);
	        if(sign_test(temp_bound_box))
	        {   OpenNodes.push_back(temp_bound_box);
	        }
				
		temp_bound_box[0]=makeVector2f(bound_box[0][0]+xdiff/2.0,bound_box[0][1]);
		temp_bound_box[1]=makeVector2f(bound_box[1][0], bound_box[0][1]+ ydiff/2.0);
	        if(sign_test(temp_bound_box))
	        {   OpenNodes.push_back(temp_bound_box);
	        }
	            
	     }

	   }

	   critical_pt_avail = true; //Mark that critical points are available

      }
   }

}

bool Experiment9_1::sign_test(vector <Vector2f> bound_box)   //Pass the bounding coordinates
{
       int posu=0, negu=0,posv=0,negv=0;
       for(int i=0; i<2; i++)
        for(int j=0; j<2; j++)
        {
	   Vector2f field_sample;
	   Vector2f NodePos;
	   NodePos = makeVector2f(bound_box[i][0], bound_box[j][1]);
	   field_sample=field.sample(NodePos);
	     if(field_sample[0]>=0)
	     {  posu++;
	     }
	     else
	     { negu++;
	     }
	     if(field_sample[1]>=0)
	     {  posv++;
	     }
	     else
	     { negv++;
	     }
	}

       return( ((posu*negu)!=0) && ((posv*negv)!=0) );
  
}

void Experiment9_1::classify_critical_points()
{
   
   //enum cp_type {source, repel_focus, saddle, center, sink, attract_focus}; 
    
   //Cycle through the list of critical points and check the jacobian eigenvalues for the points
   for(int i=0; i<CriticalPoints.size(); i++)
   {
              Vector2f CrPt;
              CrPt=makeVector2f(CriticalPoints[i][0],CriticalPoints[i][1]);    //Read X and Y coordinate out
              
	      //Get Jacobian and Eigenvalues
	      Vector2f RealEigen;
	      Vector2f ImEigen;
	      Matrix2f EigenValues;

	      Matrix2f field_jacobian;
	      field_jacobian = field.sampleJacobian(CrPt);

	      if(fabs(field_jacobian.getDeterminant()) < 10e-8)  // If the determinant of the jacobian is rather small
	      {
                CriticalPoints[i][2]=10;
	      }
	      else
	      {
	        field_jacobian.solveEigenProblem(RealEigen,ImEigen,EigenValues);

	        //Classify the point
		if( (RealEigen[0]>0) && (RealEigen[1]>0) && (fabs(ImEigen[0]) < 10e-8) && (fabs(ImEigen[1]) < 10e-8) ) //Source
                 CriticalPoints[i][2]=0;
		else if( (RealEigen[0]<0) && (RealEigen[1]<0) && (fabs(ImEigen[0]) < 10e-8) && (fabs(ImEigen[1]) < 10e-8) ) //Sink
                 CriticalPoints[i][2]=4;
		else if( ((RealEigen[0]*RealEigen[1])<0) && (fabs(ImEigen[0]) < 10e-8) && (fabs(ImEigen[1]) < 10e-8) ) //Saddle
                 CriticalPoints[i][2]=2;
		else if( (fabs(RealEigen[0]-RealEigen[1])<10e-8) && (RealEigen[0]>0) && (fabs(ImEigen[0]) > 10e-8) && (fabs(ImEigen[1]+ImEigen[1]) < 10e-8) ) //Repelling focus
                 CriticalPoints[i][2]=1;
		else if( (fabs(RealEigen[0]-RealEigen[1])<10e-8) && (RealEigen[0]< 0) && (fabs(ImEigen[0]) > 10e-8) && (fabs(ImEigen[1]+ImEigen[1]) < 10e-8) ) //Attracting focus
                 CriticalPoints[i][2]=5;
		else if( (fabs(RealEigen[0]) < 10e-8) && (fabs(RealEigen[1]) < 10e-8) && (fabs(ImEigen[0]) > 10e-8) && (fabs(ImEigen[1]+ImEigen[1]) < 10e-8) ) //Center
                 CriticalPoints[i][2]=3;
		else
                  CriticalPoints[i][2]=10;
	      }

   }  

}

void Experiment9_1::GenerateRandomTexture()
{

     viewer->clear();
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

void Experiment9_1::ShowFlowLIC()
{
     viewer->clear();
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
	      
	      float pixel_mag;
	      int count=0;
	      float pixel_value=0;
	      Vector2f XSeed= field.boundMin()+makeVector2f(X_inc*i,Y_inc*j);

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

	      	      
	      texture_field.setNodeScalar(i,j,pixel_value);

      }
   }
	      
   viewer->setTextureGray(texture_field.getData());

   viewer->refresh();

}



bool Experiment9_1::rk4_forward(Vector2f &x1)
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
bool Experiment9_1::rk4_back(Vector2f &x1)
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



Vector2f Experiment9_1::func_v(Vector2f x)
{
   //Bilinear Interpolation of the vector field to get the field value at this point
   return field.sample(x);  	
}




//  /home/dushy/Desktop/VisDat13/Assignments/Assignment07/Data/Cylinderclose2CT10.am

//   /home/dushy/Desktop/VisDat13/Assignments/Assignment07/Data/Sink.am
