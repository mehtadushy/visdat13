//---------------------------------------------------------------------------
#ifndef Experiment3_1H
#define Experiment3_1H
//---------------------------------------------------------------------------
#include "Experiment.h"
#include "LinearAlgebra.h"
#include "GLGeometryViewer.h"
//---------------------------------------------------------------------------


/// Assignment 3 Problem 1
///
/// The code takes a scalar field file as an input and shows
///  the data overlayed on a mesh and plots the Iso lines 

class Experiment3_1 : public Experiment
{
    GEOX_CLASS(Experiment3_1)

//Constructor / Destructor
public:
    Experiment3_1();
    virtual ~Experiment3_1();

//Methods
public:
    void DrawRegularMesh();
    void DrawIsoContour();	
    void DrawIsoContour_Asymp();	
private:

    virtual QWidget* createViewer();

//Attributes
private:
    string scalar_filename;       //String that contains the path to the Scalar Field file
    bool grid_w_data;             // Create grid with data (1) or without data (0)
    float32 grid_alpha;           // Alpha value for the grid
    float32 iso_alpha;            // Alpha value of the Iso lines
    float32 iso_c;                // Value at which the contour is to be drawn


protected:
    GLGeometryViewer* viewer;
};


#endif
