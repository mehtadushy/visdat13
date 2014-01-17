//---------------------------------------------------------------------------
#include "stdafx.h"
#include <cmath>
//---------------------------------------------------------------------------
#include "Experiment3_1.h"
//---------------------------------------------------------------------------
#include "Properties.h"
#include "GLGeometryViewer.h"
#include "GeoXOutput.h"
//---------------------------------------------------------------------------
#include <limits>
#include "Field2.hpp"


IMPLEMENT_GEOX_CLASS( Experiment3_1, 0)                        //Name the class. Set version number to 0
{
    BEGIN_CLASS_INIT( Experiment3_1 );                         //Begin the declaration of properties
							       //We first add fields for manipulating data for the Experiment
   ADD_SEPARATOR("Scalar Field")
   ADD_STRING_PROP(scalar_filename,0)
   ADD_SEPARATOR("Plot grid with or without data")
   ADD_BOOLEAN_PROP(grid_w_data,0)
   ADD_SEPARATOR("Alpha Value for Grid Lines")
   ADD_FLOAT32_PROP(grid_alpha,0)  
   ADD_NOARGS_METHOD(Experiment3_1::DrawRegularMesh)
   ADD_SEPARATOR("Alpha value for Iso")
   ADD_FLOAT32_PROP(iso_alpha,0)  
   ADD_SEPARATOR("Iso Contour Value")
   ADD_FLOAT32_PROP(iso_c,0)    
   ADD_NOARGS_METHOD(Experiment3_1::DrawIsoContour)
   ADD_NOARGS_METHOD(Experiment3_1::DrawIsoContour_Asymp)

}

QWidget* Experiment3_1::createViewer()  
{
    viewer = new GLGeometryViewer();
    return viewer;
}

Experiment3_1::Experiment3_1()           //Constructor
{
    viewer = NULL;                       // Why?
    grid_w_data=true;
    grid_alpha=0.3;
    iso_alpha=0.5;
    iso_c=5.0f;                        //Might not mean anything for a given dataset 

    
}

Experiment3_1::~Experiment3_1() {}

 

void Experiment3_1::DrawRegularMesh()
{
    viewer->clear();

    //Load scalar field
    ScalarField2 field;
    if (!field.load(scalar_filename))
    {
        output << "Error loading field file " << scalar_filename << "\n";
        return;
    }


    //Get the minimum/maximum value in that field
    float32 min = std::numeric_limits<float32>::max();
    float32 max = -std::numeric_limits<float32>::max();
    for(size_t j=0; j<field.dims()[1]; j++)
    {
        for(size_t i=0; i< field.dims()[0]; i++)
        {
            const float32 val = field.nodeScalar(i,j);
            min = val < min ? val : min;
            max = val > max ? val : max;
        }
    }

  //Plot the grid
  //Traverse one dimension first and then the other dimension
   for(size_t j=0; j<field.dims()[1]; j++)
    {
        viewer->addLine(field.nodePosition(0,j), field.nodePosition(field.dims()[0]-1,j), makeVector4f(0.5,0.5,0.5,grid_alpha), 1);
    }
   for(size_t i=0; i<field.dims()[0]; i++)
    {
        viewer->addLine(field.nodePosition(i,0), field.nodePosition(i,field.dims()[1]-1), makeVector4f(0.5,0.5,0.5,grid_alpha), 1);
    }

  //Optionally overlay data on the grid
  if (grid_w_data == true )
  {
    //Draw a point for each grid vertex.
    for(size_t j=0; j<field.dims()[1]; j++)
    {
        for(size_t i=0; i<field.dims()[0]; i++)
        {
            const float32 val = field.nodeScalar(i, j);
            const float32 c = (val - min) / (max - min);

            Point2D p;
            p.position  = field.nodePosition(i, j);
            p.size = 5;
            //Use a grayscale depending on the actual value
            p.color[0] = c; p.color[1] = c; p.color[2] = c;
            viewer->addPoint(p);
        }
    }
  }



    viewer->refresh();
}


