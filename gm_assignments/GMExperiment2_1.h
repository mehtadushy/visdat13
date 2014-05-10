//---------------------------------------------------------------------------
#ifndef GMExperiment2_1H
#define GMExperiment2_1H
//---------------------------------------------------------------------------
#include "Experiment.h"
#include "LinearAlgebra.h"
#include "DynamicLinearAlgebra.h"
#include "IterativeSolvers.h"
#include "GLGeometryViewer.h"
#include <utility>
//---------------------------------------------------------------------------
//

///
class GMExperiment2_1 : public Experiment
{
    GEOX_CLASS(GMExperiment2_1)
//Constructor / Destructor
public:
    GMExperiment2_1();
    virtual ~GMExperiment2_1();
public:
    void drawParabola();
    void drawTangentNormal();
    void interpPolynomial();     //Draws a polynomial of degree num_pts-1
    void fitPolynomial();        //Fits a polynomial of degree interp_deg to the points
    //void clearDisplay();       //Clear everything
    //void clearPolynomial();    //Clear the drawn polynomial
private:
    // Internal functions
    void populate_parabola();
    void render_parabola();
    void render_polynomial(DynamicVector<float> & poly_coeff, int n_points , pair<float,float> xrange, Vector4f color);

private:
    virtual QWidget* createViewer();

//Attributes
private:
    int32 num_pts;     //Number of points in the parabola
    float32 coeff;     //Coefficient a in y=a*x^2
    int32 selected_pt; //Point slection for drawing the normal and the tangent
    int32 interp_deg;  //Interpolator degree
    bool persist_on;   //Clear display before drawing

    //Internal stuff
    vector<Vector2f> parabola;       //x,y pairs for the parabola
    DynamicVector<float> poly_coeff;     //Coefficients of the interpolated polygon
    DynamicVector<float> fit_poly_coeff; //Coefficients of the fit polygon

protected:
    GLGeometryViewer* viewer;


};


#endif
