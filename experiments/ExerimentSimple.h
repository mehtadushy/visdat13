//---------------------------------------------------------------------------
#ifndef ExerimentSimpleH
#define ExerimentSimpleH
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
class ExerimentSimple : public Experiment
{
    GEOX_CLASS(ExerimentSimple)

//Constructor / Destructor
public:
    ExerimentSimple();
    virtual ~ExerimentSimple();

//Methods
public:
    void DrawLine();
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
