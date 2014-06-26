
//---------------------------------------------------------------------------
#include "stdafx.h"
#include <cmath>
//---------------------------------------------------------------------------
#include "GMExperiment8_1.h"
//---------------------------------------------------------------------------
#include "Properties.h"
#include "GLGeometryViewer.h"
#include "GeoXOutput.h"
//---------------------------------------------------------------------------

// Ellipse and Farin Experiment
// Terrible Horrible Code


IMPLEMENT_GEOX_CLASS( GMExperiment8_1, 0)
{
   BEGIN_CLASS_INIT( GMExperiment8_1 );
   ADD_SEPARATOR("Num of points")
   ADD_INT32_PROP(num_samples,0)
   ADD_SEPARATOR("Ellipse Properties")
   ADD_VECTOR2F_PROP(scale_ellipse,0)
   ADD_FLOAT32_PROP(rotate_ellipse,0)
   ADD_VECTOR2F_PROP(translate_ellipse,0)
   ADD_BOOLEAN_PROP(persist_on,0)

   ADD_NOARGS_METHOD(GMExperiment8_1::populateUnitCircle)  //
   ADD_NOARGS_METHOD(GMExperiment8_1::drawEllipse)    //
   ADD_NOARGS_METHOD(GMExperiment8_1::drawFarin)    //
}
QWidget* GMExperiment8_1::createViewer()
{
    viewer = new GLGeometryViewer();
    return viewer;
}

GMExperiment8_1::GMExperiment8_1()
{
    viewer = NULL;
    num_samples=100;
    scale_ellipse[0]= 1.0f;
    scale_ellipse[1]= 1.0f;
    translate_ellipse[0] = 0.0f;
    translate_ellipse[1] = 0.0f;
    rotate_ellipse = 0.0;
    persist_on = true;
}


GMExperiment8_1::~GMExperiment8_1() {}
template <typename T>  //T is vector2 or vector 3
vector<T> get_bezier_points(vector<T> points,vector<float> weights, int num_samples)
{
    if(points.size()!=weights.size())
    {
        cout<<"Mismatch between size of point vector and weight vector in rational decasteljau algo\n";
    }

    int bezier_order = points.size();
    vector<Vector2f> curve_pts;
    curve_pts.resize(num_samples);

    vector<Vector2f> coeff_vec;
    vector<float> weight_vec;
    //coefficients.resize(bezier_order);

    for(int k=0; k< num_samples; k++)  //t loop
    {
     float t = static_cast<float>(k)/(num_samples-1);

     //Populate weights and coefficient vector
     coeff_vec = points;
     weight_vec = weights;

     //Run through the pyramid now
     for(int j=0; j< bezier_order-1; j++)
     {
        vector<float> weight_vec_old;
        weight_vec_old = weight_vec;
        for(int i=0; i<bezier_order-1-j; i++)
        {
          weight_vec[i] = weight_vec[i]*t+weight_vec[i+1]*(1-t);
        }
        for(int i=0; i<bezier_order-1-j; i++)
        {
          coeff_vec[i] = (coeff_vec[i]*t*weight_vec_old[i]+  coeff_vec[i+1]*(1-t)*weight_vec_old[i+1])/weight_vec[i];
        }
     }
     curve_pts[k] = coeff_vec[0];
    }

   return curve_pts;
}
void GMExperiment8_1::populateUnitCircle()
{
   //3 Control points per segment
   //3 control segments in all
   ctrlpts_per_seg = 3;
   control_points.resize(9);
   weights.resize(9);
   control_points[0][0] = cos(M_PI/6);  control_points[8][0] = cos(M_PI/6);
   control_points[0][1] = sin(M_PI/6);  control_points[8][1] = sin(M_PI/6);
   control_points[2][0] = cos(M_PI*5/6); control_points[3][0] = cos(M_PI*5/6);
   control_points[2][1] = sin(M_PI*5/6); control_points[3][1] = sin(M_PI*5/6);
   control_points[5][0] = cos(M_PI*3/2);  control_points[6][0] = cos(M_PI*3/2);
   control_points[5][1] = sin(M_PI*3/2);  control_points[6][1] = sin(M_PI*3/2);

   control_points[1][0] = 0;
   control_points[1][1] = 1/cos(M_PI/3);
   control_points[4][0] = -cos(M_PI/6)/cos(M_PI/3);
   control_points[4][1] = -sin(M_PI/6)/cos(M_PI/3);
   control_points[7][0] = cos(M_PI/6)/cos(M_PI/3);
   control_points[7][1] = -sin(M_PI/6)/cos(M_PI/3);

   weights[0] = 1.0;
   weights[1] = 1.0/2.0;
   weights[2] = 1.0;
   weights[3] = 1.0;
   weights[4] = 1.0/2.0;
   weights[5] = 1.0;
   weights[6] = 1.0;
   weights[7] = 1.0/2.0;
   weights[8] = 1.0;

}


