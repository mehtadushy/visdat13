//---------------------------------------------------------------------------
#ifndef Experiment5_1H
#define Experiment5_1H
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
class Experiment5_1 : public Experiment
{
    GEOX_CLASS(Experiment5_1)

//Constructor / Destructor
public:
    Experiment5_1();
    virtual ~Experiment5_1();

//Methods
public:
    void DrawScalarField();
    void DrawVectorField();
    void DrawTexture();
    virtual QWidget* createViewer();
	
	// implementation of first question
	void EulerStreamlines(); 
	void RungeKuttaStreamlines(); 
	//Vector2f func_v(Vector2f);

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

	// implementation of the attributes for Euler % Kutta method
	float XStart;
	float YStart;
	float MaxDistance;
	float EulerStepSize;
	float EulerSteps;
	float KStepSize;
    float RKStepSize;
	float RKSteps;

protected:
    GLGeometryViewer* viewer;
};


#endif
