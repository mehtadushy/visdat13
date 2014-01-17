//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "Experiment1_1.h"
//---------------------------------------------------------------------------
#include "Properties.h"
#include "GLGeometryViewer.h"
#include "GeoXOutput.h"
//---------------------------------------------------------------------------

IMPLEMENT_GEOX_CLASS( Experiment1_1, 0)                        //Name the class. Set version number to 0
{
    BEGIN_CLASS_INIT( Experiment1_1 );                         //Begin the declaration of properties
							       //We first add fields for manipulating data for the Experiment
   ADD_SEPARATOR("Line")
   ADD_VECTOR2F_PROP(start_point,0)
   //ADD_FLOAT32_PROP(start_point[1],0)
   ADD_FLOAT32_PROP(line_len,0)
   ADD_FLOAT32_PROP(line_slope,0)
   
   ADD_SEPARATOR("Circle")
   ADD_VECTOR2F_PROP(center,0)
   //ADD_FLOAT32_PROP(centery,0)
   ADD_FLOAT32_PROP(radius,0)

   ADD_SEPARATOR("Hyperbola")
   ADD_FLOAT32_PROP(hyp_a,0)
   ADD_FLOAT32_PROP(hyp_b,0)

    ADD_NOARGS_METHOD(Experiment1_1::Create_Line)
    ADD_NOARGS_METHOD(Experiment1_1::Create_Circle)
    ADD_NOARGS_METHOD(Experiment1_1::Create_Hyperbola)
    ADD_NOARGS_METHOD(Experiment1_1::Create_Hyperbola_Ellipse)
}

QWidget* Experiment1_1::createViewer()  //What is this for?
{
    viewer = new GLGeometryViewer();
    return viewer;
}

Experiment1_1::Experiment1_1()           //Constructor
{
    viewer = NULL;                       // Why?
    start_point = makeVector2f(0,1); 
    line_slope = -1.0f;
    line_len = 1.42f;
 
    center = makeVector2f(4.0,4.0);
    radius = 4.0f;

    hyp_a=1.0f;
    hyp_b=1.0f;
    
}

Experiment1_1::~Experiment1_1() {}


void Experiment1_1::Create_Line()
{
    // We will put 10 points per unit line_len, with a maximum of 1000 points
    int32 i32_pts_len = 10;
    int32 i32_num_points = (int32)( ((float32)i32_pts_len) * line_len );
    i32_num_points= (i32_num_points<1000)?i32_num_points:1000;
    Data.clear();
    Data.resize(i32_num_points);
    Vector2f end_point;
    end_point[0]=start_point[0] + line_len/sqrtf( line_slope * line_slope + 1.0);
    end_point[1]=start_point[1]+ (line_len*line_slope)/sqrtf( line_slope * line_slope + 1.0);

    for(int i=0;i<(int)Data.size();i++)
    {
        Data[i][0] = start_point[0]+ ((float) i) * (end_point[0]-start_point[0])/(float)(Data.size()-1.0);
        Data[i][1] = start_point[1]+ ((float) i) * (end_point[1]-start_point[1])/(float)(Data.size()-1.0);
        output << i << ": " << Data[i][0] << " " << Data[i][1] << "\n" ;
    }
     
    // Now that we have data with us, we will call the draw scatter function
    viewer->clear();

    DrawScatterPlot();
    DrawParallelPlot(5.0);
}

void Experiment1_1::Create_Circle()
{
    // We will put 50 points per unit radius, with a maximum of 1000 points
    int32 i32_pts_len = 10;
    int32 i32_num_points = (int32)( radius*radius* i32_pts_len);
    i32_num_points= (i32_num_points<1000)?i32_num_points:1000;
    Data.clear();
    Data.resize(i32_num_points);

    for(int i=0;i<(int)Data.size();i++)
    {
        Data[i][0] = radius* cos( (((float) i)* 2.0*3.14)/(float)i32_num_points) + center[0];
        Data[i][1] = radius* sin( (((float) i)* 2.0*3.14)/(float)i32_num_points) + center[1];
    }
     
    // Now that we have data with us, we will call DrawScatterPlot and DrawParallelPlot
    viewer->clear();

    DrawScatterPlot();
    DrawParallelPlot(radius * 2.0);
}

