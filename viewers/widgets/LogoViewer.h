//---------------------------------------------------------------------------
#ifndef GLGeometryViewerH
#define GLGeometryViewerH
//---------------------------------------------------------------------------
#include "ui_LogoViewerDesigner.h"
//---------------------------------------------------------------------------

class LogoViewer : public QWidget {
   Q_OBJECT

private:
   Ui_LogoViewer ui;

public:
   LogoViewer(QWidget * parent=NULL, Qt::WFlags f=0);
   ~LogoViewer();


};





#endif
