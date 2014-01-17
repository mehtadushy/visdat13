#ifndef GLGeometryViewerH
#define GLGeometryViewerH
//---------------------------------------------------------------------------
#include "ui_GLGeometryViewerDesigner.h"
#include "VisTypes.h"
//---------------------------------------------------------------------------

class GLGeometryViewer : public QWidget {
   Q_OBJECT

 private slots:
	void on_btnCamera_pressed();
	void on_btnPick_pressed();
	void on_btnClear_pressed();
	void on_btnCreate_pressed();
	void on_btnDrawAxis_pressed();
	void on_btnDrawPoints_pressed();
	void on_btnDrawLines_pressed();
	void on_btnDrawTriangles_pressed();
	void on_btnDrawWithGlow_pressed();
	void on_btnDrawTexture_pressed();
	void sltWidgetContentChanged();
	void sltPointSelected(int index);

 signals:
	void viewerContentChanged();
	void pointSelected(int);
	
 public:
    typedef void (*PointSelectedCallback)(int);
    GLGeometryViewer(QWidget * parent=NULL, Qt::WFlags f=0);
    ~GLGeometryViewer();

	int     addPoint(Point2D p);
	Point2D getPoint(int index);
	void    setPoint(int index, Point2D p);
	int		getLastSelectedPointIndex() const { return ui.glFrame->getLastSelectedPointIndex(); }

	int  addLine(Line l);
	int  addLine(Vector2f pos1, Vector2f pos2,
                 const Vector4f& color = makeVector4f(0.0f,1.0f,1.0f,1.0f),
                 const float32 thickness = 1.0f, const float32 point_thickness = 0.0f);
	int  addLine(float pos1X, float pos1Y, float pos2X, float pos2Y,
                 const Vector4f& color = makeVector4f(0.0f,1.0f,1.0f,1.0f),
                 const float32 thickness = 1.0f, const float32 point_thickness = 0.0f);
	Line getLine(int index);
	void setLine(int index, Line l);

	int      addTriangle(Triangle t);
	int      addTriangle(Vector2f pos1, Vector2f pos2, Vector2f pos3);
	int      addTriangle(float pos1X, float pos1Y, float pos2X, float pos2Y, float pos3X, float pos3Y);
	Triangle getTriangle	(int index);
	void     setTriangle (int index, Triangle t);

	/** Sets the texture to show at the coordinates that are given with the GridData.
		Allows only scalar fields.
		Value range is clamped to [0, 1].
	*/
	bool setTextureGray(const GridData& argTexture)
	{
		return ui.glFrame->setTextureGray(argTexture);
	}
	
	/** Sets the color texture to show at the coordinates that are given with the GridData.
		The color channels are given as individual scalar fields.
		Value range is clamped to [0, 1].
	*/
	bool setTextureRGB(const GridData& Red, const GridData& Green, const GridData& Blue)
	{
		return ui.glFrame->setTextureRGB(Red, Green, Blue);
	}

	// returns the number of points indices are 0..getPointCount()-1
	int getNumberOfPoints()    {return ui.glFrame->getNumberOfPoints();}
	int getNumberOfLines()     {return ui.glFrame->getNumberOfLines();};
	int getNumberOfTriangles() {return ui.glFrame->getNumberOfTriangles();};

	void refresh() { ui.glFrame->updateGL(); }
	void setBackgroundColor(Vector3f color){ui.glFrame->setBackgroundColor(color);};
	void setBackgroundColor(float red, float green, float blue){ui.glFrame->setBackgroundColor(red, green,blue);};
	void setPointSelectedCallback(PointSelectedCallback cb) { cbPointSelected = cb; }

	void clear();
	void clearLines();
	void clearTriangles();

private:
	Ui_GLGeometryViewer ui;
	PointSelectedCallback cbPointSelected;
};





#endif