void Experiment1_1::Create_Hyperbola()
{
    int32 i32_num_points = 1000;
    Data.clear();
    Data.resize(i32_num_points*4);

    for(int i=0;i<i32_num_points;i++)
    {
        Data[i][0] = hyp_a* cosh( -3 + (((float) i) * (3.0+3.0) )/(float)i32_num_points) ;
        Data[i][1] = hyp_b* sinh( -3 + (((float) i) * (3.0+3.0) )/(float)i32_num_points) ;
        Data[i+i32_num_points][0] = -hyp_a* cosh( -3 + (((float) i) * (3.0+3.0) )/(float)i32_num_points) ;
        Data[i+i32_num_points][1] = hyp_b* sinh( -3 + (((float) i) * (3.0+3.0) )/(float)i32_num_points) ;
        Data[i+2*i32_num_points][0] = hyp_b* sinh( -3 + (((float) i) * (3.0+3.0) )/(float)i32_num_points) ;
        Data[i+2*i32_num_points][1] = hyp_a* cosh( -3 + (((float) i) * (3.0+3.0) )/(float)i32_num_points) ;
        Data[i+3*i32_num_points][0] = hyp_b* sinh( -3 + (((float) i) * (3.0+3.0) )/(float)i32_num_points) ;
        Data[i+3*i32_num_points][1] = -hyp_a* cosh( -3 + (((float) i) * (3.0+3.0) )/(float)i32_num_points) ;

    }
     
    // Now that we have data with us, we will call DrawScatterPlot and DrawParallelPlot
    viewer->clear();

    DrawScatterPlot();
    DrawParallelPlot2(hyp_a * 8.0);
}

void Experiment1_1::Create_Hyperbola_Ellipse()
{
    int32 i32_num_points = 1000;
    Data.clear();
    Data.resize(i32_num_points*4);

    for(int i=0;i<i32_num_points;i++)
    {
        Data[i][0] = hyp_a* cosh( 3 - (((float) i) * (3.0) )/(float)i32_num_points) ;
        Data[i][1] = hyp_b* sinh( -3 + (((float) i) * (3.0) )/(float)i32_num_points) ;
        Data[i+i32_num_points][0] = -hyp_a* cosh( -3 + (((float) i) * (3.0) )/(float)i32_num_points) ;
        Data[i+i32_num_points][1] = hyp_b* sinh( 3 - (((float) i) * (3.0) )/(float)i32_num_points) ;
        Data[i+2*i32_num_points][1] = hyp_a* cosh( 3 - (((float) i) * (3.0) )/(float)i32_num_points) ;
        Data[i+2*i32_num_points][0] = hyp_b* sinh( -3 + (((float) i) * (3.0) )/(float)i32_num_points) ;
        Data[i+3*i32_num_points][1] = -hyp_a* cosh( -3 + (((float) i) * (3.0) )/(float)i32_num_points) ;
        Data[i+3*i32_num_points][0] = hyp_b* sinh( 3 - (((float) i) * (3.0) )/(float)i32_num_points) ;

    }
     
    // Now that we have data with us, we will call DrawScatterPlot and DrawParallelPlot
    viewer->clear();

    DrawScatterPlot();
    DrawParallelPlot2(hyp_a * 8.0);
}

