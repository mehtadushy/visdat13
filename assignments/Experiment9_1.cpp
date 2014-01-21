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
   ADD_SEPARATOR("RK4 Properties")
   ADD_INT32_PROP(max_steps,0)  
   ADD_FLOAT32_PROP(field_val_tolerance,0)  


   
   ADD_NOARGS_METHOD(Experiment9_1::LoadVectorField)        //Load vector field and reset "critical_pt_avail" flag
   ADD_NOARGS_METHOD(Experiment9_1::LoadScalarField)        //Load scalar field, compute its gradient and reset "critical_pt_avail" flag
   ADD_NOARGS_METHOD(Experiment9_1::ShowCriticalPoints)     //If critical points have not been computed, compute them, set the
                                                            //"critical_pt_avail" flag and display the points. Otherwise just
                                                            //display the points.
   ADD_NOARGS_METHOD(Experiment9_1::ShowSeparatrices)       //If critical points have not been computed, compute them, set 
                                                            //the"critical_pt_avail" flag and compute & display the separatrices.
                                                            //Otherwise just compute and show the separatrices 
   ADD_NOARGS_METHOD(Experiment9_1::GenerateRandomTexture)  //Generate a random texture for LIC
   ADD_NOARGS_METHOD(Experiment9_1::ShowFlowLIC)            //Display the result of LIC
   ADD_NOARGS_METHOD(Experiment9_1::DrawStreamModulated)       //Draw streamlines seeded by a random distribution of points
   ADD_NOARGS_METHOD(Experiment9_1::OverlayGrid)
	
}
QWidget* Experiment9_1::createViewer()  
{
    viewer = new GLGeometryViewer();
    return viewer;
}

