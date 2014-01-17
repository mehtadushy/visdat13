//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ExampleExperiment2DGraphics.h"
//---------------------------------------------------------------------------
#include "Properties.h"
#include "GLGeometryViewer.h"
#include "GeoXOutput.h"
//---------------------------------------------------------------------------

IMPLEMENT_GEOX_CLASS( ExampleExperiment2DGraphics, 0)
{
    BEGIN_CLASS_INIT( ExampleExperiment2DGraphics );
    ADD_NOARGS_METHOD(ExampleExperiment2DGraphics::drawSquares)
}

QWidget* ExampleExperiment2DGraphics::createViewer()
{
    viewer = new GLGeometryViewer();
    return viewer;
}

ExampleExperiment2DGraphics::ExampleExperiment2DGraphics()
{
    viewer = NULL;
}

ExampleExperiment2DGraphics::~ExampleExperiment2DGraphics() {}

void ExampleExperiment2DGraphics::drawSquares()
{
    //////////////////////////////////////////////////

    // describe left square
    // first add all points

    // first way to describe a point
    Point2D p1;
    p1.position  = makeVector2f(-3.0f,1.0f);
    int p1Handle = viewer->addPoint(p1);

    // second way to describe a point
    Point2D p2(makeVector2f(-1.0f,1.0f));
    int p2Handle = viewer->addPoint(p2);

    // third way to describe a point
    Point2D p3(-1.0f,-1.0f);
    int p3Handle = viewer->addPoint(p3);

    Point2D p4;
    p4.position  = makeVector2f(-3.0f,-1.0f);
    int p4Handle = viewer->addPoint(p4);

    // then declare connectivity
    Line l1;
    l1.vertices[0] = p1Handle;
    l1.vertices[1] = p2Handle;
    viewer->addLine(l1);
    Line l2;
    l2.vertices[0] = p2Handle;
    l2.vertices[1] = p3Handle;
    viewer->addLine(l2);
    Line l3;
    l3.vertices[0] = p3Handle;
    l3.vertices[1] = p4Handle;
    viewer->addLine(l3);
    Line l4;
    l4.vertices[0] = p4Handle;
    l4.vertices[1] = p1Handle;
    viewer->addLine(l4);

    //////////////////////////////////////////////////

    // right square
    // add lines immediately
    // but this adds new points for each line

    // first way to directly describe a line
    // - default color and default width
    viewer->addLine(makeVector2f(1.0,1.0), makeVector2f(3.0,1.0));
    // - red color and default width
    viewer->addLine(makeVector2f(3.0,1.0), makeVector2f(3.0,-1.0), makeVector4f(1,0,0,1));
    // - red color & semi-transparent and width of 5 pixels
    viewer->addLine(makeVector2f(3.0,1.0), makeVector2f(3.0,-1.0), makeVector4f(1,0,0,0.5), 5);

    //second way to directly describe a line
    viewer->addLine(3.0,-1.0, 1.0,-1.0, makeVector4f(1,0.5f,0,1), 3);
    viewer->addLine(1.0,-1.0,1.0,1.0);

    //////////////////////////////////////////////////

    // display changes
    viewer->refresh();
}

