//---------------------------------------------------------------------------
#ifndef Experiment9_1H
#define Experiment9_1H
//---------------------------------------------------------------------------
#include "Experiment.h"
#include "LinearAlgebra.h"
#include "GLGeometryViewer.h"
//---------------------------------------------------------------------------
#include "Field2.hpp"



///
class Experiment9_1 : public Experiment
{
    GEOX_CLASS(Experiment9_1)
//Constructor / Destructor
public:
    Experiment9_1();
    virtual ~Experiment9_1();
public:
    void LoadVectorField();           //Load vector field and reset "critical_pt_avail" flag
    void LoadScalarField();           //Load scalar field, compute its gradient and reset "critical_pt_avail" flag
    void ShowCriticalPoints();       
    void ShowSeparatrices();
    void GenerateRandomTexture();     //Generate a random texture using a seed number as input, as well
                                      // as the mean and SD of the distribution, along with the ability to select
                                      // between black and white and the dimensions of the the texture. The function
                                      // also displays the vector field
                                      
    void ShowFlowLIC();	             //Execute LIC and display the updated texture
    void DrawStreamModulated();
    void OverlayGrid();
private:
    bool rk4_forward(Vector2f &X);    //Return the next point on the streamline given the current point after arclength l. Return 'false' if hit boundary
    bool rk4_back(Vector2f &X);       //Return the next point on the streamline given the current point after arclength l
    Vector2f func_v(Vector2f);         //Return the value of the vector field at the point.
    virtual QWidget* createViewer();

    int classify_critical_points(const Vector2f &);    //Checks the determinant condition (det(J)!=0) for critical points and finds their properties. 
                                        //Vector field comes from "field" and the information regarding critical points and their properties 
                                        //goes into "CriticalPoints"
    void get_critical_points();         //Looks for points where the vector field goes to 0 and pushes them onto "CriticalPoints".
                                        //Sets "critical_pt_avail" flag to 1
    bool sign_test(vector <Vector2f>);
    void draw_stream_rk4(Vector2f X_Seed, bool direction);

//Attribute
private:
    string vector_filename;      //String that contains the path to the Vector Field file
    string scalar_filename;      //String that contains the path to the Scalar Field file.
                                 //For the scalar field, a gradient map is computed and put into "field"
    VectorField2 field;          //The vector field
    
    vector <Vector3f> CriticalPoints;  //Keeps track of critical points and their properties 
    bool critical_pt_avail;

    //LIC Related Stuff
    ScalarField2 texture_field;  //Scalar Field that contains the texture.

    int32 texture_size;         //Texture size is (2^texture_size)x(2^texture_size)
    int32 texture_seed;          // Seed value that goes into srand() for generation of the random texture
    bool bw_texture;             // 1: Random Texture is Black and White    0: Random texture is grayscale
    float32 texture_mean;        // Mean of the distribution       0 to 1
    float32 texture_variance;    // Variance of the distribution   0 to 1
    float32 bw_thresh;
    int32 kernel_size;         //L. The kernel goes from -L to L-1.

    //Streamline Related Stuff
    bool show_seeds;             // 1: Show Seed Points    0: Don't show Seed Points
    bool persist_on;             // 1: Don't clear the screen before drawing  0: Clear the screen before drawing
    int num_seeds;               // Number of seeds for random streams
    float32 stream_alpha;        // Alpha of streamlines
    Vector4f stream_color;
    int32 stream_width;
    bool draw_stream;           //Flag used to indicate to RK4 if a streamline is to be drawn
    
    //RK4 Parameters
    float32 step_size;            //The step size to use for RK4. The smaller of x or y size of a pixel would give the step size.
    int32 max_steps;              //The maximum number of steps to integrate with
    float32 arc_length;           //The smaller of x or y size of a pixel would also give the arc length for LIC
    float32 field_val_tolerance;  //The tolerance for the field value



protected:
    GLGeometryViewer* viewer;



};


#endif
