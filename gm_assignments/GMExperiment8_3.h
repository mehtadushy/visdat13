//---------------------------------------------------------------------------
#ifndef GMExperiment8_3H
#define GMExperiment8_3H
//---------------------------------------------------------------------------
#include "Experiment.h"
#include "LinearAlgebra.h"
#include "DynamicLinearAlgebra.h"
#include "IterativeSolvers.h"
#include "GLGeometryViewer3D.h"
#include <utility>
//---------------------------------------------------------------------------
// Draw an ellipse by applying affine transformations to a unit circle formed
// using 3 rational bezier segments
///
class GMExperiment8_3 : public Experiment
{
    GEOX_CLASS(GMExperiment8_3)
public:
    GMExperiment8_3();
    virtual ~GMExperiment8_3();
public:
    void populateCurves();
    void drawRuled();
    void drawRuledBetter();
private:
    // Internal functions
    void draw_polygon(vector<Vector3f> data);

private:
    virtual QWidget* createViewer();

//Attributes
private:
    Vector3f p0,p1,p2;
    Vector3f q0,q1,q2;
    float32 wp, wq;
    int32 num_samples;     //Number of points on the curves
    bool persist_on;
    int32 num_fine_triangle; //number of triangles each strip is to be divided into

    //Internal stuff
    vector<Vector3f> curvep,curveq;

protected:
    GLGeometryViewer3D* viewer;

};


#endif
