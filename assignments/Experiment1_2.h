//---------------------------------------------------------------------------
#ifndef Experiment1_2H
#define Experiment1_2H
//---------------------------------------------------------------------------
#include "Experiment.h"
#include "LinearAlgebra.h"
#include "GLGeometryViewer.h"
//---------------------------------------------------------------------------


/// Assignment 1 Problem 2
///
/// The code takes a csv file as an input and plots 
///  the data in parallel coordinates
class Experiment1_2 : public Experiment
{
    GEOX_CLASS(Experiment1_2)

//Constructor / Destructor
public:
    Experiment1_2();
    virtual ~Experiment1_2();

//Methods
public:
    void DrawParallelPlot();

private:

    virtual QWidget* createViewer();

//Attributes
private:
//    vector <Vector2f> Data;   //Keeps track of data coming from the input file
    Vector2f Data_Size;       //Records number of rows and columns of the data
    vector <Vector3f> Data_Attr; //Keeps track of the max and min value of each column as well as the data being 
                                 // plotted at present
    string csv_filename;       //String that contains the path to the csv file

    float32 XScale;            //Scaling X
    float32 YScale;            //Scaling Y
    float32 Alpha;             //Scaling Alpha of the plotted lines

protected:
    GLGeometryViewer* viewer;
};


#endif
