
//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "Experiment5_1.h"
//---------------------------------------------------------------------------
#include "Properties.h"
#include "GLGeometryViewer.h"
#include "GeoXOutput.h"
//---------------------------------------------------------------------------

#include <limits>
#include "Field2.hpp"

IMPLEMENT_GEOX_CLASS( Experiment5_1, 0)
{
    BEGIN_CLASS_INIT( Experiment5_1 );

    //ADD_SEPARATOR("Scalarfield")
    //ADD_STRING_PROP(ScalarfieldFilename, 0)
    //ADD_NOARGS_METHOD(Experiment5_1::DrawScalarField)

    //ADD_SEPARATOR("Vectorfield")
    //ADD_STRING_PROP(VectorfieldFilename, 0)
    //ADD_FLOAT32_PROP(ArrowScale, 0)
    //ADD_NOARGS_METHOD(Experiment5_1::DrawVectorField)

    //ADD_SEPARATOR("Texture")
    //ADD_STRING_PROP(ImageFilename, 0)
    //ADD_BOOLEAN_PROP(bColoredTexture, 0)
    
	//Add Fields for inserting the values:
	ADD_SEPARATOR("Start Position")
	ADD_FLOAT32_PROP(XStart, 0)
	ADD_FLOAT32_PROP(YStart, 0)
	ADD_FLOAT32_PROP(MaxDistance, 0)
	ADD_SEPARATOR("Euler")
	ADD_FLOAT32_PROP(EulerStepSize, 0)
	ADD_FLOAT32_PROP(EulerSteps, 0)
	ADD_SEPARATOR("Rung Kutta")
	ADD_FLOAT32_PROP(RKStepSize, 0)
	ADD_FLOAT32_PROP(RKSteps, 0)
	
	ADD_NOARGS_METHOD(Experiment5_1::DrawTexture)
	ADD_NOARGS_METHOD(Experiment5_1::EulerStreamlines)
    ADD_NOARGS_METHOD(Experiment5_1::RungeKuttaStreamlines)

}

QWidget* Experiment5_1::createViewer()
{
    viewer = new GLGeometryViewer();
    return viewer;
}

Experiment5_1::Experiment5_1()
{
    viewer = NULL;
    ScalarfieldFilename = "";
    VectorfieldFilename = "";
    ArrowScale = 0.1;
    ImageFilename = "";
    bColoredTexture = true;
	XStart=1;
	YStart=0;
	MaxDistance=5.3;
	EulerStepSize=0.1;
	EulerSteps=100;
	RKStepSize=0.3;
	RKSteps=30;
}

Experiment5_1::~Experiment5_1() {}
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
void Experiment5_1::EulerStreamlines()
{
	Vector2f x1 = makeVector2f(XStart,YStart);
	viewer->clear();
	for (int i=0;i<EulerSteps;i++)
	{

		Point2D p1, p2;
		p1.position=x1;
		p1.color=makeVector4f(0,1,0,0.7);
		viewer->addPoint(p1);

		x1[0]=x1[0]- EulerStepSize * p1.position[1];
		x1[1]=x1[1]+ EulerStepSize * p1.position[0]/2;

		p2.position=x1;
		p2.color=makeVector4f(0,1,0,0.7);
		viewer->addPoint(p2);

		viewer->addLine(p1.position,p2.position,makeVector4f(1,0,0,0.7),2);

	}
		viewer -> refresh();
}
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
Vector2f func_v(Vector2f x)
{

	Vector2f vx;
    vx[0]= -x[1];
    vx[1]= x[0]/2;
    return vx;

}

void Experiment5_1::RungeKuttaStreamlines()
{
	
	Vector2f x1 = makeVector2f(XStart,YStart);
	for (int i=0;i<RKSteps;i++)	
	{
		Point2D p1, p2;
		p1.position=x1;
		p1.color=makeVector4f(1,1,0,.7);
		viewer->addPoint(p1);
		
		Vector2f v1=func_v(x1);
		Vector2f v2=func_v(x1+v1*RKStepSize/2);
		Vector2f v3=func_v(x1+v2*RKStepSize/2);
		Vector2f v4=func_v(x1+v3*RKStepSize);
		x1=x1 + (v1/6+ v2/3 + v3/3 + v4/6)*RKStepSize;

		p2.position=x1;
		p2.color=makeVector4f(1,1,0,.7);
		viewer->addPoint(p2);
		viewer->addLine(p1.position,p2.position,makeVector4f(1,0,0.5,0.7),2);

	}
		viewer -> refresh();

}

