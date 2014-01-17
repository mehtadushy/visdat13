//---------------------------------------------------------------------------
#include "stdafx.h"
#include <cmath>
//---------------------------------------------------------------------------
#include "Experiment5_3.h"
//---------------------------------------------------------------------------
#include "Properties.h"
#include "GLGeometryViewer.h"
#include "GeoXOutput.h"
//---------------------------------------------------------------------------
#include <limits>


IMPLEMENT_GEOX_CLASS( Experiment5_3, 0)                        //Name the class. Set version number to 0
{
    BEGIN_CLASS_INIT( Experiment5_3 );                         //Begin the declaration of properties
							       //We first add fields for manipulating data for the Experiment
   ADD_SEPARATOR("Vector Field")
   ADD_STRING_PROP(vector_filename,0)
   ADD_SEPARATOR("Show Seeds")
   ADD_BOOLEAN_PROP(show_seeds,0)  
   ADD_SEPARATOR("Set Persist")
   ADD_BOOLEAN_PROP(persist_on,0)
   ADD_SEPARATOR("Direction field")
   ADD_BOOLEAN_PROP(direction_field,0)
   ADD_SEPARATOR("Vector field")
   ADD_BOOLEAN_PROP(vector_field,0)
   ADD_SEPARATOR("Number of Seeds")
   ADD_INT32_PROP(num_seeds,0) 
   ADD_SEPARATOR("Number of Grid points for Uniform Seeding")
   ADD_INT32_PROP(num_seeds_x,0)
   ADD_INT32_PROP(num_seeds_y,0)
   ADD_SEPARATOR("Alpha of Streamlines")
   ADD_FLOAT32_PROP(stream_alpha,0)
   ADD_NOARGS_METHOD(Experiment5_3::LoadVectorField)
   ADD_NOARGS_METHOD(Experiment5_3::OverlayGrid)

   ADD_SEPARATOR("RK4 PARAMETERS")
   ADD_FLOAT32_PROP(step_size,0)
   ADD_INT32_PROP(max_steps,0)
   ADD_FLOAT32_PROP(field_val_tolerance,0)

   ADD_NOARGS_METHOD(Experiment5_3::DrawStreamRandom)
   ADD_NOARGS_METHOD(Experiment5_3::DrawStreamUniform)
   ADD_NOARGS_METHOD(Experiment5_3::DrawStreamModulated)

}

QWidget* Experiment5_3::createViewer()  
{
    viewer = new GLGeometryViewer();
    return viewer;
}

Experiment5_3::Experiment5_3()           //Constructor
{
    viewer = NULL;                       
    show_seeds=true;
    stream_alpha=0.5;
    persist_on=false;
    vector_field=true;
    direction_field=false;
    num_seeds=100;
    num_seeds_x=10;
    num_seeds_y=10;
    stream_color=makeVector4f(1,0,0,stream_alpha);
    stream_width=1;
    step_size=0.01;
    max_steps=1000;
    field_val_tolerance = 0.0005;
}

Experiment5_3::~Experiment5_3() {}

void Experiment5_3::LoadVectorField()
{
    if (!field.load(vector_filename))
    {
        output << "Error loading field file " << vector_filename << "\n";
        return;
    }	
}


void Experiment5_3::OverlayGrid()
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
Vector2f Experiment5_3::rk4_forward(Vector2f x1,bool field_type)
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
       
    x2= x1 + (v1/6 + v2/3 + v3/3 + v4/6) * step_size;
    viewer->addLine(x1,x2,stream_color,stream_width);
    return x2;
}
Vector2f Experiment5_3::rk4_back(Vector2f x1,bool field_type)
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
       
    x2= x1 - (v1/6 + v2/3 + v3/3 + v4/6) * step_size;
    viewer->addLine(x1,x2,stream_color,stream_width);
    return x2;
}

Vector2f Experiment5_3::func_v(Vector2f x)
{
   //Bilinear Interpolation of the vector field to get the field value at this point
   return field.sample(x);  	
}

void Experiment5_3::draw_stream_rk4(Vector2f X_Seed, bool direction)
{
  if(vector_field == true) //Draw vector field	
  {  for(int i=0; i<max_steps; i++)
     {
          if(direction == true)  //Forward
             X_Seed=rk4_forward(X_Seed,true);
          else                  //Backward
             X_Seed=rk4_back(X_Seed,true);

          //Terminate execution on hitting the domain boundaries
          if( (X_Seed[0] > (field.boundMax()[0]-0.0005)) || (X_Seed[0] < (field.boundMin()[0]+0.0005)) || (X_Seed[1] > (field.boundMax()[1]-0.0005)) || (X_Seed[1] < (field.boundMin()[1] + 0.0005)) )
           break;
          //Terminate execution if the field value is too low
          Vector2f Vec=field.sample(X_Seed);
          if(Vec.getSqrNorm()< (field_val_tolerance* field_val_tolerance))
           break;

     }
     viewer->refresh();
  }

   if(direction_field == true) //Draw direction fields
   { for(int i=0; i<max_steps; i++)
     {
	if(direction == true)  //Forward
	   X_Seed=rk4_forward(X_Seed,false);
	else                  //Backward
	   X_Seed=rk4_back(X_Seed,false);

	//Terminate execution on hitting the domain boundaries
        if( (X_Seed[0] > (field.boundMax()[0]-0.0005)) || (X_Seed[0] < (field.boundMin()[0]+0.0005)) || (X_Seed[1] > (field.boundMax()[1]-0.0005)) || (X_Seed[1] < (field.boundMin()[1] + 0.0005)) )
	 break;
	//Terminate execution if the field value is too low
        Vector2f Vec=field.sample(X_Seed);
	if(Vec.getSqrNorm()< (field_val_tolerance* field_val_tolerance))
	 break;
	//
     }
     viewer->refresh();     
   }

}


void Experiment5_3::DrawStreamRandom()
{
    
   if(persist_on == false)
   {
     viewer->clear();
   }

   //Draw streamlines in both forward and backward directions for num_seeds random points

   for(int i=0; i< num_seeds; i++)
   {
     Vector2f X_Seed;
     X_Seed[0]= (((float32)rand() * (field.boundMax()[0]-field.boundMin()[0])) / RAND_MAX) + field.boundMin()[0];
     X_Seed[1]= (((float32)rand() * (field.boundMax()[1]-field.boundMin()[1])) / RAND_MAX) + field.boundMin()[1];
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
    

}

void Experiment5_3::DrawStreamUniform()
{
    
   if(persist_on == false)
   {
     viewer->clear();
   }

   //Draw streamlines in both forward and backward directions for the number of points
   // given by num_seeds_x and num_seeds_y
   for(int i=0; i< num_seeds_x; i++)
   for(int j=0; j< num_seeds_y; j++)
   {
     Vector2f X_Seed;
     X_Seed[0]= i* ((field.boundMax()[0]-field.boundMin()[0]) / num_seeds_x) + field.boundMin()[0];
     X_Seed[1]= j* ((field.boundMax()[1]-field.boundMin()[1]) / num_seeds_y) + field.boundMin()[1];
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
    

}

void Experiment5_3::DrawStreamModulated()
{
    
   if(persist_on == false)
   {
     viewer->clear();
   }
   
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
// /home/dushy/Desktop/VisDat13/Assignments/Assignment05/Data/Cylinderclose2CT10.am
