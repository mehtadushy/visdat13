//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "Experiment1_2.h"
//---------------------------------------------------------------------------
#include "Properties.h"
#include <sstream>
#include <fstream>
#include "GLGeometryViewer.h"
#include "GeoXOutput.h"
//---------------------------------------------------------------------------

IMPLEMENT_GEOX_CLASS( Experiment1_2, 0)                        //Name the class. Set version number to 0
{
    BEGIN_CLASS_INIT( Experiment1_2 );                         //Begin the declaration of properties
							       //We first add fields for manipulating data for the Experiment
   ADD_SEPARATOR("Filename")
   ADD_STRING_PROP(csv_filename,0)
   
   ADD_SEPARATOR("Visual Scaling")
   ADD_FLOAT32_PROP(XScale,0)
   ADD_FLOAT32_PROP(YScale,0)


   ADD_SEPARATOR("Rendering")
   ADD_FLOAT32_PROP(Alpha,0)

   ADD_NOARGS_METHOD(Experiment1_2::DrawParallelPlot)
}

QWidget* Experiment1_2::createViewer()  //What is this for?
{
    viewer = new GLGeometryViewer();
    return viewer;
}

Experiment1_2::Experiment1_2()           //Constructor
{
    viewer = NULL;                       // Why?
    XScale=1.0f; 
    YScale=1.0f;
    Alpha = 0.7f; 
    
}

Experiment1_2::~Experiment1_2() {}

 

void Experiment1_2::DrawParallelPlot()
{
    //Read the CSV file and figure out the number of rows. We do not need to store the data anywhere and can be plotted on the fly on the second pass, after we have figured out the max and min for each column

   std::string line;
   std::ifstream input_file;
   input_file.open( csv_filename.c_str() );
   int32 row=0, column=0;
   if ( input_file.is_open() )
   {
      while( getline(input_file,line) )
      {
          if(row == 0)
          {
            //Count Columns and resize Data_Attr
            //Traverse the string and count 
            for (int c=0; c< line.length(); c++ )
            {
                 if(line[c] == ',')
                 { column ++;
                 }
            }
            column=column+1;  
          
            Data_Attr.clear();
            Data_Attr.resize(column);
            Data_Size[1]=column;
          }
          else            //Get Max and Min
          {
             line= line + std::string(",");
             std::stringstream iss(line);

             for(int32 i=0; i< column; i++)
             {
               std::string val;
               std::getline(iss, val, ',');
               if ( !iss.good() )
                break;
               std::stringstream convertor(val);
            
               convertor >> Data_Attr[i][0];
               if(row == 1)
                { Data_Attr[i][1]=Data_Attr[i][0];
                  Data_Attr[i][2]=Data_Attr[i][0];
                }
               else  //Determine max and min
               {
                 if(Data_Attr[i][0]>Data_Attr[i][1])    //Max
                 {
                    Data_Attr[i][1]= Data_Attr[i][0];
                 }
                 if(Data_Attr[i][0]<Data_Attr[i][2])    //Min
                 {
                    Data_Attr[i][2]= Data_Attr[i][0];
                 }

               }
             }
           }
       
          row++;
       }
    }
    else   //If file couldn't be opened.
    {
       output<<"Couldn't Open File\n";
    }
    viewer->clear();

    input_file.clear();  //Clearing bad state after EOF
    input_file.seekg(0); //Reset the file pointer
    row=0;

    float XOff=2.0;                        //Separation between the axis of the parallel plot
    while( getline(input_file,line) )
      {
         line= line + std::string(",");
         if(row>0) 
          {
             std::stringstream iss(line);

             for(int32 i=0; i< column; i++)
             {
               std::string val;
               
               std::getline(iss, val, ',');
               if ( !iss.good() )
                break;
               std::stringstream convertor(val);
            
               convertor >> Data_Attr[i][0];
               
  
             }
             //Plot the data here
             for(int i=0; i< column-1; i++)
             {

               viewer->addLine(makeVector2f(XOff * ((float) i) * XScale, YScale*((Data_Attr[i][0]-Data_Attr[i][2])/ (Data_Attr[i][1]-Data_Attr[i][2]))), 
                               makeVector2f(XOff * ((float) (i+1)) * XScale, YScale*((Data_Attr[i+1][0]-Data_Attr[i+1][2])/ (Data_Attr[i+1][1]-Data_Attr[i+1][2]))),
                               makeVector4f(0, 1, 0.5 , Alpha)  );
              //output << YScale*((Data_Attr[i][0]-Data_Attr[i][2])/ (Data_Attr[i][1]-Data_Attr[i][2]))<<" "<< YScale*((Data_Attr[i+1][0]-Data_Attr[i+1][2])/ (Data_Attr[i+1][1]-Data_Attr[i+1][2]))<<"\n" ;

             }
           } 
       
          row++;
       }

    
    
     //Axes
     for (int i=0; i< Data_Size[1]; i++)
     {

        viewer->addLine(makeVector2f( XOff * ((float) i) * XScale, 0), makeVector2f(XOff * ((float) i) * XScale, YScale), makeVector4f(1,0,1,1), 3);
     }
    // display changes
    viewer->refresh();

    

}




