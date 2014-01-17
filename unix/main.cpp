//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "InitGeoX.h"
//---------------------------------------------------------------------------
#include "GeoXMainWindow.h"
#include "ExceptionHandling.h"
#include "LogoViewer.h"
#include "GeoXOutput.h"
//---------------------------------------------------------------------------
// include icons & other resources
//#include "../images/moc_geoX_rcc.h"
int qInitResources_geoX();
int qCleanupResources_geoX();
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------

int main(int argc, char **argv)
{
   int n = 0;
   QApplication qapp(n, 0);

   init();
   qInitResources_geoX();

   //QApplication::setStyle(new QCleanlooksStyle);
   //QApplication::setStyle(new QPlastiqueStyle);

   // create main window
   mainWindow = new GeoXMainWindow;
   mainWindow->show();
   mainWindow->setDisplayWidget(new LogoViewer());
   qapp.setActiveWindow( mainWindow );

   //Close all windows with main window
   QApplication::connect(mainWindow, SIGNAL(destroyed()), &qapp, SLOT(closeAllWindows()));

   output << "GeoX started.\n";

   // run it
   qapp.exec();

   shutdown();
   qCleanupResources_geoX();

   return 0;
}

