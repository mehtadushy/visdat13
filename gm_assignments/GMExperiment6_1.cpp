//---------------------------------------------------------------------------
#include "stdafx.h"
#include <cmath>
//---------------------------------------------------------------------------
#include "GMExperiment6_1.h"
//---------------------------------------------------------------------------
#include "Properties.h"
#include "GLGeometryViewer.h"
#include "GeoXOutput.h"
#include <fstream>
#include <iomanip>
#include <limits>

#include <QDebug>
#include <QVector>

#include <Eigen/Dense>

//---------------------------------------------------------------------------

// Interpolation and curve fitting

int solveCubic(double c[4], double s[3]);


IMPLEMENT_GEOX_CLASS( GMExperiment6_1, 0)
{
   BEGIN_CLASS_INIT( GMExperiment6_1 );
   ADD_INT32_PROP(gauss_support,0)
   ADD_INT32_PROP(curv_support,0)
   ADD_FLOAT32_PROP(tangent_filter_angle,0)
   ADD_INT32_PROP(knot_filter,0)
   ADD_SEPARATOR("Data file")
   ADD_STRING_PROP(filename,0)
   ADD_BOOLEAN_PROP(persist_on,0)
   ADD_BOOLEAN_PROP(display_control_points,0)
   ADD_BOOLEAN_PROP(gradient_descent,0)
   ADD_BOOLEAN_PROP(least_squares, 0)
   ADD_INT32_PROP(num_iterations, 0)
   ADD_INT32_PROP(segment_id, 0)

   ADD_NOARGS_METHOD(GMExperiment6_1::loadData)  //
   ADD_NOARGS_METHOD(GMExperiment6_1::smoothenData)    //
   ADD_NOARGS_METHOD(GMExperiment6_1::displayRawData)   //
   ADD_NOARGS_METHOD(GMExperiment6_1::displaySmoothData)   //
  // ADD_NOARGS_METHOD(GMExperiment6_1::displayCurvature)   //
  ADD_NOARGS_METHOD(GMExperiment6_1::displayBezierSpline)   //
}
QWidget* GMExperiment6_1::createViewer()
{
    viewer = new GLGeometryViewer();
    return viewer;
}

GMExperiment6_1::GMExperiment6_1()
{
    viewer = NULL;
    gauss_support = 2;
    curv_support = 8;
    persist_on = true;
    display_control_points = true;
    tangent_filter_angle = 90;
    gradient_descent = true;
    knot_filter = 5;
    least_squares = false;
    num_iterations = 200;
    segment_id = -1;
    filename = "/home/dushy/Desktop/VisDat13/ExampleData/CamelHeadSilhouette.txt";
}


GMExperiment6_1::~GMExperiment6_1() {}

static Vector4f getColor(float val, float min_val, float max_val)
{
    std::vector<Vector4f> colours;
    colours.push_back(makeVector4f(0, 0, 1, 1));
    colours.push_back(makeVector4f(1, 1, 1, 1));
    colours.push_back(makeVector4f(1, 0, 0, 1));

    float step = (max_val - min_val)/(colours.size()-1);
    for(int i = 0; i < colours.size() - 1; ++i)
    {
        float a = min_val + step*i;
        float b = a + step;
        if(val <= b)
        {
            auto x = (val - a)/step;
            return colours[i]*(1-x)+colours[i+1]*x;
        }
    }
    return colours[colours.size()-1];
}

vector<float> formGaussKernel(int support_size)
{
   //Size of the gauss kernel comes from the interface
   //via support_size
   vector<float> gaussKernel(support_size);
   float norm_factor = pow(2.0, - (support_size-1));
   gaussKernel[0] = norm_factor;
   gaussKernel[support_size-1] = norm_factor;
   for(int i=1; i< (support_size+1)/2; i++)
   {
    gaussKernel[i] = gaussKernel[i-1] * (support_size-i)/i;
    gaussKernel[support_size-1-i] = gaussKernel[i];
   }
   return gaussKernel;
   //gaussKernel=gaussKernel*norm_factor;
   //output<<"Gausskernel:";
   //float sum=0;
   //for (vector<float>::const_iterator j=gaussKernel.begin(); j!=gaussKernel.end(); ++j)
   //{
   //   output <<*j<<" ";
   //   sum+=*j;
   //}
   //output<< "  Sum:"<<sum<<"\n";
}


// uniform initialisation
template<typename T> T initVal();
template<> float initVal<float>() { return 0.0; }
template<> Vector2f initVal<Vector2f>() { return makeVector2f(0.0, 0.0); }