void Experiment5_1::DrawScalarField()
{
    viewer->clear();

    //Load scalar field
    ScalarField2 field;
    if (!field.load(ScalarfieldFilename))
    {
        output << "Error loading field file " << ScalarfieldFilename << "\n";
        return;
    }

    //Get the minimum/maximum value in that field
    float32 min = std::numeric_limits<float32>::max();
    float32 max = -std::numeric_limits<float32>::max();
    for(size_t j=0; j<field.dims()[1]; j++)
    {
        for(size_t i=0; i< field.dims()[0]; i++)
        {
            const float32 val = field.nodeScalar(i,j);
            min = val < min ? val : min;
            max = val > max ? val : max;
        }
    }

    //Draw a point for each grid vertex.
    for(size_t j=0; j<field.dims()[1]; j++)
    {
        for(size_t i=0; i<field.dims()[0]; i++)
        {
            const float32 val = field.nodeScalar(i, j);
            const float32 c = (val - min) / (max - min);

            Point2D p;
            p.position  = field.nodePosition(i, j);
            p.size = 5;
            //Use a grayscale depending on the actual value
            p.color[0] = c; p.color[1] = c; p.color[2] = c;
            viewer->addPoint(p);
        }
    }

    viewer->refresh();
}


void Experiment5_1::DrawVectorField()
{
    viewer->clear();

    //Load the vector field
    VectorField2 field;
    if (!field.load(VectorfieldFilename))
    {
        output << "Error loading field file " << VectorfieldFilename << "\n";
        return;
    }

    //Draw vector directions (constant length)
    for(float32 x=field.boundMin()[0]; x<=field.boundMax()[0]; x+=0.2)
    {
        for(float32 y=field.boundMin()[1]; y<=field.boundMax()[1]; y+=0.2)
        {
            Vector2f vec = field.sample(x,y);
            vec.normalize();

            viewer->addLine(x, y, x + ArrowScale*vec[0], y + ArrowScale*vec[1]);
        }
    }

    viewer->refresh();
}


namespace
{
    ///Returns the next power-of-two
    int32 NextPOT(int32 n)
    {
        n--;
        n |= n >> 1;   // Divide by 2^k for consecutive doublings of k up to 32,
        n |= n >> 2;   // and then or the results.
        n |= n >> 4;
        n |= n >> 8;
        n |= n >> 16;
        n++;           // The result is a number of 1 bits equal to the number
                       // of bits in the original number, plus 1. That's the
                       // next highest power of 2.
        return n;
    }
}


void Experiment5_1::DrawTexture()
{
    viewer->clear();

    //Load the texture using Qt
    QImage image(ImageFilename.c_str());

    //Get its (original) dimensions. Used as bounds later.
    const float fWidth = (float)image.width();
    const float fHeight = (float)image.height();

    //Resize to power-of-two and mirror.
    image = image.mirrored().scaled(NextPOT(image.width()), NextPOT(image.height()));

    //Get its new integer dimensions.
    const int iWidth = image.width();
    const int iHeight = image.height();

    if (bColoredTexture)
    {
        //Create three color channels for the texture
        //Each of them is represented using a scalar field
        ScalarField2 Red;
        Red.init(makeVector2f(-fWidth, -fHeight), makeVector2f(fWidth, fHeight), makeVector2ui(iWidth, iHeight));
        ScalarField2 Green;
        Green.init(makeVector2f(-fWidth, -fHeight), makeVector2f(fWidth, fHeight), makeVector2ui(iWidth, iHeight));
        ScalarField2 Blue;
        Blue.init(makeVector2f(-fWidth, -fHeight), makeVector2f(fWidth, fHeight), makeVector2ui(iWidth, iHeight));

        //Fill the scalar fields
        for(size_t j=0; j<Red.dims()[1]; j++)
        {
            for(size_t i=0; i<Red.dims()[0]; i++)
            {
                Red.setNodeScalar(i, j, (float)(qRed(image.pixel(i, j))) / 255.0 );
                Green.setNodeScalar(i, j, (float)(qGreen(image.pixel(i, j))) / 255.0 );
                Blue.setNodeScalar(i, j, (float)(qBlue(image.pixel(i, j))) / 255.0 );
            }
        }

        //Set the texture in the viewer
        viewer->setTextureRGB(Red.getData(), Green.getData(), Blue.getData());
    }
    else
    {
        //Create one gray color channel represented as a scalar field
        ScalarField2 Gray;
        Gray.init(makeVector2f(-fWidth, -fHeight), makeVector2f(fWidth, fHeight), makeVector2ui(iWidth, iHeight));

        //Set the values at the vertices
        for(size_t j=0; j<Gray.dims()[1]; j++)
        {
            for(size_t i=0; i<Gray.dims()[0]; i++)
            {
                Gray.setNodeScalar(i, j, (float)(qGray(image.pixel(i, j))) / 255.0 );
            }
        }

        //Set the texture in the viewer
        viewer->setTextureGray(Gray.getData());
    }

    viewer->refresh();
}