void Experiment3_1::DrawIsoContour()
{
    viewer->clear();

    //Load scalar field
    ScalarField2 field;
    if (!field.load(scalar_filename))
    {
        output << "Error loading field file " << scalar_filename << "\n";
        return;
    }


    //Get the minimum/maximum value in that field
    float32 min = std::numeric_limits<float32>::max();
    float32 max = -std::numeric_limits<float32>::max();
    for(size_t j=0; j<field.dims()[1]; j++)
    {
        for(size_t i=0; i< field.dims()[0]; i++)
        {
            const float32 val = field.nodeScalar(i,j);
            min = val < min ? val : min;
            max = val > max ? val : max;
        }
    }

  //Plot the grid
  //Traverse one dimension first and then the other dimension
   for(size_t j=0; j<field.dims()[1]; j++)
    {
        viewer->addLine(field.nodePosition(0,j), field.nodePosition(field.dims()[0]-1,j), makeVector4f(0.5,0.5,0.5,grid_alpha), 1);
    }
   for(size_t i=0; i<field.dims()[0]; i++)
    {
        viewer->addLine(field.nodePosition(i,0), field.nodePosition(i,field.dims()[1]-1), makeVector4f(0.5,0.5,0.5,grid_alpha), 1);
    }

  //Optionally overlay data on the grid
  if (grid_w_data == true )
  {
    //Draw a point for each grid vertex.
    for(size_t j=0; j<field.dims()[1]; j++)
    {
        for(size_t i=0; i<field.dims()[0]; i++)
        {
            const float32 val = field.nodeScalar(i, j);
            const float32 c = (val - min) / (max - min);

            Point2D p;
            p.position  = field.nodePosition(i, j);
            p.size = 5;
            //Use a grayscale depending on the actual value
            p.color[0] = c; p.color[1] = c; p.color[2] = c;
            viewer->addPoint(p);
        }
    }
  }



    viewer->refresh();



  //Plot the Iso Contour
  //Traverse the cells
   for(size_t i=0; i<field.dims()[0]-1; i++)
    {
      for(size_t j=0; j<field.dims()[1]-1; j++)
       {
          
         //Compute Max and Min values in each cell
         float32 fval[4];
         fval[0]= field.nodeScalar(i,j);
         fval[1]= field.nodeScalar(i,j+1);
         fval[2]= field.nodeScalar(i+1,j+1);
         fval[3]= field.nodeScalar(i+1,j);

         vector <Vector2f> Pos;
         Pos.resize(4);
         Pos[0]=field.nodePosition(i,j);
         Pos[1]=field.nodePosition(i,j+1);
         Pos[2]=field.nodePosition(i+1,j+1);
         Pos[3]=field.nodePosition(i+1,j);

         //output << "** "<< Pos[0][0] << " " << Pos[0][1] <<"\n";
         //output << field.nodePosition(i,j)[0] << " "<< field.nodePosition(i,j)[1] << "\n" ;

         
         float32 maxf, minf;
         maxf=fmax(fval[0],fval[1]);
         maxf=fmax(maxf,fval[2]);
         maxf=fmax(maxf,fval[3]);
         minf=fmin(fval[0],fval[1]);
         minf=fmin(minf,fval[2]);
         minf=fmin(minf,fval[3]);


         //Check if the iso contour passes the grid
         if((iso_c < minf)  || (iso_c > maxf))
           continue;
         else   //We plot the iso contour
         {
            
              float32 EdgeChar[3][4];

             //Now figure out the signs for the four nodes
               bool sign[4]= {0,0,0,0};
               for(int k=0; k<4; k++)
               {
                 if(fval[k]>=iso_c)
                  sign[k] = 1;
               }

               //Traverse the nodes clockwise and see if the edge carries a crossing point
               for(int k=0; k<4; k++)
               {
                  if( sign[k] ^ sign[(k+1)%4] )
                  {
                    EdgeChar[2][k] = 1;
                    float32 x,y;
                    x = ( (Pos[(k+1)%4][0] - Pos[k][0])*iso_c + Pos[k][0] * fval[(k+1)%4] -  Pos[(k+1)%4][0] * fval[k] ) / ( fval[(k+1)%4] - fval[k] ) ;
                    y = ( (Pos[(k+1)%4][1] - Pos[k][1])*iso_c + Pos[k][1] * fval[(k+1)%4] -  Pos[(k+1)%4][1] * fval[k] ) / ( fval[(k+1)%4] - fval[k] ) ;
                    EdgeChar[0][k] = x;
                    EdgeChar[1][k] = y;

                  }
                  else
                    EdgeChar[2][k] = 0;

                } 
   
                //Count the number of edges that have a valid crossing point. It should be either 2 or 4 
               int EdgeCount=0; 
               for(int k=0; k<4; k++)
               {
                  if(EdgeChar[2][k]==1)
                  EdgeCount++;
               }
               
               if(EdgeCount == 2)
               {
                  //Simply plot the contour
                  vector <Vector2f> LineEnd;
                  LineEnd.clear();
                  LineEnd.resize(2);
                  int index=0;

                  for(int k=0; k<4; k++)
                  {
                    if(EdgeChar[2][k]==1)
                    {
                      LineEnd[index]= makeVector2f(EdgeChar[0][k], EdgeChar[1][k]) ;
                      index++;
                    }
                  }
                  viewer->addLine(LineEnd[0], LineEnd[1], makeVector4f(0.5,0.5,0.5,iso_alpha), 2);
                  viewer->refresh();
                  
                  
               }
               else   //Cell has the contour intersecting at 4 points
               {
                 //There is an ambiguity to be resolved
                 float32 fmid= (fval[0]+fval[1]+fval[2]+fval[3])/4.0 ;
                 bool fsign=0;
                 if (fmid >= iso_c)  
                   fsign = 1;

                  vector <Vector2f> LineEnd;
                  LineEnd.clear();
                  LineEnd.resize(4);

                  for(int k=0; k<4; k++)
                  {
                      LineEnd[k]= makeVector2f(EdgeChar[0][k], EdgeChar[1][k]) ;
                  }


                 if(!(fsign ^ sign[0]))  //Connect 0 to 1  and 2 to 3
                 {
                           viewer->addLine(LineEnd[0], LineEnd[1], makeVector4f(0.5,1,0.5,iso_alpha), 2);
                           viewer->addLine(LineEnd[2], LineEnd[3], makeVector4f(0.5,1,0.5,iso_alpha), 2);
                             viewer->refresh();

                 }
                 else                //Connect 3 to 0 and 1 to 2
                 {
                                         
                           viewer->addLine(LineEnd[3], LineEnd[0], makeVector4f(0.5,1,0.5,iso_alpha), 2);
                           viewer->addLine(LineEnd[1], LineEnd[2], makeVector4f(0.5,1,0.5,iso_alpha), 2);
                            viewer->refresh();

                 }
         
                 
               
               }
               
          }

           
    }

  }
}



