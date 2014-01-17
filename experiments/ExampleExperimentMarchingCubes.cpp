//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ExampleExperimentMarchingCubes.h"
//---------------------------------------------------------------------------
#include "Properties.h"
#include "GLGeometryViewer3D.h"
#include "MarchingCubes3D.h"
//---------------------------------------------------------------------------

#define rnd01() (((double) rand()) / RAND_MAX)

IMPLEMENT_GEOX_CLASS( ExampleExperimentMarchingCubes ,0)
{
    BEGIN_CLASS_INIT( ExampleExperimentMarchingCubes );
    ADD_NOARGS_METHOD(ExampleExperimentMarchingCubes::showPerVertexLighting)
    ADD_NOARGS_METHOD(ExampleExperimentMarchingCubes::applyMarchingCubes)
}

QWidget *ExampleExperimentMarchingCubes::createViewer()
{
   viewer = new GLGeometryViewer3D();
   return viewer;
}

ExampleExperimentMarchingCubes::ExampleExperimentMarchingCubes() 
{
    viewer = NULL;
}

ExampleExperimentMarchingCubes::~ExampleExperimentMarchingCubes() {}

void ExampleExperimentMarchingCubes::showPerVertexLighting() 
{
    viewer->clear();

    //llf
    Point3D p0;
    p0.position = makeVector3f(-2.0f,-2.0f,-2.0f);
    p0.color = makeVector4f(0.0f,0.0f,0.0f,0.0f);
    p0.normal = -normalize(makeVector3f(-1.0f,-1.0f,-1.0f));
    int p0Handle = viewer->addPoint(p0);
    
    //lrf
    Point3D p1;
    p1.position = makeVector3f(2.0f,-2.0f,-2.0f);
    p1.color = makeVector4f(1.0f,0.0f,0.0f,0.0f);
    p1.normal = -normalize(makeVector3f(1.0f,-1.0f,-1.0f));
    int p1Handle = viewer->addPoint(p1);
    
    //urf
    Point3D p2;
    p2.position = makeVector3f(2.0f,2.0f,-2.0f);
    p2.color = makeVector4f(1.0f,1.0f,0.0f,0.0f);
    p2.normal = -normalize(makeVector3f(1.0f,1.0f,-1.0f));
    int p2Handle = viewer->addPoint(p2);
    
    //ulf
    Point3D p3;
    p3.position = makeVector3f(-2.0f,2.0f,-2.0f);
    p3.color = makeVector4f(0.0f,1.0f,0.0f,0.0f);
    p3.normal = -normalize(makeVector3f(-1.0f,1.0f,-1.0f));
    int p3Handle = viewer->addPoint(p3);
    
    //llb
    Point3D p4;
    p4.position = makeVector3f(-2.0f,-2.0f,2.0f);
    p4.color = makeVector4f(0.0f,0.0f,1.0f,0.0f);
    p4.normal = -normalize(makeVector3f(-1.0f,-1.0f,1.0f));
    int p4Handle = viewer->addPoint(p4);

    //lrb
    Point3D p5;
    p5.position = makeVector3f(2.0f,-2.0f,2.0f);
    p5.color = makeVector4f(1.0f,0.0f,1.0f,0.0f);
    p5.normal = -normalize(makeVector3f(1.0f,-1.0f,1.0f));
    int p5Handle = viewer->addPoint(p5);

    //urb
    Point3D p6;
    p6.position = makeVector3f(2.0f,2.0f,2.0f);
    p6.color = makeVector4f(1.0f,1.0f,1.0f,0.0f);
    p6.normal = -normalize(makeVector3f(1.0f,1.0f,1.0f));
    int p6Handle = viewer->addPoint(p6);

    //ulb
    Point3D p7;
    p7.position = makeVector3f(-2.0f,2.0f,2.0f);
    p7.color = makeVector4f(0.0f,1.0f,1.0f,0.0f);
    p7.normal = -normalize(makeVector3f(-1.0f,1.0f,1.0f));
    int p7Handle = viewer->addPoint(p7);

    // front
    Triangle t0;
    t0.vertices[0] = p0Handle;
    t0.vertices[1] = p1Handle;
    t0.vertices[2] = p2Handle;
    int t0Handle = viewer->addTriangle(t0);
    
    Triangle t1;
    t1.vertices[0] = p2Handle;
    t1.vertices[1] = p3Handle;
    t1.vertices[2] = p0Handle;
    int t1Handle = viewer->addTriangle(t1);

    // right
    Triangle t2;
    t2.vertices[0] = p1Handle;
    t2.vertices[1] = p5Handle;
    t2.vertices[2] = p6Handle;
    int t2Handle = viewer->addTriangle(t2);

    Triangle t3;
    t3.vertices[0] = p6Handle;
    t3.vertices[1] = p2Handle;
    t3.vertices[2] = p1Handle;
    int t3Handle = viewer->addTriangle(t3);
    
    // left
    Triangle t4;
    t4.vertices[0] = p0Handle;
    t4.vertices[1] = p3Handle;
    t4.vertices[2] = p7Handle;
    int t4Handle = viewer->addTriangle(t4);

    Triangle t5;
    t5.vertices[0] = p7Handle;
    t5.vertices[1] = p4Handle;
    t5.vertices[2] = p0Handle;
    int t5Handle = viewer->addTriangle(t5);

    // back
    Triangle t6;
    t6.vertices[0] = p6Handle;
    t6.vertices[1] = p5Handle;
    t6.vertices[2] = p4Handle;
    int t6Handle = viewer->addTriangle(t6);

    Triangle t7;
    t7.vertices[0] = p4Handle;
    t7.vertices[1] = p7Handle;
    t7.vertices[2] = p6Handle;
    int t7Handle = viewer->addTriangle(t7);

    // top
    Triangle t8;
    t8.vertices[0] = p3Handle;
    t8.vertices[1] = p2Handle;
    t8.vertices[2] = p6Handle;
    int t8Handle = viewer->addTriangle(t8);

    Triangle t9;
    t9.vertices[0] = p6Handle;
    t9.vertices[1] = p7Handle;
    t9.vertices[2] = p3Handle;
    int t9Handle = viewer->addTriangle(t9);

    // bottom
    Triangle t10;
    t10.vertices[0] = p5Handle;
    t10.vertices[1] = p1Handle;
    t10.vertices[2] = p0Handle;
    int t10Handle = viewer->addTriangle(t10);

    Triangle t11;
    t11.vertices[0] = p0Handle;
    t11.vertices[1] = p4Handle;
    t11.vertices[2] = p5Handle;
    int t11Handle = viewer->addTriangle(t11);

    // display changes
    viewer->refresh();
}

