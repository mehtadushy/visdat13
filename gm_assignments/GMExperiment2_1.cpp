//---------------------------------------------------------------------------
#include "stdafx.h"
#include <cmath>
//---------------------------------------------------------------------------
#include "GMExperiment2_1.h"
//---------------------------------------------------------------------------
#include "Properties.h"
#include "GLGeometryViewer.h"
#include "GeoXOutput.h"
//---------------------------------------------------------------------------

// Interpolation and curve fitting


IMPLEMENT_GEOX_CLASS( GMExperiment2_1, 0)
{
   BEGIN_CLASS_INIT( GMExperiment2_1 );
   ADD_SEPARATOR("Parabola Related")
   ADD_INT32_PROP(num_pts,0)
   ADD_FLOAT32_PROP(coeff,0)
   ADD_SEPARATOR("Tangent/Normal")
   ADD_INT32_PROP(selected_pt,0)   
   ADD_SEPARATOR("Poly Fit Related")
   ADD_INT32_PROP(interp_deg,0)
   ADD_SEPARATOR("Display related")
   ADD_BOOLEAN_PROP(persist_on,0)

   ADD_NOARGS_METHOD(GMExperiment2_1::drawParabola)         // Draw points of the parabola
   ADD_NOARGS_METHOD(GMExperiment2_1::drawTangentNormal)    // Draw tangent and normal at the selected point, if it is in range
   ADD_NOARGS_METHOD(GMExperiment2_1::interpPolynomial)     // Interpolate the drawn points with a polynomial
   ADD_NOARGS_METHOD(GMExperiment2_1::fitPolynomial)        // Fit a polynomial of degree  interp_deg to the parabola points
	
}
QWidget* GMExperiment2_1::createViewer()  
{
    viewer = new GLGeometryViewer();
    return viewer;
}

GMExperiment2_1::GMExperiment2_1()          
{
    viewer = NULL;
    num_pts=10;
    coeff=1.0f;
    selected_pt=5;
    interp_deg=8;
    persist_on=true;   
}


GMExperiment2_1::~GMExperiment2_1() {}

void GMExperiment2_1::populate_parabola()
{
    pair<float,float> parabolaRange(-2.0f,2.0f);
    parabola.resize(num_pts);
    float step_size = (parabolaRange.second-parabolaRange.first)/(num_pts-1);
    for(int i=0; i<num_pts; i++)
    {
	    parabola[i][0] = parabolaRange.first + i * step_size;
	    parabola[i][1] = coeff * parabola[i][0] *  parabola[i][0];
    }
}

void GMExperiment2_1::render_parabola()
{
     Vector4f point_color;
     point_color = makeVector4f(1.0f,0.2f,0.0f,0.7f);
     Point2D parPt;
     parPt.size = 4.0;
     parPt.color = point_color;

     for(int i=0; i<parabola.size(); i++)
     {
	 parPt.position=parabola[i];
	 viewer->addPoint(parPt);
     }
     viewer->refresh();
}

void GMExperiment2_1::drawParabola()
{
    populate_parabola();
    if(persist_on == false)
    {
      viewer->clear();
    }
    render_parabola();
}

void GMExperiment2_1::drawTangentNormal()
{
   Vector4f tangent_color;
   tangent_color = makeVector4f(1.0f,0.0f,0.0f,1.0f); //Red
   Vector4f normal_color;
   normal_color = makeVector4f(0.0f,1.0f,0.0f,1.0f); //Green
   float line_len = 1.0;

   
   //Check if the index of the point where the tangent and normal are to be drawn is within range
   if( (selected_pt < 1) || (selected_pt > parabola.size()-2) )
   {
	   output<<"Error! The selected_pt is out of range!!\n";
   }
   else
   {
	   //Compute the normal and the tangent and render as lines
	   //Tangent 
	   Vector2f tangent;
	   tangent = (parabola[selected_pt+1]-parabola[selected_pt-1]) /2;
	   tangent.normalize();
	   Vector2f normal;
	   normal = makeVector2f(tangent[1],-tangent[0]);
	   tangent*=line_len/2;
	   viewer->addLine(parabola[selected_pt]-tangent, parabola[selected_pt]+tangent,tangent_color,1);
	   viewer->addLine(parabola[selected_pt], parabola[selected_pt]+normal,normal_color,1);
   }
   viewer->refresh();

}

void GMExperiment2_1::interpPolynomial()
{
      poly_coeff.setDim(parabola.size());
      // First compute the coefficients of the polynomial
      DynamicMatrix<float>A(parabola.size(),parabola.size());
      DynamicVector<float>b(parabola.size());
      
      for(int i=0; i < parabola.size() ; i++) //Each column
      {
	 DynamicVector<float> &Acol(A[i]);
	 //Acol=A[i];
	 
	 for(int r=0; r < parabola.size(); r++)
	 {  
	     Acol[r]= pow(parabola[r][0],i);
	 }
      }

      for(int r=0; r < parabola.size(); r++)
      {  
          b[r]= parabola[r][1];
      }
     
      //gaussSeidelSolve<float>(A, poly_coeff,b,1000);
      poly_coeff = invertMatrix(A)*b;
      
      //Then render the polynomial
      render_polynomial(poly_coeff, 1000, pair<float,float>(-4.0,4.0), makeVector4f(0.0,0.0,1.0,0.9)); 

}

void GMExperiment2_1::render_polynomial(DynamicVector<float> & poly_coeff, int n_points , pair<float,float> xrange, Vector4f color)
{
      //Polynomial is evaluated at each x and a line drawn between current point and the previous point
      Vector2f prevPt;
      Vector2f currPt;
      currPt = makeVector2f(xrange.first,0);
      prevPt=currPt;
      float range_step = (xrange.second-xrange.first)/(n_points-1);

      for(int i=0; i < n_points ; i++)
      {
         currPt[0]+=range_step;
	 DynamicVector<float> basis(poly_coeff.size());
	 basis[0]=1.0f;
	 currPt[1]=basis[0]*poly_coeff[0];
	 //Poor code follows
	 for(int j=1; j < basis.size(); j++)
	 { 
	    basis[j]= basis[j-1]*currPt[0];
	    currPt[1]+=basis[j]*poly_coeff[j];
	 }

	 if(i>0)
	 {
		viewer->addLine(prevPt,currPt,color,2); 
	 }
	 prevPt=currPt;
      }
      viewer->refresh();
}

void GMExperiment2_1::fitPolynomial()
{
      int deg_poly = interp_deg + 1;

      if(deg_poly > parabola.size())
      {
	   output<<"Error! Too many coefficients to be determined!\n";
      }
      else
      {
	  poly_coeff.setDim(deg_poly);
          // First compute the coefficients of the polynomial
          DynamicMatrix<float>M(deg_poly,parabola.size());  //(columns, rows) and not (rows, columns)
          DynamicVector<float>b(parabola.size());
          
          for(int i=0; i < deg_poly ; i++) //Each column
          {
             DynamicVector<float> &Mcol(M[i]);
             for(int r=0; r < parabola.size(); r++)
             {  
                 Mcol[r]= pow(parabola[r][0],i);
             }
          }

          for(int r=0; r < parabola.size(); r++)
          {  
              b[r]= parabola[r][1];
          }
     
          poly_coeff = invertMatrix(M.transpose()*M)*M.transpose()*b;

          //Then render the polynomial
          render_polynomial(poly_coeff, 1000, pair<float,float>(-4.0,4.0), makeVector4f(0.0,1.0,1.0,0.9));
      }

}

