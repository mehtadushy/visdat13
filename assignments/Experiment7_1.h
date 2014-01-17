//---------------------------------------------------------------------------
#ifndef Experiment7_1H
#define Experiment7_1H
//---------------------------------------------------------------------------
#include "Experiment.h"
#include "LinearAlgebra.h"
#include "GLGeometryViewer.h"
//---------------------------------------------------------------------------
#include "Field2.hpp"



///
class Experiment7_1 : public Experiment
{
    GEOX_CLASS(Experiment7_1)
//Constructor / Destructor
public:
    Experiment7_1();
    virtual ~Experiment7_1();
public:
    void LoadVectorField();     //Load the Vector field
    void GenerateRandomTexture();     //Generate a random texture using a seed number as input, as well
                                      // as the mean and SD of the distribution, along with the ability to select
                                      // between black and white and the dimensions of the the texture. The function
                                      // also displays the vector field
                                      
    void LoadTexture();             //Load an image and scale it appropriately for use as a texture
    void ShowFlowLIC();	             //Execute LIC and display the updated texture
    void ShowFlowFastLIC();	     //Execute Fast LIC and display the updated texture
private:
    bool rk4_forward(Vector2f &X);    //Return the next point on the streamline given the current point after arclength l. Return 'false' if hit boundary
    bool rk4_back(Vector2f &X);       //Return the next point on the streamline given the current point after arclength l
    Vector2f func_v(Vector2f);         //Return the value of the vector field at the point.
    virtual QWidget* createViewer();

//Attribute
private:
    string vector_filename;      //String that contains the path to the Vector Field file
    string texture_filename;     //String that contains the path to the Texture file, if using an input texture
    VectorField2 field;          //The vector field
    float32 field_max_mag;
    float32 field_min_mag;
    ScalarField2 texture_field;  //Scalar Field that contains the texture.

    int32 texture_size;         //Texture size is (2^texture_size)x(2^texture_size)
    
    int32 texture_seed;          // Seed value that goes into srand() for generation of the random texture
    bool bw_texture;             // 1: Random Texture is Black and White    0: Random texture is grayscale
    float32 texture_mean;        // Mean of the distribution       0 to 1
    float32 texture_variance;    // Variance of the distribution   0 to 1
    float32 bw_thresh;

    int32 kernel_size;         //L. The kernel goes from -L to L-1.

    bool color_texture;
    float32 color_factor;

  

    //RK4 Parameters
    float32 step_size;            //The step size to use for RK4. The smaller of x or y size of a pixel would give the step size.
    int32 max_steps;              //The maximum number of steps to integrate with
    float32 arc_length;           //The smaller of x or y size of a pixel would also give the arc length.
    float32 field_val_tolerance;  //The tolerance for the field value



protected:
    GLGeometryViewer* viewer;



};


#endif
