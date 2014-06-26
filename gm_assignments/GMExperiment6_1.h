//---------------------------------------------------------------------------
#ifndef GMExperiment6_1H
#define GMExperiment6_1H
//---------------------------------------------------------------------------
#include "Experiment.h"
#include "LinearAlgebra.h"
#include "DynamicLinearAlgebra.h"
#include "IterativeSolvers.h"
#include "GLGeometryViewer.h"
#include <utility>
//---------------------------------------------------------------------------
// Bezier Spline Interpolation Idea Try 1
// Read in bezier points, smoothen with a gaussean and compute and display the
// curvature of this smoothened curve.
// Then compute the inflexion points and display them on the curve.
///
class GMExperiment6_1 : public Experiment
{
    GEOX_CLASS(GMExperiment6_1)
//Constructor / Destructor
public:
    GMExperiment6_1();
    virtual ~GMExperiment6_1();
public:
    void loadData();
    void smoothenData();
    void displayRawData();
    void displaySmoothData();
    //void displayCurvature();
    //void displayInflextionPoints();
    void displayBezierSpline();
private:
    // Internal functions
    void displayData(vector<Vector2f> & data, Vector4f color, int pt_size = 3);
    void displayData(const vector<Vector2f> & data, const vector<Vector4f> &color);
    //void displayData(vector<Vector2f> & data, vector<Vector4f> & color);
    vector<float> computeCurvature(const vector<Vector2f> &data); //Computes curvature on smooth data
    void formGaussKernel(int support_size);
    void tangent_check(int32 start_idx, int32 end_idx, vector<int> & knots, vector<Vector2f> &smoothData);

private:
    virtual QWidget* createViewer();

//Attributes
private:
    int32 gauss_support;    //Size of the gaussean kernel for smoothing data
    int32 curv_support;
    int32 knot_filter;
    float32 tangent_filter_angle;
    string filename;
    bool persist_on;
    bool display_control_points;
    //Internal stuff
    vector<Vector2f> rawData;
    vector<Vector2f> smoothData;
    vector<float> curvature;

    bool gradient_descent;
    bool least_squares;
    int num_iterations;
    int segment_id;
protected:
    GLGeometryViewer* viewer;


};


#endif