void GMExperiment8_1::drawEllipse()
{
    //Form the transformation matrix
    //The get modified control points

    DMatrixF rotation;
    DMatrixF scale;
    rotation.setDimension(2,2);
    scale.setDimension(2,2);
    rotation[0][0] = cos(rotate_ellipse);
    rotation[0][1] = -sin(rotate_ellipse);
    rotation[1][0] = sin(rotate_ellipse);
    rotation[1][1] = cos(rotate_ellipse);
    scale[0][0] = scale_ellipse[0];
    scale[0][1] = 0;
    scale[1][0] = 0;
    scale[1][1] = scale_ellipse[1];

    DMatrixF trans = rotation*scale;

    vector<Vector2f> affine_control(control_points.begin(), control_points.end());
    for( int i=0; i<affine_control.size(); i++)
    {
        affine_control[i][0] = trans[0][0] * affine_control[i][0] + trans[0][1] * affine_control[i][1]+ translate_ellipse[0];
        affine_control[i][1] = trans[1][0] * affine_control[i][0] + trans[1][1] * affine_control[i][1]+ translate_ellipse[1];
    }

    farin_points.clear();
    for( int i=0; i<affine_control.size(); i+=ctrlpts_per_seg)
    {
        for(int k=0; k<ctrlpts_per_seg-1; k++)
        {
            farin_points.push_back( (affine_control[i+k] * weights[i+k] +  affine_control[i+k+1] * weights[i+k+1])/(weights[i+k+1]+weights[i+k]));
        }
    };


    if(!persist_on)
    {
        viewer->clear();
    }

    //Draw the control polygons and the circle itself
    draw_polygon(vector<Vector2f>(affine_control.begin(), affine_control.begin()+3), makeVector4f(1.0,0,0,1));
    draw_polygon(vector<Vector2f>(affine_control.begin()+3, affine_control.begin()+6), makeVector4f(0,1.0,0,1));
    draw_polygon(vector<Vector2f>(affine_control.begin()+6, affine_control.end()), makeVector4f(0,0,1.0,1));

    draw_points(affine_control, makeVector4f(1.0,0,1.0,1), 5);

    vector<Vector2f> bezier_points;
    vector<Vector2f> temp;
    temp = get_bezier_points(vector<Vector2f>(affine_control.begin(), affine_control.begin()+3),
                                      vector<float>(weights.begin(), weights.begin()+3), num_samples/3) ;
    //bezier_points.insert(bezier_points.end(),temp.begin(),temp.end());
    draw_polygon(temp, makeVector4f(0.5, 0.5, 0.5, 0.9));
    temp = get_bezier_points(vector<Vector2f>(affine_control.begin()+3, affine_control.begin()+6),
                                      vector<float>(weights.begin()+3, weights.begin()+6), num_samples/3) ;
    //bezier_points.insert(bezier_points.end(),temp.begin(),temp.end());
    draw_polygon(temp, makeVector4f(0.5, 0.5, 0.5, 0.9));
    temp =  get_bezier_points(vector<Vector2f>(affine_control.begin()+6, affine_control.end()),
                                      vector<float>(weights.begin()+6, weights.end()), num_samples/3) ;
    //bezier_points.insert(bezier_points.end(),temp.begin(),temp.end());
    draw_polygon(temp, makeVector4f(0.5, 0.5, 0.5, 0.9));
    //draw_polygon(bezier_points, makeVector4f(0.5, 0.5, 0.5, 0.9));

}
void GMExperiment8_1::drawFarin()
{
    draw_points(farin_points, makeVector4f(0.0,1.0,1.0,1.0), 3);
}
void GMExperiment8_1::draw_points(vector<Vector2f> &data, Vector4f color, int pt_size)
{

    Point2D pt;
    pt.color = color;
    pt.size = pt_size;
    for(int k=0; k< data.size(); k++)
    {
        pt.position = data[k];
        viewer->addPoint(pt);
    }
    viewer->refresh();
}
void GMExperiment8_1::draw_polygon(vector<Vector2f> data, Vector4f color)
{
    for(int k=0; k< data.size()-1; k++)
    {
        viewer->addLine(data[k], data[k+1], color,1.0);
    }
    viewer->refresh();
}


