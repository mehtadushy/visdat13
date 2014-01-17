//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "Experiment.h"
//---------------------------------------------------------------------------
#include "Properties.h"
#include "LogoViewer.h"
//---------------------------------------------------------------------------



IMPLEMENT_GEOX_CLASS( Experiment ,0)
{
   BEGIN_CLASS_INIT( Experiment );
}

QWidget *Experiment::createViewer()
{
   return new LogoViewer();
}
