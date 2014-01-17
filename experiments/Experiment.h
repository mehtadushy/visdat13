//---------------------------------------------------------------------------
#ifndef ExperimentH
#define ExperimentH
//---------------------------------------------------------------------------
#include "Persistent.h"
//---------------------------------------------------------------------------

class QWidget;
///
/// Base class for experiments.
///
/// Implement and register a subclass to define a new experiment.
/// You can use GeoX properties and methods to define parameters and actions (buttons) for your experiment.
/// See "ExampleExperiment" for details.
///
class Experiment : public Persistent
{
   GEOX_CLASS(Experiment)

public:
   // -- the following methods are optional
   // -- they can be used to customize the behavior of the experiment class

   /// override this method to specify the viewer you want to use in your experiment
   /// standard viewer is "LogoViewer" (text only)
   virtual QWidget *createViewer();
   /// this method is called when the experiment is started
   /// override if necessary
   virtual void activate() {};
   /// this method is called when the experiment is suspended (other user choice)
   /// override if necessary
   virtual void deactivate() {};
   /// this method is called after GLRendering
   /// it can be used to display additional information if necessary
   virtual void customPaintGL() {};

   virtual void update() {};
};





#endif
