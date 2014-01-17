//---------------------------------------------------------------------------
#ifndef ExampleExperimentScatterPlotH
#define ExampleExperimentScatterPlotH
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
class ExampleExperimentScatterPlot : public Experiment
{
    GEOX_CLASS(ExampleExperimentScatterPlot)

//Constructor / Destructor
public:
    ExampleExperimentScatterPlot();
    virtual ~ExampleExperimentScatterPlot();

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