template<typename T>
vector<T> smoothenData(const vector<T> &rawData, int32 support)
{
    vector<float> gaussKernel = formGaussKernel(2*support + 1);
    std::vector<T> smoothData(rawData.size());
    vector<T> tempData(rawData.size()+2*support);

    //Create a temporary vector with mirrored boundaries
    std::copy(rawData.begin(), rawData.end(), tempData.begin()+support);
    std::reverse_copy(rawData.begin(), rawData.begin()+support,tempData.begin());
    std::reverse_copy(rawData.end()-support,rawData.end(), tempData.end()-support);

    for(int i=0; i< smoothData.size(); i++)
    {
          T tempVec = initVal<T>();
          for(int j=-support; j<=support; j++)
          {
              tempVec += tempData[i+j+support] * gaussKernel[j+support];
          }
          smoothData[i] = tempVec;
    }
    return smoothData;
}


void GMExperiment6_1::loadData()
{
   ifstream datafile(filename.c_str());
   if(!datafile.is_open())
   {
     output << "Couldn't Open Data File\n";
   }
   else
   {
     rawData.clear();
     Vector2f temp;
     while(datafile >> temp[0] >> temp[1])
     {
     rawData.push_back(temp);
     }
     datafile.close();
   }

}

void GMExperiment6_1::smoothenData()
{
    smoothData = ::smoothenData(rawData, gauss_support);

    curvature = computeCurvature(smoothData);
    curvature = ::smoothenData(curvature, curv_support);
}

void GMExperiment6_1::displayRawData()
{
    displayData(rawData, makeVector4f(1.0, 0, 0, 1));
}

static double Bernstein3(int i, double t)
{
    double coeff[4] = {1,3,3,1};
    return coeff[i] * pow(t, i) * pow(1-t, 3-i);
}

Vector2f firstDerivative(const std::vector<Vector2f> &vec, int idx)
{
    if(idx == 0)
        return (vec[1]-vec[0]);
    else if(idx == (vec.size()-1))
        return vec[idx]-vec[idx-1];
    else
        return (vec[idx+1]-vec[idx-1])*0.5;
}

static Vector2f to(Vector2d p)
{
    return makeVector2f(p[0], p[1]);
}

static Vector2d to(Vector2f p)
{
    return makeVector2d(p[0], p[1]);
}

static vector<Vector2f> renderCubicBezier(vector<Vector2d> p, int num)
{
    vector<Vector2f> res;
    res.push_back(to(p[0]));
    for(int i = 0; i < num-2; ++i)
    {
        int idx = i+1;

        double t = double(idx)/(num-1);
        auto b0 = Bernstein3(0, t);
        auto b1 = Bernstein3(1, t);
        auto b2 = Bernstein3(2, t);
        auto b3 = Bernstein3(3, t);

        auto pc = to(p[0]*b0 + p[1]*b1 + p[2]*b2 + p[3]*b3);
        res.push_back(pc);
    }
    res.push_back(to(p[3]));
    return res;
}

static void displayCurve(GLGeometryViewer *viewer, const vector<Vector2f> &vec,
                         float thickness = 1, Vector4f colour = makeVector4f(0, 1, 1, 1))
{
    for(int k = 0; k < vec.size()-1; ++k)
        viewer->addLine(vec[k], vec[k+1], colour, thickness);
}

// rough approximation of the distance from a point to curve (squared)
static double distToCurve(Vector2f pt, const vector<Vector2f> &curve)
{
    double min_distance = numeric_limits<double>::max();
    for(int i = 1; i < curve.size()-1; ++i)
       min_distance = std::min(min_distance, double((curve[i]-pt).getSqrNorm()));
      return sqrt(min_distance);
}

static double totalMeanError(const vector<Vector2f> &dataPoints, const vector<Vector2f> &curve)
{
    double distance = 0.0;
    for(int i = 1; i < dataPoints.size()-1; ++i)
        distance += distToCurve(dataPoints[i], curve);
    distance /= (dataPoints.size()-2);
    return distance;
}

template <typename T> static int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

double dot(Vector2d a, Vector2d b)
{
    return a[0]*b[0]+a[1]*b[1];
}

static Vector2d ctrl2s(const vector<Vector2d> &p, const vector<Vector2d> &d)
{
    Vector2d res;
    res[0] = dot(d[0], p[1]-p[0]);
    res[1] = dot(d[1], p[2]-p[3]);
    return res;
}

