//---------------------------------------------------------------------------
#ifndef Experiment1_1H
#define Experiment1_1H
//---------------------------------------------------------------------------
#include "Experiment.h"
#include "LinearAlgebra.h"
#include "GLGeometryViewer.h"
//---------------------------------------------------------------------------


/// Assignment 1 Problem 1
///
/// The code draws a scatter plot and a parallel coordinates
///  plot side by for different kinds of data 
class Experiment1_1 : public Experiment
{
    GEOX_CLASS(Experiment1_1)

//Constructor / Destructor
public:
    Experiment1_1();
    virtual ~Experiment1_1();

//Methods
public:
    void Create_Line();
    void Create_Circle();
    void Create_Hyperbola();
    void Create_Hyperbola_Ellipse();
    void DrawScatterPlot();
    void DrawParallelPlot(float);
    void DrawParallelPlot2(float);
    virtual QWidget* createViewer();

//Attributes
private:
    vector <Vector2f> Data;   //Keeps track of data
    Vector2f max_Data;        //Keeps track of max in x and y direction
    Vector2f min_Data;        //Keeps track of min in x and y direction
    Vector2f DataColorRange;
 
    float32 line_slope;       //Takes input for the Line slope
    float32 line_len;         //Takes input for the Length of the line
    Vector2f start_point;     //Takes the starting point of the line as input
    
    Vector2f center;
    float32 radius;
    
    float32 hyp_a;
    float32 hyp_b;
    	

protected:
    GLGeometryViewer* viewer;
};


#endif
