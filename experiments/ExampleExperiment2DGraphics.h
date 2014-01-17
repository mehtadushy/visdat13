//---------------------------------------------------------------------------
#ifndef ExampleExperiment2DGraphicsH
#define ExampleExperiment2DGraphicsH
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
class ExampleExperiment2DGraphics : public Experiment
{
    GEOX_CLASS(ExampleExperiment2DGraphics)

//Constructor / Destructor
public:
    ExampleExperiment2DGraphics();
    virtual ~ExampleExperiment2DGraphics();

//Methods
public:
    void drawSquares();
    virtual QWidget* createViewer();

//Attributes
protected:
    GLGeometryViewer* viewer;
};

#endif
