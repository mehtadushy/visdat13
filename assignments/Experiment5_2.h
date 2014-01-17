//---------------------------------------------------------------------------
#ifndef Experiment5_2H
#define Experiment5_2H
//---------------------------------------------------------------------------
#include "Experiment.h"
#include "LinearAlgebra.h"
#include "GLGeometryViewer.h"
//---------------------------------------------------------------------------
#include "Field2.hpp"



///
class Experiment5_2 : public Experiment
{
    GEOX_CLASS(Experiment5_2)

//Constructor / Destructor
public:
    Experiment5_2();
    virtual ~Experiment5_2();

//Methods
public:
    void DrawVectorField();
    virtual QWidget* createViewer();
	void LoadVectorField();
	Vector2f rk4_forward(Vector2f);
    Vector2f rk4_backward(Vector2f);
	Vector2f func_v(Vector2f);
	void draw_stream_rk4();
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

	//the point of the seed
	float XStart;
	float YStart;
    VectorField2 field;
	bool field_type;
	float step_size;
	Vector4f stream_color;
	float stream_alpha;
	float stream_width;
	float arc_length;
	float arc_length_change;
	float velocity_tolerance;
	int max_steps;
	bool direction;
protected:
    GLGeometryViewer* viewer;
};


#endif
