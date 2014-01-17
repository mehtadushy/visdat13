//---------------------------------------------------------------------------
#ifndef ExampleExperimentMarchingCubesH
#define ExampleExperimentMarchingCubesH
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
class ExampleExperimentMarchingCubes : public Experiment
{
   GEOX_CLASS(ExampleExperimentMarchingCubes)
    private:
        GLGeometryViewer3D* viewer;
    public:
        ExampleExperimentMarchingCubes();
        ~ExampleExperimentMarchingCubes();
        void showPerVertexLighting();
        void applyMarchingCubes();
        virtual QWidget *createViewer();
};


#endif
