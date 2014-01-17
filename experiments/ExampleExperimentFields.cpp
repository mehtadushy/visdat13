//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ExampleExperimentFields.h"
//---------------------------------------------------------------------------
#include "Properties.h"
#include "GLGeometryViewer.h"
#include "GeoXOutput.h"
//---------------------------------------------------------------------------

#include <limits>
#include "Field2.hpp"

IMPLEMENT_GEOX_CLASS( ExampleExperimentFields, 0)
{
    BEGIN_CLASS_INIT( ExampleExperimentFields );

    ADD_SEPARATOR("Scalarfield")
    ADD_STRING_PROP(ScalarfieldFilename, 0)
    ADD_NOARGS_METHOD(ExampleExperimentFields::DrawScalarField)

    ADD_SEPARATOR("Vectorfield")
    ADD_STRING_PROP(VectorfieldFilename, 0)
    ADD_FLOAT32_PROP(ArrowScale, 0)
    ADD_NOARGS_METHOD(ExampleExperimentFields::DrawVectorField)

    ADD_SEPARATOR("Texture")
    ADD_STRING_PROP(ImageFilename, 0)
    ADD_BOOLEAN_PROP(bColoredTexture, 0)
    ADD_NOARGS_METHOD(ExampleExperimentFields::DrawTexture)
}

QWidget* ExampleExperimentFields::createViewer()
{
    viewer = new GLGeometryViewer();
    return viewer;
}

ExampleExperimentFields::ExampleExperimentFields()
{
    viewer = NULL;
    ScalarfieldFilename = "";
    VectorfieldFilename = "";
    ArrowScale = 0.1;
    ImageFilename = "";
    bColoredTexture = true;
}

ExampleExperimentFields::~ExampleExperimentFields() {}


void ExampleExperimentFields::DrawScalarField()
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


void ExampleExperimentFields::DrawVectorField()
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


void ExampleExperimentFields::DrawTexture()
{
    viewer->clear();

    //Load the texture using Qt
    QImage image(ImageFilename.c_str());
    //Get its (original) dimensions. Used as bounds later.
    const float fWidth = (float)image.width();
    const float fHeight = (float)image.height();

    output<<"Image Dimensions"<<fWidth<<" "<<fHeight<<"\n";

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

         output<<"Im Dimensions"<<iWidth<<" "<<iHeight<<"\n";
        //Set the texture in the viewer
        viewer->setTextureGray(Gray.getData());
    }

    viewer->refresh();
}