static vector<Vector2d> s2ctrl(Vector2d p0, Vector2d p3, const vector<Vector2d> &d,
                               Vector2d s)
{
    vector<Vector2d> res(4);
    res[0] = p0;
    res[1] = p0 + d[0]*s[0];
    res[2] = p3 + d[1]*s[1];
    res[3] = p3;
    return res;
}


struct GrParams
{
    Vector2d p0, p3; // start and end points
    vector<Vector2d> d; // tangent vectors
    vector<Vector2f> points; // data points to fit
};

static double errorFunc(const GrParams &param, Vector2d s)
{
    int blow = 40;
    auto p = s2ctrl(param.p0, param.p3, param.d, s);
    auto curve = renderCubicBezier(p, std::max(int(param.points.size()), int(blow)));
    return totalMeanError(param.points, curve);
}


pair<double, Vector2d> errorGradient(const GrParams &param, Vector2d s)

{
    const int num_var = 2;

    Vector2d res;
    auto fval = errorFunc(param, s);
    // choose dh for approximating derivative that is appropriate
    // if we are already close to minimum(error is smalll)
    const float h = min(fval, 0.01);
    for(int k = 0; k < num_var; ++k)
    {
        auto s_h = s;
        s_h[k] += h;
        auto f_shift = errorFunc(param, s_h);

        res[k] = (f_shift-fval)/h;
    }
    return make_pair(fval, res);
}

static Vector2d gradientDescent(vector<Vector2d> p, vector<Vector2d> d,
                                const vector<Vector2f> points,
                                int num_iter)
{
    GrParams param;
    param.p0 = p[0];
    param.p3 = p[3];
    param.points = points;
    param.d = d;

    auto s = ctrl2s(p, d);

    for(int k = 0; k < num_iter; ++k)
    {
        auto stuff = errorGradient(param, s);
        s += -stuff.second*stuff.first;
    }
    return s;
}


void GMExperiment6_1::tangent_check(int32 start_idx, int32 end_idx, vector<int> & knots, vector<Vector2f> &smoothData)
{
    Vector2f tangent_begin = firstDerivative(smoothData,start_idx);
    tangent_begin.normalize();
    //tangent_begin.normalise();
    for(int i=start_idx+1;i<end_idx; i++)
    {
           Vector2f tangent = firstDerivative(smoothData,i);
           tangent.normalize();
       if( (tangent[0]*tangent_begin[0]+tangent[1]*tangent_begin[1]) < cos(tangent_filter_angle*M_PI/180.0) )//if tangents off by more than 90 deg
       {
           knots.push_back(i);
           tangent_check(i,end_idx,knots,smoothData);
           break;
       }
    }
}

void GMExperiment6_1::displaySmoothData()
{
    // draw Bezier curve
    bool colourise = true;
    auto colour = makeVector4f(0, 1.0, 0, 1);
    vector<Vector4f> colours(smoothData.size());

    float min_curv = *std::min_element(curvature.begin(), curvature.end());
    float max_curv = *std::max_element(curvature.begin(), curvature.end());

    float max_mag = std::max(fabs(min_curv), fabs(max_curv));
    max_mag = 10;

    for(int i = 0; i < smoothData.size(); ++i)
        colours[i] = colourise ? getColor(curvature[i], -max_mag, max_mag) : colour;
    displayData(smoothData, colours);
}
vector<Vector2f> interpolatePoints(const vector<Vector2f> &points)
{
    vector<Vector2f> newpts;
    for(int i = 0; i < points.size()-1; ++i)
    {
        newpts.push_back(points[i]);
        newpts.push_back((points[i]+points[i+1])*0.5);
    }
    newpts.push_back(points[points.size()-1]);
    return newpts;
}

