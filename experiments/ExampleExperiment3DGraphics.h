//---------------------------------------------------------------------------
#ifndef ExampleExperiment3DGraphicsH
#define ExampleExperiment3DGraphicsH
//---------------------------------------------------------------------------
#include "Experiment.h"
#include "LinearAlgebra.h"
#include "GLGeometryViewer3D.h"
//---------------------------------------------------------------------------

///
/// This is an example experiment.
///
/// The code is meant to demonstrate how
///  to use the GeoX framework
///
class ExampleExperiment3DGraphics : public Experiment
{
    GEOX_CLASS(ExampleExperiment3DGraphics)

//Constructor / Destructor
public:
    ExampleExperiment3DGraphics();
    virtual ~ExampleExperiment3DGraphics();

//Methods
public:
    void addTriangles();
    virtual QWidget *createViewer();

//Attributes
protected:
    GLGeometryViewer3D* viewer;
};


#endif                                         