void Experiment3_1::DrawIsoContour_Asymp()
{
    viewer->clear();

    //Load scalar field
    ScalarField2 field;
    if (!field.load(scalar_filename))
    {
        output << "Error loading field file " << scalar_filename << "\n";
        return;
    }


    //Get the minimum/maximum value in that field
    float32 min = std::numeric_limits<float32>::max();
    float32 max = -std::numeric_limits<float32>::max();
    for(size_t j=0; j<field.dims()[1]; j++)
    {
        for(size_t i=0; i< field.dims()[0]; i++)
        {
            const float32 val = field.nodeScalar(i,j);
            min = val < min ? val : min;
            max = val > max ? val : max;
        }
    }

  //Plot the grid
  //Traverse one dimension first and then the other dimension
   for(size_t j=0; j<field.dims()[1]; j++)
    {
        viewer->addLine(field.nodePosition(0,j), field.nodePosition(field.dims()[0]-1,j), makeVector4f(0.5,0.5,0.5,grid_alpha), 1);
    }
   for(size_t i=0; i<field.dims()[0]; i++)
    {
        viewer->addLine(field.nodePosition(i,0), field.nodePosition(i,field.dims()[1]-1), makeVector4f(0.5,0.5,0.5,grid_alpha), 1);
    }

  //Optionally overlay data on the grid
  if (grid_w_data == true )
  {
    //Draw a point for each grid vertex.
    for(size_t j=0; j<field.dims()[1]; j++)
    {
        for(size_t i=0; i<field.dims()[0]; i++)
        {
            const float32 val = field.nodeScalar(i, j);
            const float32 c = (val - min) / (max - min);

            Point2D p;
            p.position  = field.nodePosition(i, j);
            p.size = 5;
            //Use a grayscale depending on the actual value
            p.color[0] = c; p.color[1] = c; p.color[2] = c;
            viewer->addPoint(p);
        }
    }
  }



    viewer->refresh();



  //Plot the Iso Contour
  //Traverse the cells
   for(size_t i=0; i<field.dims()[0]-1; i++)
    {
      for(size_t j=0; j<field.dims()[1]-1; j++)
       {
          
         //Compute Max and Min values in each cell
         float32 fval[4];
         fval[0]= field.nodeScalar(i,j);
         fval[1]= field.nodeScalar(i,j+1);
         fval[2]= field.nodeScalar(i+1,j+1);
         fval[3]= field.nodeScalar(i+1,j);

         vector <Vector2f> Pos;
         Pos.resize(4);
         Pos[0]=field.nodePosition(i,j);
         Pos[1]=field.nodePosition(i,j+1);
         Pos[2]=field.nodePosition(i+1,j+1);
         Pos[3]=field.nodePosition(i+1,j);

         //output << "** "<< Pos[0][0] << " " << Pos[0][1] <<"\n";
         //output << field.nodePosition(i,j)[0] << " "<< field.nodePosition(i,j)[1] << "\n" ;

         
         float32 maxf, minf;
         maxf=fmax(fval[0],fval[1]);
         maxf=fmax(maxf,fval[2]);
         maxf=fmax(maxf,fval[3]);
         minf=fmin(fval[0],fval[1]);
         minf=fmin(minf,fval[2]);
         minf=fmin(minf,fval[3]);


         //Check if the iso contour passes the grid
         if((iso_c < minf)  || (iso_c > maxf))
           continue;
         else   //We plot the iso contour
         {
            
              float32 EdgeChar[3][4];

             //Now figure out the signs for the four nodes
               bool sign[4]= {0,0,0,0};
               for(int k=0; k<4; k++)
               {
                 if(fval[k]>=iso_c)
                  sign[k] = 1;
               }

               //Traverse the nodes clockwise and see if the edge carries a crossing point
               for(int k=0; k<4; k++)
               {
                  if( sign[k] ^ sign[(k+1)%4] )
                  {
                    EdgeChar[2][k] = 1;
                    float32 x,y;
                    x = ( (Pos[(k+1)%4][0] - Pos[k][0])*iso_c + Pos[k][0] * fval[(k+1)%4] -  Pos[(k+1)%4][0] * fval[k] ) / ( fval[(k+1)%4] - fval[k] ) ;
                    y = ( (Pos[(k+1)%4][1] - Pos[k][1])*iso_c + Pos[k][1] * fval[(k+1)%4] -  Pos[(k+1)%4][1] * fval[k] ) / ( fval[(k+1)%4] - fval[k] ) ;
                    EdgeChar[0][k] = x;
                    EdgeChar[1][k] = y;

                  }
                  else
                    EdgeChar[2][k] = 0;

                } 
   
                //Count the number of edges that have a valid crossing point. It should be either 2 or 4 
               int EdgeCount=0; 
               for(int k=0; k<4; k++)
               {
                  if(EdgeChar[2][k]==1)
                  EdgeCount++;
               }
               
               if(EdgeCount == 2)
               {
                  //Simply plot the contour
                  vector <Vector2f> LineEnd;
                  LineEnd.clear();
                  LineEnd.resize(2);
                  int index=0;

                  for(int k=0; k<4; k++)
                  {
                    if(EdgeChar[2][k]==1)
                    {
                      LineEnd[index]= makeVector2f(EdgeChar[0][k], EdgeChar[1][k]) ;
                      index++;
                    }
                  }
                  viewer->addLine(LineEnd[0], LineEnd[1], makeVector4f(0.5,0.5,0.5,iso_alpha), 2);
                  viewer->refresh();
                  
                  
               }
               else   //Cell has the contour intersecting at 4 points
               {
                 //There is an ambiguity to be resolved
                 float32 fmid= (fval[3]*fval[1] - fval[2]*fval[0]) / ( fval[3]+fval[1] -fval[2] - fval[0] ) ;
                
                  vector <Vector2f> LineEnd;
                  LineEnd.clear();
                  LineEnd.resize(4);

                  for(int k=0; k<4; k++)
                  {
                      LineEnd[k]= makeVector2f(EdgeChar[0][k], EdgeChar[1][k]) ;
                  }


		 if(fval[1] < iso_c)
		 {
                    if(iso_c > fmid)  //Connect 3 to 0 and 1 to 2
                    {
                              viewer->addLine(LineEnd[3], LineEnd[0], makeVector4f(1,0.5,0.5,iso_alpha), 2);
                              viewer->addLine(LineEnd[1], LineEnd[2], makeVector4f(1,0.5,0.5,iso_alpha), 2);
                              viewer->refresh();
                    }
                    else            //Connect 0 to 1  and 2 to 3    
                    {
                              viewer->addLine(LineEnd[0], LineEnd[1], makeVector4f(1,0.5,0.5,iso_alpha), 2);
                              viewer->addLine(LineEnd[2], LineEnd[3], makeVector4f(1,0.5,0.5,iso_alpha), 2);
                              viewer->refresh();
                    }

		 }
		 else
		 {
                    if(iso_c > fmid)  //Connect 0 to 1  and 2 to 3
                    {
                              viewer->addLine(LineEnd[0], LineEnd[1], makeVector4f(1,0.5,0.5,iso_alpha), 2);
                              viewer->addLine(LineEnd[2], LineEnd[3], makeVector4f(1,0.5,0.5,iso_alpha), 2);
                                viewer->refresh();

                    }
                    else                //Connect 3 to 0 and 1 to 2
                    {
                                            
                              viewer->addLine(LineEnd[3], LineEnd[0], makeVector4f(1,0.5,0.5,iso_alpha), 2);
                              viewer->addLine(LineEnd[1], LineEnd[2], makeVector4f(1,0.5,0.5,iso_alpha), 2);
                               viewer->refresh();

                    }
		 }
         
                 
               
               }
               
          }

           
    }

  }
}

