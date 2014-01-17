//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "Experiment1_1.h"
//---------------------------------------------------------------------------
#include "Properties.h"
#include "GLGeometryViewer.h"
#include "GeoXOutput.h"
//---------------------------------------------------------------------------

IMPLEMENT_GEOX_CLASS( Experiment1_1, 0)
{
    BEGIN_CLASS_INIT( Experiment1_1 );
    ADD_NOARGS_METHOD(Experiment1_1::CreateData_Random)
    ADD_NOARGS_METHOD(Experiment1_1::DrawScatterPlot)
}

QWidget* Experiment1_1::createViewer()
{
    viewer = new GLGeometryViewer();
    return viewer;
}

Experiment1_1::Experiment1_1()
{
    viewer = NULL;
}

Experiment1_1::~Experiment1_1() {}


void Experiment1_1::CreateData_Random()
{
    Data.clear();
    Data.resize(1000);
    for(int i=0;i<(int)Data.size();i++)
    {
        Data[i][0] = (float)rand() / RAND_MAX;
        Data[i][1] = (float)rand() / RAND_MAX;
    }
}


void Experiment1_1::DrawScatterPlot()
{
    for(int i=0;i<(int)Data.size();i++)
    {
        Point2D NewPoint(Data[i][0], Data[i][1]);
        NewPoint.color = makeVector4f(Data[i][0], Data[i][1], 0.5, 1);
        viewer->addPoint(NewPoint);
    }

    //Axes
    Point2D Origin(0, 0);
    Origin.color = makeVector4f(1,1,1,1);
    Origin.size = 10;
    const int idOrigin = viewer->addPoint(Origin);

    Point2D XOff(1.1, 0);
    XOff.color = makeVector4f(1,0,0,1);
    XOff.size = 10;
    const int idXOff = viewer->addPoint(XOff);

    Point2D YOff(0, 1.1);
    YOff.color = makeVector4f(0,1,0,1);
    YOff.size = 10;
    const int idYOff = viewer->addPoint(YOff);

    //X-Axis
    Line Axis;
    Axis.vertices[0] = idOrigin;
    Axis.vertices[1] = idXOff;
    Axis.color = makeVector4f(1,1,1,1);
    Axis.thickness = 3;
    viewer->addLine(Axis);

    //Y-Axis
    Axis.vertices[1] = idYOff;
    viewer->addLine(Axis);

    // display changes
    viewer->refresh();
}

