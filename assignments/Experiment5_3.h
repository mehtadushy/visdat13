//---------------------------------------------------------------------------
#ifndef Experiment5_3H
#define Experiment5_3H
//---------------------------------------------------------------------------
#include "Experiment.h"
#include "LinearAlgebra.h"
#include "GLGeometryViewer.h"
//---------------------------------------------------------------------------
#include "Field2.hpp"


/// Assignment 5 Problem 3
///
/// Seeding of Stream Lines for a Vector Field

class Experiment5_3 : public Experiment
{
    GEOX_CLASS(Experiment5_3)

//Constructor / Destructor
public:
    Experiment5_3();
    virtual ~Experiment5_3();

//Methods
public:
    void LoadVectorField();     //Load the Vector field
    void OverlayGrid();            //Draw the Grid with alpha 0.3
    void DrawStreamRandom();	//Draw Streamlines with random seeding. The user inputs the number of seed points.
    void DrawStreamUniform();	//Draw Streamlines with a uniform seeding. The user inputs the number of grid points in each direction
    void DrawStreamModulated();	
private:
    void draw_stream_rk4(Vector2f, bool);             //Integrate streamline with RK4 in the direction given by the flag
    Vector2f rk4_forward(Vector2f,bool);       //Return the next point on the streamline given the current point. Flag indicates whether 
                                       // to use the actual vector field or the direction field
    Vector2f rk4_back(Vector2f,bool);       //Return the next point on the streamline given the current point. Flag indicates whether 
                                       // to use the actual vector field or the direction field
    Vector2f func_v(Vector2f);         //Return the value of the vector field at the point.
    virtual QWidget* createViewer();

//Attribute
private:
    string vector_filename;      //String that contains the path to the Vector Field file
    VectorField2 field;
    bool show_seeds;             // 1: Show Seed Points    0: Don't show Seed Points
    bool persist_on;             // 1: Don't clear the screen before drawing  0: Clear the screen before drawing
    bool vector_field;           // 1: Draw streamlines for vector field      0: Don't Draw streamlines the vector field
    bool direction_field;         // 1: Draw streamlines for direction field  0: Don't Draw streamlines for direction field
    int num_seeds;               // Number of seeds for random and modulated streams
    int num_seeds_x, num_seeds_y; //Number of grid points in x and y direction for uniform seeding
    float32 stream_alpha;        // Alpha of streamlines 

    //RK4 Parameters
    float32 step_size;    //The step size to use for RK4
    int32 max_steps;        //The maximum number of steps to integrate with
    float32 field_val_tolerance;  //The tolerance for the field value

    Vector4f stream_color;        //This is the current streamline color
    float32  stream_width;        //This is the current streamline width


protected:
    GLGeometryViewer* viewer;
};


#endif
