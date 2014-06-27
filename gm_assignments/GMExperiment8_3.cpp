
//---------------------------------------------------------------------------
#include "stdafx.h"
#include <cmath>
//---------------------------------------------------------------------------
#include "GMExperiment8_3.h"
//---------------------------------------------------------------------------
#include "Properties.h"
#include "GLGeometryViewer.h"
#include "GeoXOutput.h"
//---------------------------------------------------------------------------

// Ellipse and Farin Experiment
// Terrible Horrible Code


IMPLEMENT_GEOX_CLASS( GMExperiment8_3, 0)
{
   BEGIN_CLASS_INIT( GMExperiment8_3 );
   ADD_SEPARATOR("Num of points")
   ADD_INT32_PROP(num_samples,0)
   ADD_INT32_PROP(num_fine_triangle,0)
   ADD_SEPARATOR("Curve Properties")
   ADD_VECTOR3F_PROP(p0,0)
   ADD_VECTOR3F_PROP(p1,0)
   ADD_VECTOR3F_PROP(p2,0)
   ADD_VECTOR3F_PROP(q0,0)
   ADD_VECTOR3F_PROP(q1,0)
   ADD_VECTOR3F_PROP(q2,0)
   ADD_FLOAT32_PROP(wp,0)
   ADD_FLOAT32_PROP(wq,0)
   ADD_BOOLEAN_PROP(persist_on,0)

   ADD_NOARGS_METHOD(GMExperiment8_3::populateCurves)  //
   ADD_NOARGS_METHOD(GMExperiment8_3::drawRuled)    //
   ADD_NOARGS_METHOD(GMExperiment8_3::drawRuledBetter)    //
}
QWidget* GMExperiment8_3::createViewer()
{
    viewer = new GLGeometryViewer3D();
    return viewer;
}

GMExperiment8_3::GMExperiment8_3()
{
    viewer = NULL;
    num_samples=10;
    num_fine_triangle = 6;
    wp=0.5;
    wq=1.5;
    persist_on = true;

    p0[0]=1.0;
    p0[1]=0.0;
    p0[2]=1.0;
    p1[0]=1.0;
    p1[1]=1.0;
    p1[2]=1.0;
    p2[0]=0.0;
    p2[1]=1.0;
    p2[2]=1.0;
    q0[0]=1.0;
    q0[1]=0.0;
    q0[2]=0.0;
    q1[0]=1.0;
    q1[1]=1.0;
    q1[2]=0.0;
    q2[0]=0.0;
    q2[1]=1.0;
    q2[2]=0.0;
}


GMExperiment8_3::~GMExperiment8_3() {}
template <typename T>  //T is vector2 or vector 3
vector<T> get_bezier_points(vector<T> points,vector<float> weights, int num_samples)
{
    if(points.size()!=weights.size())
    {
        cout<<"Mismatch between size of point vector and weight vector in rational decasteljau algo\n";
    }

    int bezier_order = points.size();
    vector<T> curve_pts;
    curve_pts.resize(num_samples);

    vector<T> coeff_vec;
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
void GMExperiment8_3::populateCurves()
{
     vector<Vector3f> p_bez;
     vector<Vector3f> q_bez;
     p_bez.push_back(p0);
     p_bez.push_back(p1);
     p_bez.push_back(p2);
     q_bez.push_back(q0);
     q_bez.push_back(q1);
     q_bez.push_back(q2);
     vector<float> p_w,q_w;
     p_w.push_back(1.0);
     p_w.push_back(wp);
     p_w.push_back(1.0);
     q_w.push_back(1.0);
     q_w.push_back(wq);
     q_w.push_back(1.0);

     curvep=get_bezier_points( p_bez, p_w, num_samples);
     curveq=get_bezier_points( q_bez, q_w, num_samples);

      if(!persist_on)
      {
          viewer->clear();
      }
      draw_polygon(curvep);
      draw_polygon(curveq);
}


void GMExperiment8_3::drawRuled()
{
   for(int i =0; i< num_samples-1; i++)
   {
       viewer->addTriangle(curvep[i], curvep[i+1], curveq[i]);
       viewer-> addTriangle(curvep[i+1],curveq[i+1], curveq[i]);
   }
   viewer->refresh();

}
void GMExperiment8_3::drawRuledBetter()
{
   for(int i =0; i< num_samples-1; i++) //for each strip
   {
       Vector3f inc1, inc2;
       inc1 = (curveq[i] - curvep[i])/num_fine_triangle;
       inc2 = (curveq[i+1] - curvep[i+1])/num_fine_triangle;

       for(int k = 0; k< num_fine_triangle; k++)
       {
           viewer->addTriangle(curvep[i]+(inc1*k), curvep[i+1]+(inc2*k), curvep[i]+(inc1*(k+1)));
           viewer->addTriangle(curvep[i+1]+(inc2*k), curvep[i+1]+(inc2*(k+1)), curvep[i]+(inc1*(k+1)));
       }
   }
   viewer->refresh();

}

void GMExperiment8_3::draw_polygon(vector<Vector3f> data )
{
    for(int k=0; k< data.size()-1; k++)
    {
        viewer->addLine(data[k], data[k+1]);
    }
    viewer->refresh();
}