Experiment9_1::Experiment9_1()           //Constructor
{
    viewer = NULL; 

    //RK4 Stuff
    max_steps=1000;
    field_val_tolerance = 0.00005;
      //rk4 internal - get changed internally
    arc_length=2;
    step_size=0.01;

    //Internal flags
    critical_pt_avail=false;
    draw_stream=false;  


    //LIC Related Stuff
    bw_thresh=0.3;
    bw_texture=false;
    texture_seed=2;
    texture_mean=0.5;
    texture_variance=0.9;
    texture_size=8;
    kernel_size=15;

    //Streamline related stuff
    show_seeds=false;
    num_seeds=100;
    stream_alpha=0.7;
       //stream stuff for internal use - get modified internally
    stream_color=makeVector4f(1,1,1,stream_alpha);
    stream_width=3;

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
           //There is a bug in the sampleGradient function. Use your own derivative function 
	   //or wait till the sampleGradient function is fixed
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
   }

   //Now we come down to the display business of critical points
   //Assuming the critical point enumeration is as follows: source, repelling focus, saddle, center, sink, attracting focus
   //Defining colors as red, orange, yellow, green, blue, violet
   vector <Vector4f> cp_color;
   float cp_alpha = 0.7;
   cp_color.resize(8);
   cp_color[0]=makeVector4f(1,0,0,cp_alpha);     //Red   
   cp_color[1]=makeVector4f(1,0.2,0,cp_alpha);   //Orange
   cp_color[2]=makeVector4f(1,1,0,cp_alpha);     //Yellow
   cp_color[3]=makeVector4f(0,1,0,cp_alpha);     //Green
   cp_color[4]=makeVector4f(0,0,1,cp_alpha);     //Blue
   cp_color[5]=makeVector4f(.5,.1,.9,cp_alpha);  //Violet
   cp_color[6]=makeVector4f(1,1,1,cp_alpha);     //White
   cp_color[7]=makeVector4f(.4,1,.5,cp_alpha);     //

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
        
   output << "Preparing to display critical points ...\n";
   output << CriticalPoints.size() <<"\n";

   for(int i=0; i<CriticalPoints.size(); i++)
   {
	   if((static_cast<int>(CriticalPoints[i][2])) != 10)
	   {
              Point2D CrPt;
              CrPt.position=makeVector2f(CriticalPoints[i][0],CriticalPoints[i][1]);    //Read X and Y coordinate out
              CrPt.size=8.0;
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
   output<<"Computing Critical Points ...\n";

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

	     float32 thresh=1e-6;  //Threshold to terminate the search for critical point

	     if( (fabs(xdiff)<thresh) && (fabs(ydiff)<thresh))
	     {
               int CrPt_Class;
	       CrPt_Class=classify_critical_points(bound_box[0]);
	       if(CrPt_Class != 10)
	       {
	          CriticalPoints.push_back(makeVector3f(bound_box[0][0],bound_box[0][1],CrPt_Class));
	       }	     
	     }
	     else
	     {
	        vector <Vector2f>  temp_bound_box(2, makeVector2f(0,0)); //Contains the 4 subregions of the domain

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


      }
   }
	   critical_pt_avail = true; //Mark that critical points are available
	   output<<"Done computing critical points\n";

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

int Experiment9_1::classify_critical_points(const Vector2f & CrPt)
{
   
   //enum cp_type {source, repel_focus, saddle, center, sink, attract_focus}; 
    
              
	      //Get Jacobian and Eigenvalues
	      Vector2f RealEigen;
	      Vector2f ImEigen;
	      Matrix2f EigenValues;

	      Matrix2f field_jacobian;
	      field_jacobian = field.sampleJacobian(CrPt);

	      if(fabs(field_jacobian.getDeterminant()) < 1e-7)  // If the determinant of the jacobian is rather small
	      {
                return 10;
	      }
	      else
	      {
	      field_jacobian.solveEigenProblem(RealEigen,ImEigen,EigenValues);

	      //Classify the point
	      if( (RealEigen[0]>0) && (RealEigen[1]>0) && (fabs(ImEigen[0]) < 10e-8) && (fabs(ImEigen[1]) < 10e-8) ) //Source
               return 0;
	      else if( (RealEigen[0]<0) && (RealEigen[1]<0) && (fabs(ImEigen[0]) < 10e-8) && (fabs(ImEigen[1]) < 10e-8) ) //Sink
               return 4;
	      else if( ((RealEigen[0]*RealEigen[1])<0) && (fabs(ImEigen[0]) < 10e-8) && (fabs(ImEigen[1]) < 10e-8) ) //Saddle
               return 2;
	      else if( (fabs(RealEigen[0]-RealEigen[1])<10e-8) && (RealEigen[0]>0) && (fabs(ImEigen[0]) > 10e-8) && (fabs(ImEigen[0]+ImEigen[1]) < 10e-8) ) //Repelling focus
               return 1;
	      else if( (fabs(RealEigen[0]-RealEigen[1])<10e-8) && (RealEigen[0]< 0) && (fabs(ImEigen[0]) > 10e-8) && (fabs(ImEigen[0]+ImEigen[1]) < 10e-8) ) //Attracting focus
               return 5;
	      else if( (fabs(RealEigen[0]) < 10e-8) && (fabs(RealEigen[1]) < 10e-8) && (fabs(ImEigen[0]) > 10e-8) && (fabs(ImEigen[0]+ImEigen[1]) < 10e-8) ) //Center
               return 3;
	      else
                return 6;
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
     draw_stream=false;
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

void Experiment9_1::OverlayGrid()
{
   float32 grid_alpha=0.3;
   Vector4f grid_color;
   grid_color = makeVector4f(0.5,0.5,0.5,grid_alpha);
  //Traverse one dimension first and then the other dimension
   for(size_t j=0; j<field.dims()[1]; j++)
    {
        viewer->addLine(field.nodePosition(0,j), field.nodePosition(field.dims()[0]-1,j), grid_color, 1);
    }
   for(size_t i=0; i<field.dims()[0]; i++)
    {
        viewer->addLine(field.nodePosition(i,0), field.nodePosition(i,field.dims()[1]-1), grid_color, 1);
    }

    viewer->refresh();
}

void Experiment9_1::ShowSeparatrices()
{
   if(persist_on == false)
   {
     viewer->clear();
   }

   //Streamline color and width
   stream_color=makeVector4f(1,1,1,stream_alpha);
   stream_width=4;
   //RK4 arclength and step size
   step_size=((field.boundMax()[0]-field.boundMin()[0])/field.dims()[0])*((field.boundMax()[0]-field.boundMin()[0])/field.dims()[0]);
		   
   step_size+=((field.boundMax()[1]-field.boundMin()[1])/field.dims()[1])*((field.boundMax()[1]-field.boundMin()[1])/field.dims()[1]);
		   
   step_size= 0.05* sqrtf(step_size);

   arc_length=((field.boundMax()[0]-field.boundMin()[0]))*((field.boundMax()[0]-field.boundMin()[0])) + ((field.boundMax()[1]-field.boundMin()[1]))*((field.boundMax()[1]-field.boundMin()[1]));
   arc_length= 2 * sqrtf(arc_length); //Max arclength is 2 x the diagonal of the bounding box 

   if(critical_pt_avail ==false)
   {
     get_critical_points();
   }

   //Go through Critical points and look for saddle points
              
   for(int i=0; i<CriticalPoints.size(); i++)
   {
	   if((static_cast<int>(CriticalPoints[i][2])) == 2)
	   {
              Vector2f CrPt;
              CrPt=makeVector2f(CriticalPoints[i][0],CriticalPoints[i][1]);    //Read X and Y coordinate out
	      //Get Jacobian and EigenVectors

	      Vector2f RealEigen;
	      Vector2f ImEigen;
	      Matrix2f EigenVectors;

	      Matrix2f field_jacobian;
	      field_jacobian = field.sampleJacobian(CrPt);
	      field_jacobian.solveEigenProblem(RealEigen,ImEigen,EigenVectors);

	      
	      //Check the eigen value corresponding to the eigen vectors to figure out if the eigen vector points
	      //inward or outward and start backward/forward integration.
	      //The field value may be too low close to the saddle point, causing rk4 to take many small steps,
	      //potentially slowing down the generation of separatrices. Take seed points a bit further away from
	      //the saddle point, in the direction of the eigenvectors. Let the distance be a function of the cell
	      //diagonal

	      
	      for(int idx=0; idx<2; idx++)
	      {
		   //Computing the seed points
		   float step_from_saddle=((field.boundMax()[0]-field.boundMin()[0])/field.dims()[0])*((field.boundMax()[0]-field.boundMin()[0])/field.dims()[0]);
		   step_from_saddle+=((field.boundMax()[1]-field.boundMin()[1])/field.dims()[1])*((field.boundMax()[1]-field.boundMin()[1])/field.dims()[1]);
		   step_from_saddle= 0.005* sqrtf(step_from_saddle);

	           draw_stream_rk4(CrPt+makeVector2f(step_from_saddle*EigenVectors[idx][0],step_from_saddle*EigenVectors[idx][1]),(RealEigen[idx] > 0)); 
	           draw_stream_rk4(CrPt-makeVector2f(step_from_saddle*EigenVectors[idx][0],step_from_saddle*EigenVectors[idx][1]),(RealEigen[idx] > 0));
	      }
		      
	   }
   }  


}

void Experiment9_1::DrawStreamModulated()
{
    
   if(persist_on == false)
   {
     viewer->clear();
   }

   //Streamline color and width
   stream_color=makeVector4f(1,1,.1,stream_alpha);
   stream_width=1;
   //RK4 arclength and step size
   step_size=((field.boundMax()[0]-field.boundMin()[0])/field.dims()[0])*((field.boundMax()[0]-field.boundMin()[0])/field.dims()[0]);
		   
   step_size+=((field.boundMax()[1]-field.boundMin()[1])/field.dims()[1])*((field.boundMax()[1]-field.boundMin()[1])/field.dims()[1]);
		   
   step_size= 0.1* sqrtf(step_size);

   arc_length=((field.boundMax()[0]-field.boundMin()[0]))*((field.boundMax()[0]-field.boundMin()[0])) + ((field.boundMax()[1]-field.boundMin()[1]))*((field.boundMax()[1]-field.boundMin()[1]));
   arc_length= sqrtf(arc_length); //Max arclength is 1 x the diagonal of the bounding box

   //Divide x and y into 10x10 grid
   //Distribute the number of seeds among the squares by the average value of each square

   float32 avg_val[10][10]; 
   float32 sum_avg=0;
   float32 x_step = (field.boundMax()[0]-field.boundMin()[0])/10;
   float32 y_step = (field.boundMax()[1]-field.boundMin()[1])/10;
   int i=0,j=0;
   for(float32 x=field.boundMin()[0]; x<field.boundMax()[0]-x_step/2; x+=x_step)
   {
     j=0;	   
     for(float32 y=field.boundMin()[1]; y<field.boundMax()[1]-y_step/2; y+=y_step)
     {
          Vector2f vec = field.sample(makeVector2f(x,y));
	  avg_val[i][j]= sqrtf( vec.getSqrNorm());
	  vec=field.sample(makeVector2f(x+x_step,y));
	  avg_val[i][j] += sqrtf( vec.getSqrNorm());
	  vec=field.sample(makeVector2f(x,y+y_step));
	  avg_val[i][j] += sqrtf( vec.getSqrNorm());
	  vec=field.sample(makeVector2f(x+x_step,y+y_step));
	  avg_val[i][j] += sqrtf( vec.getSqrNorm());

	  avg_val[i][j]=avg_val[i][j]/4;
	  sum_avg+=avg_val[i][j];

	  output << i << " " << j << " "<<avg_val[i][j]<<"\n";
	  j++;
     }
     i++;

   }

   i=0;
   j=0;
   int total_seeds=0;
   for(float32 x=field.boundMin()[0]; x<field.boundMax()[0]-x_step/2; x+=x_step)
   {
    j=0;	   
    for(float32 y=field.boundMin()[1]; y<field.boundMax()[1]-y_step/2; y+=y_step)
    {
	 int nseeds= ((avg_val[i][j]/sum_avg)*(float)num_seeds) + rand()%2;
	 total_seeds+=nseeds;

         //Draw streamlines in both forward and backward directions for num_seeds random points

         for(int k=0; k< nseeds; k++)
         {
           Vector2f X_Seed;
           X_Seed[0]= ((rand() * x_step)/ RAND_MAX) + x;
           X_Seed[1]= ((rand() * y_step) / RAND_MAX) + y;
           if(show_seeds==true)
           {
              Point2D SeedPt;
              SeedPt.position=X_Seed;
              SeedPt.color=makeVector4f(1,1,0,1);
              viewer->addPoint(SeedPt);     
           }
           draw_stream_rk4(X_Seed,true); //Forward
           draw_stream_rk4(X_Seed,false); //Backward
           
         }
	 j++;
    }
    i++;
   }
    output<< "Total Seeds "<<total_seeds;

}


void Experiment9_1::draw_stream_rk4(Vector2f X_Seed, bool direction)
{
	  draw_stream=true;
	  bool result;
          if(direction == true)  //Forward
             result=rk4_forward(X_Seed);
          else                  //Backward
             result=rk4_back(X_Seed);

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

             
	  if(draw_stream)
	  {
            viewer->addLine(x1,x1+inc,stream_color,stream_width);
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

             	  
	  if(draw_stream)
	  {
            viewer->addLine(x1,x1-inc,stream_color,stream_width);
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