void ExampleExperimentMarchingCubes::applyMarchingCubes()
{
    viewer->clear();

    Vector3f corners[8];
    corners[0] = makeVector3f(-2.0f,-2.0f,-2.0f);
    corners[1] = makeVector3f( 2.0f,-2.0f,-2.0f);
    corners[2] = makeVector3f( 2.0f, 2.0f,-2.0f);
    corners[3] = makeVector3f(-2.0f, 2.0f,-2.0f);
    corners[4] = makeVector3f(-2.0f,-2.0f, 2.0f);
    corners[5] = makeVector3f( 2.0f,-2.0f, 2.0f);
    corners[6] = makeVector3f( 2.0f, 2.0f, 2.0f);
    corners[7] = makeVector3f(-2.0f, 2.0f, 2.0f);

    float32 values[8];
    for(int i = 0; i < 8; i++)
        values[i] = rnd01();

    vector<Vector3f> vertices;

    MarchingCubes3D::triangulate(corners, values, 0.5, vertices);
    for (unsigned i = 0; i < vertices.size(); i++ )
        viewer->addPoint( Point3D(vertices[i]) );

    for (int j = 0; j < (int) (viewer->getNumberOfPoints() / 3); j++)
    {
        Triangle t;
        for(int k = 0; k < 3;k++)
            t.vertices[k] = j*3+k;
        viewer->addTriangle(t);
    }

    // display changes
    viewer->refresh();

}
