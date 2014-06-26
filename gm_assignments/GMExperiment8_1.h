//---------------------------------------------------------------------------
#ifndef GMExperiment8_1H
#define GMExperiment8_1H
//---------------------------------------------------------------------------
#include "Experiment.h"
#include "LinearAlgebra.h"
#include "DynamicLinearAlgebra.h"
#include "IterativeSolvers.h"
#include "GLGeometryViewer.h"
#include <utility>
//---------------------------------------------------------------------------
// Draw an ellipse by applying affine transformations to a unit circle formed
// using 3 rational bezier segments
///
class GMExperiment8_1 : public Experiment
{
    GEOX_CLASS(GMExperiment8_1)
public:
    GMExperiment8_1();
    virtual ~GMExperiment8_1();
public:
    void populateUnitCircle();
    void drawEllipse();
    void drawFarin();
private:
    // Internal functions
    void draw_polygon(vector<Vector2f> data, Vector4f color);
    void draw_points(vector<Vector2f> &data, Vector4f color, int pt_size);

private:
    virtual QWidget* createViewer();

//Attributes
private:
    Vector2f scale_ellipse;
    Vector2f translate_ellipse;
    float32 rotate_ellipse;
    int32 num_samples;     //Number of points on the curve
    bool persist_on;

    //Internal stuff
    vector<Vector2f> control_points;
    vector<float> weights;
    int32 ctrlpts_per_seg;
    vector<Vector2f> farin_points;

protected:
    GLGeometryViewer* viewer;


};


#endif
