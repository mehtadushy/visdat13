//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ExampleExperiment3DGraphics.h"
//---------------------------------------------------------------------------
#include "Properties.h"
#include "GLGeometryViewer3D.h"
#include "GeoXOutput.h"
//---------------------------------------------------------------------------

IMPLEMENT_GEOX_CLASS( ExampleExperiment3DGraphics ,0)
{
    BEGIN_CLASS_INIT( ExampleExperiment3DGraphics );
    ADD_NOARGS_METHOD(ExampleExperiment3DGraphics::addTriangles)
}

QWidget *ExampleExperiment3DGraphics::createViewer()
{
    viewer = new GLGeometryViewer3D();
    return viewer;
}

ExampleExperiment3DGraphics::ExampleExperiment3DGraphics()
{
    viewer = NULL;
}

ExampleExperiment3DGraphics::~ExampleExperiment3DGraphics() {}

void ExampleExperiment3DGraphics::addTriangles()
{
    // first way to describe a point
    Point3D p1;
    p1.position  = makeVector3f(-3.0f,1.0f,1.0f);
    int p1Handle = viewer->addPoint(p1);

    // second way to describe a point
    Point3D p2(makeVector3f(-1.0f,1.0f,-2.0f));
    int p2Handle = viewer->addPoint(p2);

    // third way to describe a point
    Point3D p3(-1.0f,-1.0f,3.0f);
    int p3Handle = viewer->addPoint(p3);

    // then declare connectivity
    Triangle t1;
    t1.vertices[0] = p1Handle;
    t1.vertices[1] = p2Handle;
    t1.vertices[2] = p3Handle;
    viewer->addTriangle(t1);

    // or directly describe a triangle, be aware of introducing multiple new vertices
    viewer->addTriangle(makeVector3f(1.0,1.0,0.0),makeVector3f(3.0,1.0,3.0),makeVector3f(3.0,1.0,1.0));

    // display changes
    viewer->refresh();
}

