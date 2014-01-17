#ifndef GLGeometryViewer3DH
#define GLGeometryViewer3DH
//---------------------------------------------------------------------------
#include "ui_GLGeometryViewer3DDesigner.h"
#include "VisTypes.h"
//---------------------------------------------------------------------------

class GLGeometryViewer3D : public QWidget {
   Q_OBJECT
 private:
   Ui_GLGeometryViewer3D ui;

 private slots:
	void on_btnCamera_pressed();
	void on_btnPick_pressed();
	void on_btnClear_pressed();
	void on_btnLight_pressed();
	void on_btnFillMode_pressed();
	void on_btnDrawAxis_pressed();
	void on_btnHandles_pressed();
	void on_btnResetCamera_pressed();
	void on_btnVertexLighting_pressed();
	void sltWidgetContentChanged();

signals:
	void viewerContentChanged();

 public:
   GLGeometryViewer3D(QWidget * parent=NULL, Qt::WFlags f=0);
   ~GLGeometryViewer3D();

	// primitive I/O
	int       addPoint(Point3D p);
	Point3D   getPoint(int index);
	void      setPoint(int index, Point3D p);

	int       addLine(Line l);
	int       addLine(Vector3f pos1, Vector3f pos2);
	Line      getLine(int index);
	void      setLine(int index, Line l);

	int       addTriangle(Triangle t);
	int       addTriangle(Vector3f pos1, Vector3f pos2, Vector3f pos3);
	Triangle  getTriangle(int index);
	void      setTriangle(int index, Triangle t);

	int       getNumberOfPoints()    {return ui.glFrame->getNumberOfPoints();}
	int       getNumberOfLines()     {return ui.glFrame->getNumberOfLines();};
	int       getNumberOfTriangles() {return ui.glFrame->getNumberOfTriangles();};

	// user interface appearance
	void      setBackgroundColor(Vector3f color){ui.glFrame->setBackgroundColor(color);};
	void      setBackgroundColor(float red, float green, float blue){ui.glFrame->setBackgroundColor(red, green,blue);};
	void      refresh() {ui.glFrame->updateGL();}

	// remove primitives
	void      clear(){ui.glFrame->clear();};                   // all primitives
	void      clearLines(){ui.glFrame->clearLines();};         // only lines
	void      clearTriangles(){ui.glFrame->clearTriangles();}; // only triangles
	
	// set perspective or ortho projection
	void      enableOrthoProjection(bool en) { if (en) ui.glFrame->setCameraOrthoMode(); else ui.glFrame->setCameraMode(); }
};





#endif
