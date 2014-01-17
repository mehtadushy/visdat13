//---------------------------------------------------------------------------
#ifndef ExampleExperimentFieldsH
#define ExampleExperimentFieldsH
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
class ExampleExperimentFields : public Experiment
{
    GEOX_CLASS(ExampleExperimentFields)

//Constructor / Destructor
public:
    ExampleExperimentFields();
    virtual ~ExampleExperimentFields();

//Methods
public:
    void DrawScalarField();
    void DrawVectorField();
    void DrawTexture();
    virtual QWidget* createViewer();

//Attributes
public:
    ///File name of the scalar field
    string ScalarfieldFilename;

    ///File name of the vector field
    string VectorfieldFilename;

    ///Length of the arrows
    float ArrowScale;

    ///File name of the image for the texture
    string ImageFilename;

    ///Whether to draw the texture in RGB or grayscale
    bool bColoredTexture;

protected:
    GLGeometryViewer* viewer;
};


#endif