Vector2d leastSquaresEstimate(vector<Vector2f> points,
                              Vector2f p0, Vector2f p3,
                              Vector2d d1, Vector2d d2)
{
    // hack up points
    if(points.size() <= 10)
        points = interpolatePoints(points);
    if(points.size() <= 20)
        points = interpolatePoints(points);

    qDebug() << "num points in arc" << points.size();
    int m = points.size()-2; // number of points to fit
    int n = 2;               // number of control points to find

    Eigen::MatrixXd Z(m, n);
    Eigen::MatrixXd Y(m, n);
    Eigen::VectorXd pu(m);
    Eigen::VectorXd pv(m);

    for(int i = 0; i < m; ++i)


    {
        int idx = i+1;
        Vector2f pt = points[idx];

        double t = double(idx)/(m-1);
        auto b0 = Bernstein3(0, t);
        auto b1 = Bernstein3(1, t);
        auto b2 = Bernstein3(2, t);
        auto b3 = Bernstein3(3, t);

        // Filling in the matrices
        Z(i, 0) = b1*d1[0];
        Z(i, 1) = b2*d2[0];

        Y(i, 0) = b1*d1[1];
        Y(i, 1) = b2*d2[1];

        auto pp = pt - p0*(b0 + b1) - p3*(b2 + b3);
        pu(i) = pp[0];
        pv(i) = pp[1];
    }

    auto Zt = Z.transpose();
    auto Yt = Y.transpose();
    auto A = Zt*Z+Yt*Y;

    auto rhs = Zt*pu+Yt*pv;
    Eigen::VectorXd ans = A.inverse()*rhs;

    auto s = makeVector2d(ans(0), ans(1));
    qDebug() << "least squares" << s[0] << s[1];
    s[0]=fabs(s[0])/**10*/;
    s[1]=fabs(s[1]);
    return s;
}
void GMExperiment6_1::displayBezierSpline()
{
    //Trying a new way of computing inflection points
    //First look for long straight bits on the curve and add
    //one of the end points to the list of knots
    float inflection_thresh = 1.5f;
    int32 inflection_filter = 4;
    Vector2i inflection_range;
    inflection_range[0] = -1; inflection_range[1]= -1;
    vector<Vector2i> searchRanges;
    vector<int> inflection_idx;

    for(int i = 0; i < curvature.size(); ++i)
    {
        if(abs(curvature[i]) < inflection_thresh)
        { if(inflection_range[0]<0)
            { inflection_range[0]=i;
            }
        }
        else
        {
            if((inflection_range[0]>=0)) //Infection range was being observed
            {
                inflection_idx.push_back(inflection_range[0]);
                inflection_range[1]=i;
                //inflection_idx.push_back(inflection_range[1]);
                searchRanges.push_back(inflection_range);
                inflection_range[0]=-1;
                inflection_range[1]=-1;
            }
        }
    }


    //Now add endpoints to the vector and filter out any indices extremely close to each other
    //See if the endpoints already exist. If not, then push them onto the data structure
    if(inflection_idx.empty())
    {
        inflection_idx.push_back(0);
        inflection_idx.push_back(curvature.size()-1);
    }
    else
    {
        if(inflection_idx[0]!=0)
        {
            inflection_idx.insert( inflection_idx.begin(), 0);
        }
        if(inflection_idx[inflection_idx.size()-1] != curvature.size()-1)
        {
            inflection_idx.push_back(curvature.size()-1);
        }
    }

    qDebug()<<"Number of inflection range points "<<inflection_idx.size()<<"\n";

    //Then look for sign changes in the segments not spanned by the long straight curves
    //and add those and sort the vector
    if(!searchRanges.empty())
    {
        if(searchRanges[0][0]!=0)
        {
            searchRanges.insert(searchRanges.begin(),makeVector2i(0,0));
        }
        if(searchRanges[searchRanges.size()-1][1]!=curvature.size()-1)
        {
            searchRanges.push_back(makeVector2i(curvature.size()-1,curvature.size()-1));
        }

        for(int i=0; i<searchRanges.size()-1; i++)
        {
            bool old_sign = curvature[searchRanges[i][1]]>0;
            for(int j=searchRanges[i][1]+1; j<searchRanges[i+1][0]; j++)
            {
                bool new_sign = curvature[j]>0;
                if(new_sign!=old_sign)
                {
                    inflection_idx.push_back(j);
                }
                old_sign=new_sign;
            }
        }
#if 1
    for(int i=0; i<inflection_idx.size()-1; i++)
    {
       if(i < inflection_idx.size()-2)
       {
           if(( inflection_idx[i+1]-inflection_idx[i]) < inflection_filter)
           {
            inflection_idx.erase(inflection_idx.begin()+i+1);
           }
       }
       else
       {
           if(( inflection_idx[i+1]-inflection_idx[i]) < inflection_filter)
           {
            inflection_idx.erase(inflection_idx.begin()+i);
           }
       }
    }
#endif
    }
    sort(inflection_idx.begin(),inflection_idx.end());
    vector<int> knots;
    knots = inflection_idx;
    //knots.push_back(inflection_idx[0]);
    //knots.push_back(inflection_idx[inflection_idx.size()-1]);

    //for(int i=1; i<inflection_idx.size()-1; i+=2)
    //{ knots.push_back(inflection_idx[i]);
    //}
    //Do the tangent check between inflection points and add to knots
    for(int j=0; j<inflection_idx.size()-1; j++)
    {
            tangent_check(inflection_idx[j],inflection_idx[j+1],knots,smoothData);
    }

    std::sort(knots.begin(), knots.end());

#if 1
    for(int i=0; i<knots.size()-1; i++)
    {
       if(i < knots.size()-2)
       {
           if(( knots[i+1]-knots[i]) < knot_filter)
           {
            knots.erase(knots.begin()+i+1);
           }
       }
       else
       {
           if(( knots[i+1]-knots[i]) < knot_filter)
           {
            knots.erase(knots.begin()+i);
           }
       }
    }
#endif
    output<<"Total Number of control points (knots):"<< knots.size()<<"\n";


    int start = 0;
    int end = knots.size()-1;

    if(segment_id != -1)

    {
        start = segment_id;
        end = segment_id+1;
    }

    for(int j = start; j < end; ++j)

    {
        int i_start = knots[j];
        int i_end = knots[j+1];

        auto p0 = smoothData[i_start];
        auto p3 = smoothData[i_end];

        // estimate derivatives
        vector<Vector2d> d(2);
        d[0] = to(firstDerivative(smoothData, i_start));
        d[0].normalize();
        d[1] = -to(firstDerivative(smoothData, i_end));
        d[1].normalize();

        vector<Vector2f> points;
        for(int k = knots[j]; k <= knots[j+1]; k++)
            points.push_back(smoothData[k]);



//        if(points.size() < 6)
//            continue;

        Vector2d s;
        if(least_squares)
        {
            s = leastSquaresEstimate(points, p0, p3, d[0], d[1]);
        }
        else
        {
            s[0] = sqrt((points[1]-points[0]).getSqrNorm());
            s[1] = sqrt((points[points.size()-1]-points[points.size()-2]).getSqrNorm());
        }

        auto ctrl = s2ctrl(to(p0), to(p3), d, s);

        if(gradient_descent)
        {
            auto new_s = gradientDescent(ctrl, d, points, num_iterations);
            ctrl = s2ctrl(to(p0), to(p3), d, new_s);
        }

        // display the curve!
        auto piece = renderCubicBezier(ctrl, std::max(points.size(), size_t(20)));
        displayCurve(viewer, piece, 2);

        // display control points
        int pt_indices[4];
        if(display_control_points)
        {
            for(int k = 0; k < 4; ++k)
            {
                Point2D pt;
                pt.position = to(ctrl[k]);

                bool is_knot = k == 0 || k == 3;
                pt.size = is_knot ? 8 : 6;
                pt.color = is_knot ? makeVector4f(0, 1.0, 0, 1) :
                                     makeVector4f(1, 0, 1, 1);
                pt_indices[k] = viewer->addPoint(pt);
            }
            Line l;
            l.thickness = 1;
            l.color = makeVector4f(1,1,1,1);
            l.vertices[0] = pt_indices[0];
            l.vertices[1] = pt_indices[1];
            viewer->addLine(l);
            l.vertices[0] = pt_indices[2];
            l.vertices[1] = pt_indices[3];
            viewer->addLine(l);
        }
    }


    viewer->refresh();
}