void Experiment1_1::DrawScatterPlot()
{
    max_Data = makeVector2f(Data[0][0], Data[0][1]);
    min_Data = makeVector2f(Data[0][0], Data[0][1]);
    for(int i=0;i<(int)Data.size();i++)
    {
        if(i>0)
        {
          if (Data[i][0] > max_Data [0])
          {
            max_Data[0]=Data[i][0];
          }
          if (Data[i][1] > max_Data [1])
          {
            max_Data[1]=Data[i][1];
          }
          if (Data[i][0] < min_Data [0])
          {
            min_Data[0]=Data[i][0];
          }
          if (Data[i][1] < min_Data [1])
          {
            min_Data[1]=Data[i][1];
          }

        }
    }
    DataColorRange = makeVector2f(max_Data[0]-min_Data[0], max_Data[1]-min_Data[1]);
    if (max_Data[0]<0)
        max_Data[0]=0;
    if (max_Data[1]<0)
        max_Data[1]=0;
    if (min_Data[0]>0)
        min_Data[0]=0;
    if (min_Data[1]>0)
        min_Data[1]=0;

    for(int i=0;i<(int)Data.size();i++)
    {
        Point2D NewPoint(Data[i][0], Data[i][1]);
        NewPoint.color = makeVector4f(Data[i][0]/DataColorRange[0], Data[i][1]/DataColorRange[1], 0.5 /*((float) i)/Data.size()*/, 1);
        viewer->addPoint(NewPoint);
    }
    //Axes
    Point2D Origin(0, 0);
    Origin.color = makeVector4f(1,1,1,1);
    Origin.size = 10;
    const int idOrigin = viewer->addPoint(Origin);

    Point2D XNeg(min_Data[0], 0);
    Point2D XPos(max_Data[0], 0);
    XNeg.color = makeVector4f(1,0,0,1);
    XPos.color = makeVector4f(1,0,0,1);
    XNeg.size = 10;
    XPos.size = 10;
    const int idXNeg = viewer->addPoint(XNeg);
    const int idXPos = viewer->addPoint(XPos);

    Point2D YNeg(0, min_Data[1]);
    Point2D YPos(0, max_Data[1]);
    YPos.color = makeVector4f(0,1,0,1);
    YNeg.color = makeVector4f(0,1,0,1);
    YPos.size = 10;
    YNeg.size = 10;
    const int idYPos = viewer->addPoint(YNeg);
    const int idYNeg = viewer->addPoint(YPos);

    //X-Axis
    Line Axis;
    Axis.vertices[0] = idXNeg;
    Axis.vertices[1] = idXPos;
    Axis.color = makeVector4f(1,1,1,1);
    Axis.thickness = 3;
    viewer->addLine(Axis);

    //Y-Axis
    Axis.vertices[0] = idYNeg;
    Axis.vertices[1] = idYPos;
    viewer->addLine(Axis);

    // display changes
    viewer->refresh();
}

void Experiment1_1::DrawParallelPlot(float XSep)
{
    float XOff=2.0;                        //Offset between the scatter plot and the parallel plot
    //float XSep=5.0;                        //Separation between the axis of the parallel plot
    
    for(int i=0;i<(int)Data.size();i++)
    {
        viewer->addLine(makeVector2f(XOff+max_Data[0],Data[i][0]-min_Data[0]+0.1+min_Data[1]), makeVector2f(XOff+max_Data[0]+XSep,Data[i][1]-min_Data[1]+0.1+min_Data[1]),makeVector4f(Data[i][0]/DataColorRange[0], Data[i][1]/DataColorRange[1], 0.5 /*((float) i)/Data.size()*/ , 1)  );
    }
    //Axes
   
     viewer->addLine(makeVector2f(XOff+max_Data[0],min_Data[1]), makeVector2f(XOff+max_Data[0],max_Data[0]-min_Data[0]+0.1+min_Data[1]), makeVector4f(1,0,1,1), 3);
     viewer->addLine(makeVector2f(XOff+XSep+max_Data[0],min_Data[1]), makeVector2f(XOff+XSep+max_Data[0],max_Data[1]-min_Data[1]+0.1+min_Data[1]), makeVector4f(1,0,1,1), 3);

    // display changes
    viewer->refresh();
}

void Experiment1_1::DrawParallelPlot2(float XSep)
{
    float XOff=2.0;                        //Offset between the scatter plot and the parallel plot
    //float XSep=5.0;                        //Separation between the axis of the parallel plot
    
    for(int i=0;i<(int)Data.size();i++)
    {
        viewer->addLine(makeVector2f(XOff+max_Data[0],Data[i][0]), makeVector2f(XOff+max_Data[0]+XSep,Data[i][1]),makeVector4f(Data[i][0]/DataColorRange[0], Data[i][1]/DataColorRange[1], 0.5 /*((float) i)/Data.size()*/ , 1)  );
    }
    //Axes
   
     viewer->addLine(makeVector2f(XOff+max_Data[0],min_Data[0]), makeVector2f(XOff+max_Data[0],max_Data[0]), makeVector4f(1,0,1,1), 3);
     viewer->addLine(makeVector2f(XOff+XSep+max_Data[0],min_Data[1]), makeVector2f(XOff+XSep+max_Data[0],max_Data[1]), makeVector4f(1,0,1,1), 3);

    // display changes
    viewer->refresh();
}

