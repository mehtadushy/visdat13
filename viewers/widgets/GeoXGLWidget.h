//---------------------------------------------------------------------------
#ifndef GeoXGLWidgetH
#define GeoXGLWidgetH
//---------------------------------------------------------------------------
#include <QtOpenGL/QGLWidget>
#include <GL/glu.h>
#include "VisTypes.h"
#include "WalkthroughCameraController.h"
#include "Camera.h"
#include "ViewFrustum.h"
#include "GridData.hpp"
//---------------------------------------------------------------------------
#define PICK 0
#define CAM 1
#define CREATE 2

class GeoXGLWidget : public QGLWidget
{
    Q_OBJECT
private:
    // datastructures containing the primitives
    std::vector<Point2D> points;
    std::vector<Line> lines;
    std::vector<Triangle> triangles;

	///Structure to hold texture parameters
	struct TTextureParameters
	{
		TTextureParameters()
			:GLTexName(0)
		{
			Clear();
		}

		void Clear()
		{
			Dims.setZero();
			BBoxMin.setZero();
			BBoxMax.setZero();
			ActualTexture.clear();
		}

		///OpenGL Name
		GLuint GLTexName;

		///Dimensions, number of pixels in each direction.
		///Seems as if this needs to be a power-of-two. Weird, though, should support any sizes.
		Vector2ui Dims;

		///Location in space where the texture is anchored.
		Vector2f BBoxMin;
		Vector2f BBoxMax;

		///Texture Min/Max-Location. Half a pixel over the boundary.
		Vector2f TexMin;
		Vector2f TexMax;

		///Data: RGB
	    std::vector< unsigned char > ActualTexture;
	};

	TTextureParameters TexParams;

    void findClosestPoint(int x, int y);

    // CAMERA, PICKING or CREATE
    int mode;
    // currently selected point
    int pickedPointIndex;
    int lastPickedPointIndex;

    // camera steering
    float zoom;
    float transX, transY;
    int startX, startY;
    Vector3f bgColor;
    bool drawAxisFlag;
    bool drawPointsFlag;
    bool drawLinesFlag;
    bool drawTrianglesFlag;
    bool drawWithGlowFlag;
    bool drawTextureFlag;

    static QGLFormat openglformat;
public:
    static void initOpenGLFormat();

protected:
    // derived functions
    virtual void paintGL();
    virtual void initializeGL();
    void CheckForGLError(const char* DescriptiveCallingLocation);
    void drawAxis();
    virtual void resizeGL(int width, int height);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent * event);
    virtual void mouseMoveEvent(QMouseEvent *event);

public:
    GeoXGLWidget(QWidget *parent=0,
                 const QGLWidget *shareWidget=0,
                 Qt::WindowFlags f=0);
    virtual ~GeoXGLWidget();

    // add primitives
    int addPoint    (Point2D p);
    int addLine     (Line l);
    int addTriangle (Triangle t);

    // return primitives
    Point2D  getPoint    (int index);
    Line     getLine     (int index);
    Triangle getTriangle	(int index);

    // change primitives
    void setPoint    (int index, Point2D p);
    void setLine     (int index, Line l);
    void setTriangle (int index, Triangle t);

    /** Sets the texture to show at the coordinates that are given with the GridData.
        Allows only scalar fields.
        Value range is clamped to [0, 1].
    */
    bool setTextureGray(const GridData& argTexture);

    /** Sets the color texture to show at the coordinates that are given with the GridData.
        The color channels are given as individual scalar fields.
        Value range is clamped to [0, 1].
    */
    bool setTextureRGB(const GridData& Red, const GridData& Green, const GridData& Blue);

    int getNumberOfPoints()    {return points.size();}
    int getNumberOfLines()     {return lines.size();}
    int getNumberOfTriangles() {return triangles.size();}

    // clear all primitives
    void clear();
    void clearLines(); // clear lines, leave points and triangles in place
    void clearTriangles(); // clear triangles, leave points and lines in place

    void setCameraMode() {mode = CAM;}
    void setPickMode()  {mode = PICK;}
    void setCreateMode() {mode = CREATE;}

    void setBackgroundColor(Vector3f color) {bgColor = color;}
    void setBackgroundColor(float red, float green, float blue) {bgColor = makeVector3f(red,green,blue);}

    // get point which was selcted as last
    int getLastSelectedPointIndex() const { return lastPickedPointIndex; }

    // toggle axis drawing
    void swapDrawAxis();
    void swapDrawPoints();
    void swapDrawLines();
    void swapDrawTriangles();
    void swapDrawWithGlow();
    void swapDrawTexture();

signals:
    void widgetContentChanged();
    void pointSelected(int);
};

#endif
