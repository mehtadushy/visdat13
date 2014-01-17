//---------------------------------------------------------------------------
#ifndef Experiment1_1H
#define Experiment1_1H
//---------------------------------------------------------------------------
#include "Experiment.h"
#include "LinearAlgebra.h"
#include "GLGeometryViewer.h"
//---------------------------------------------------------------------------


/// This is an example experiment.
///
/// The code is meant to demonstrate how
///  to use the GeoX framework
///
class Experiment1_1 : public Experiment
{
    GEOX_CLASS(Experiment1_1)

//Constructor / Destructor
public:
    Experiment1_1();
    virtual ~Experiment1_1();

//Methods
public:
    void CreateData_Random();
    void DrawScatterPlot();
    virtual QWidget* createViewer();

//Attributes
public:
    vector< Vector2f > Data;

protected:
    GLGeometryViewer* viewer;
};


#endif