void GMExperiment6_1::displayData(vector<Vector2f> &data, Vector4f color, int pt_size)
{
    if(!persist_on)
    { viewer->clear();
    }
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

void GMExperiment6_1::displayData(const vector<Vector2f> &data, const vector<Vector4f> &color)
{
    if(!persist_on)
        viewer->clear();

    for(int k=0; k< data.size(); k++)
    {
        Point2D pt;
        pt.color = color[k];
        pt.position = data[k];
        viewer->addPoint(pt);
    }
    viewer->refresh();
}

vector<float> GMExperiment6_1::computeCurvature(const vector<Vector2f> &data)
{
    vector<float> curvature(data.size());
    for(int i = 0; i < data.size(); ++i)
    {
        Vector2f c1,c2;
        if(i == 0)
        {
            c1 = data[1]-data[0];
            continue;
        }
        else if(i == (data.size()-1))
        {
            c1 = data[data.size()-1] - data[data.size()-2];
            continue;
        }
        else
        {
            c1 = (data[i+1]-data[i-1])*0.5;
            c2 = data[i+1]-data[i]*2+data[i-1];
        }

        curvature[i] = (-c2[0]*c1[1] + c2[1]*c1[0])/pow(c1.getSqrNorm(), 1.5);
    }
    curvature[0] = curvature[1];
    curvature[data.size()-1] = curvature[data.size()-2];
    return curvature;
}

