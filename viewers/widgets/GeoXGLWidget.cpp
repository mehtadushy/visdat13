//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "GeoXOutput.h"
#include "GeoXGLWidget.h"
#include <QtGui/qevent.h>
//---------------------------------------------------------------------------

void GeoXGLWidget::initOpenGLFormat()
{
    //openglformat.setVersion(2,1);
}

QGLFormat GeoXGLWidget::openglformat;

GeoXGLWidget::GeoXGLWidget(QWidget *parent,
                           const QGLWidget *shareWidget, Qt::WindowFlags f)
    : QGLWidget(openglformat, parent, shareWidget, f)
{
    //initializeGL();
    mode = CAM;
    pickedPointIndex = -1;
    lastPickedPointIndex = -1;
    zoom = 1.0f;
    transX = transY = 0;
    bgColor = makeVector3f(0.1f,0.1f,0.2f);
    drawAxisFlag = false;
    drawPointsFlag = true;
    drawLinesFlag = true;
    drawTrianglesFlag = true;
    drawWithGlowFlag = false;
    drawTextureFlag = true;

    this->clear();
}

void GeoXGLWidget::initializeGL()
{
    glViewport(0,0,width(),height());
	if (TexParams.GLTexName == 0)
    {
        glGenTextures(1, &TexParams.GLTexName);
        CheckForGLError("glGenTextures");
    }
}

void GeoXGLWidget::CheckForGLError(const char* DescriptiveCallingLocation)
{
    GLenum err_code = glGetError();
    while (GL_NO_ERROR != err_code)
    {
        output << "OpenGL Error @ " << DescriptiveCallingLocation << ": " << err_code << "\n";
        err_code = glGetError();
    }
}

void GeoXGLWidget::clear()
{
    points.clear();
    lines.clear();
    triangles.clear();
	TexParams.Clear();
    updateGL();
}

void GeoXGLWidget::clearLines()
{
    lines.clear();
    updateGL();
}

void GeoXGLWidget::clearTriangles()
{
    triangles.clear();
    updateGL();
}

void GeoXGLWidget::drawAxis()
{
    glLineWidth(1.0f);
    glColor4f(1.0f,0.0f,0.0f,1.0f);
    glBegin(GL_LINES);
    glVertex3f(-2000.0f, 0.0f, 0.0f);
    glVertex3f( 2000.0f, 0.0f, 0.0f);
    glEnd();
    glColor4f(0.0f,1.0f,0.0f,1.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, -2000.0f, 0.0f);
    glVertex3f(0.0f,  2000.0f, 0.0f);
    glEnd();
}

void GeoXGLWidget::paintGL()
{
    //Matrices and Background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10.0f*zoom,10.0f*zoom,(-10.0f*zoom)/((float)width()/height()),(10.0f*zoom)/((float)width()/height()));

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(transX, transY, 0);
    glClearColor(bgColor[0],bgColor[1],bgColor[2],0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    if (drawWithGlowFlag)
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    }
    else
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    //Draw Texture
    if (drawTextureFlag && TexParams.Dims[0] > 0 && TexParams.Dims[1] > 0)
    {
        if (TexParams.GLTexName != 0)
        {
            glColor4f(1.0f,1.0f,1.0f,1.0f);
            glEnable(GL_TEXTURE_2D);
            //glDisable(GL_LIGHTING);
            //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
            glBindTexture(GL_TEXTURE_2D, TexParams.GLTexName);
            CheckForGLError("glBindTexture");

            glTexImage2D(GL_TEXTURE_2D, 0,
                         GL_RGB, TexParams.Dims[0], TexParams.Dims[1], 0,
                         GL_RGB, GL_UNSIGNED_BYTE, &TexParams.ActualTexture[0]);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glBegin(GL_QUADS);
            glTexCoord2f(TexParams.TexMin[0], TexParams.TexMin[1]); glVertex2f(TexParams.BBoxMin[0], TexParams.BBoxMin[1]);
            glTexCoord2f(TexParams.TexMin[0], TexParams.TexMax[1]); glVertex2f(TexParams.BBoxMin[0], TexParams.BBoxMax[1]);
            glTexCoord2f(TexParams.TexMax[0], TexParams.TexMax[1]); glVertex2f(TexParams.BBoxMax[0], TexParams.BBoxMax[1]);
            glTexCoord2f(TexParams.TexMax[0], TexParams.TexMin[1]); glVertex2f(TexParams.BBoxMax[0], TexParams.BBoxMin[1]);
            glEnd();

            glDisable(GL_TEXTURE_2D);
        }
        else
        {
            output << "Internal error: Texture ID not valid\n";
        }
    }

    //Axis
    if (drawAxisFlag) drawAxis();

    //Draw Triangles
    if (drawTrianglesFlag)
    {
        for(unsigned int k = 0; k<triangles.size(); k++)
        {
            glColor4f(triangles[k].color[0],triangles[k].color[1],triangles[k].color[2],triangles[k].color[3]);
            glBegin(GL_TRIANGLES);
            glVertex2f(	points[triangles[k].vertices[0]].position[0],
                        points[triangles[k].vertices[0]].position[1]);
            glVertex2f(	points[triangles[k].vertices[1]].position[0],
                        points[triangles[k].vertices[1]].position[1]);
            glVertex2f( points[triangles[k].vertices[2]].position[0],
                        points[triangles[k].vertices[2]].position[1]);
            glEnd();
        }
    }

    //Draw Lines
    if (drawLinesFlag)
    {
        for(unsigned int j = 0; j<lines.size(); j++)
        {
            glLineWidth(lines[j].thickness);
            glColor4f(lines[j].color[0],lines[j].color[1],lines[j].color[2],lines[j].color[3]);
            glBegin(GL_LINES);
            glVertex2f(	points[lines[j].vertices[0]].position[0],
                        points[lines[j].vertices[0]].position[1]);
            glVertex2f(	points[lines[j].vertices[1]].position[0],
                        points[lines[j].vertices[1]].position[1]);
            glEnd();
        }
    }

    //Draw all Points
    if (drawPointsFlag)
    {
        for(unsigned int i = 0; i<points.size(); i++)
        {
            if (points[i].size > 0)
            {
                glPointSize(points[i].size);
                glColor4f(points[i].color[0],points[i].color[1],points[i].color[2],points[i].color[3]);
                glBegin(GL_POINTS);
                glVertex2f(	points[i].position[0],
                            points[i].position[1]);
                glEnd();
            }
        }

        //Draw the selected point
        if (lastPickedPointIndex > -1 && lastPickedPointIndex < (int)points.size())
        {
            glPushAttrib(GL_ENABLE_BIT);
            glLineStipple(1, 0x0707);
            glEnable(GL_LINE_STIPPLE);

            glColor4f(points[lastPickedPointIndex].color[0],points[lastPickedPointIndex].color[1],points[lastPickedPointIndex].color[2],points[lastPickedPointIndex].color[3]);

            glBegin(GL_LINE_STRIP);

            float px = points[lastPickedPointIndex].position[0];
            float py = points[lastPickedPointIndex].position[1];
            float ampl = 0.5f;

            for (unsigned i=0; i < 33; i++)
            {
                float alpha = 2.0f * M_PI * float(i) / float(32);
                glVertex2f(ampl * cos(alpha) + px, ampl * sin(alpha) + py);
            }

            glEnd();
            glPopAttrib();
        }
    }
}

int GeoXGLWidget::addPoint(Point2D p)
{
    points.push_back(p);
    return points.size()-1;
}

int GeoXGLWidget::addLine(Line l)
{
    lines.push_back(l);
    return lines.size()-1;
}

int GeoXGLWidget::addTriangle(Triangle t)
{
    triangles.push_back(t);
    return triangles.size()-1;
}

Point2D GeoXGLWidget::getPoint(int index)
{
    if ((index < 0) || (index > ((int)points.size()-1)))
    {
        output << "WARNING: No Point entry at index " << index << " ! A default value is returned!\n";
        return Point2D();
    }
    return points[index];
}

Line GeoXGLWidget::getLine(int index)
{
    if ((index < 0) || (index > ((int)lines.size()-1)))
    {
        output << "WARNING: No Line entry at index " << index << " ! A default value is returned!\n";
        return Line();
    }
    return lines[index];
}

Triangle GeoXGLWidget::getTriangle(int index)
{
    if ((index < 0) || (index > ((int)triangles.size()-1)))
    {
        output << "WARNING: No Triangle entry at index " << index << " ! A default value is returned!\n";
        return Triangle();
    }
    return triangles[index];
}

void GeoXGLWidget::setPoint(int index, Point2D p)
{
    if ((index < 0) || (index > ((int)points.size()-1)))
    {
        output << "WARNING: Index " << index << " out of bounds!\n";
        output << "WARNING: No changes applied!\n";
        return;
    }
    points[index] = p;
}

void GeoXGLWidget::setLine(int index, Line l)
{
    if ((index < 0) || (index > ((int)lines.size()-1)))
    {
        output << "WARNING: Index " << index << " out of bounds!\n";
        output << "WARNING: No changes applied!\n";
        return;
    }
    lines[index] = l;
}

void GeoXGLWidget::setTriangle(int index, Triangle t)
{
    if ((index < 0) || (index > ((int)triangles.size()-1)))
    {
        output << "WARNING: Index " << index << " out of bounds!\n";
        output << "WARNING: No changes applied!\n";
        return;
    }
    triangles[index] = t;
}

namespace
{
///Scale from [0, 1] to [0, 255] and clamp
inline unsigned char ScaleAndClamp(float fValue)
{
    fValue *= 255;
    if (fValue > 255) return 255;
    if (fValue < 0) return 0;

    return (unsigned char)fValue;
}
}

#define IS_POW_2(x) (((x) != 0) && (((x) & ((x) - 1)) == 0))
bool GeoXGLWidget::setTextureGray(const GridData& argTexture)
{
    if (argTexture.numDataComponents != 1)
    {
        output << "setTextureGray failed, because the given data is not a scalar field.\n";
        return false;
    }

    if (!IS_POW_2(argTexture.dims[0]) || !IS_POW_2(argTexture.dims[1]))
    {
        output << "setTextureGray failed, because the given dimensions are not a power of two.\n";
        output << "Use a scalar field size of 256x256, 512x512, 256x512, 128x512, or similar.\n";
        return false;
    }

	TexParams.Clear();
	TexParams.Dims[0] = argTexture.dims[0];
	TexParams.Dims[1] = argTexture.dims[1];
	TexParams.BBoxMin[0] = argTexture.boundMin[0];
	TexParams.BBoxMin[1] = argTexture.boundMin[1];
	TexParams.BBoxMax[0] = argTexture.boundMax[0];
	TexParams.BBoxMax[1] = argTexture.boundMax[1];
	TexParams.TexMin = makeVector2f(0.5f / TexParams.Dims[0], 0.5f / TexParams.Dims[1]);
	TexParams.TexMax = makeVector2f(1, 1) - TexParams.TexMin;

    TexParams.ActualTexture.resize(argTexture.dims[0] * argTexture.dims[1] * 3);

    for(unsigned int j=0; j<argTexture.dims[1]; j++)
    {
        for(unsigned int i=0; i<argTexture.dims[0]; i++)
        {
            const unsigned int idx = i + j * argTexture.dims[0];
            const unsigned int idxRGB = 3 * idx;

            //Cast to byte and set the texture pixel.
            TexParams.ActualTexture[idxRGB] =
            TexParams.ActualTexture[idxRGB+1] =
            TexParams.ActualTexture[idxRGB+2] = ScaleAndClamp(argTexture.data[idx]);
        }
    }

    return true;
}


bool GeoXGLWidget::setTextureRGB(const GridData& Red, const GridData& Green, const GridData& Blue)
{
    if (Red.numDataComponents != 1 || Green.numDataComponents != 1 || Blue.numDataComponents != 1)
    {
        output << "setTextureRGB failed, because the given data is not a scalar field.\n";
        return false;
    }

    if (!IS_POW_2(Red.dims[0]) || !IS_POW_2(Red.dims[1]))
    {
        output << "setTextureRGB failed, because the given dimensions are not a power of two.\n";
        output << "Use a scalar field size of 512x512, or similar.\n";
        return false;
    }

    if (Red.dims != Green.dims || Red.dims != Blue.dims)
    {
        output << "setTextureRGB failed, because the three scalar fields have different dimensions.\n";
        return false;
    }

	//We only need the meta data of one channel
	TexParams.Clear();
	TexParams.Dims[0] = Red.dims[0];
	TexParams.Dims[1] = Red.dims[1];
	TexParams.BBoxMin[0] = Red.boundMin[0];
	TexParams.BBoxMin[1] = Red.boundMin[1];
	TexParams.BBoxMax[0] = Red.boundMax[0];
	TexParams.BBoxMax[1] = Red.boundMax[1];
	TexParams.TexMin = makeVector2f(0.5f / TexParams.Dims[0], 0.5f / TexParams.Dims[1]);
	TexParams.TexMax = makeVector2f(1, 1) - TexParams.TexMin;

    TexParams.ActualTexture.resize(Red.dims[0] * Red.dims[1] * 3);

    for(unsigned int j=0; j<Red.dims[1]; j++)
    {
        for(unsigned int i=0; i<Red.dims[0]; i++)
        {
            const unsigned int idx = i + j * Red.dims[0];
            const unsigned int idxRGB = 3 * idx;

            //Cast to byte and set the texture pixel.
            TexParams.ActualTexture[idxRGB] = ScaleAndClamp(Red.data[idx]);
            TexParams.ActualTexture[idxRGB+1] = ScaleAndClamp(Green.data[idx]);
            TexParams.ActualTexture[idxRGB+2] = ScaleAndClamp(Blue.data[idx]);
        }
    }

    return true;
}
#undef IS_POW_2


void GeoXGLWidget::swapDrawAxis()
{
    drawAxisFlag = !drawAxisFlag;
    updateGL();
}

void GeoXGLWidget::swapDrawPoints()
{
    drawPointsFlag = !drawPointsFlag;
    updateGL();
}

void GeoXGLWidget::swapDrawLines()
{
    drawLinesFlag = !drawLinesFlag;
    updateGL();
}

void GeoXGLWidget::swapDrawTriangles()
{
    drawTrianglesFlag = !drawTrianglesFlag;
    updateGL();
}

void GeoXGLWidget::swapDrawWithGlow()
{
    drawWithGlowFlag = !drawWithGlowFlag;
    updateGL();
}

void GeoXGLWidget::swapDrawTexture()
{
    drawTextureFlag = !drawTextureFlag;
    updateGL();
}

GeoXGLWidget::~GeoXGLWidget()
{
}

void GeoXGLWidget::mousePressEvent(QMouseEvent *event)
{
    if (mode == PICK)
    {
        findClosestPoint(event->x(),event->y());
        if (pickedPointIndex != -1)
            emit pointSelected(pickedPointIndex);
    }
    if (mode == CAM)
    {
        startX = event->x();
        startY = event->y();
    }
    if(mode == CREATE)
    {
        startX = event->x();
        startY = event->y();
        Point2D pNew;
        int newIndex = this->addPoint(pNew);
        double modelview[16], projection[16];
        int viewport[4];
        glGetDoublev( GL_PROJECTION_MATRIX, projection );
        glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
        glGetIntegerv( GL_VIEWPORT, viewport );
        Vector3d objPos;
        gluUnProject(event->x(), viewport[3]-event->y(), 0.0, modelview, projection, viewport, &objPos[0], &objPos[1], &objPos[2] );
        points[newIndex].position = makeVector2f(objPos[0], objPos[1]);
    }
    updateGL();
}

void GeoXGLWidget::mouseReleaseEvent (QMouseEvent * event)
{
    if((mode == PICK) && (pickedPointIndex != -1))
    {
        double modelview[16], projection[16];
        int viewport[4];
        glGetDoublev( GL_PROJECTION_MATRIX, projection );
        glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
        glGetIntegerv( GL_VIEWPORT, viewport );
        Vector3d objPos;
        gluUnProject(event->x(), viewport[3]-event->y(), 0.0, modelview, projection, viewport, &objPos[0], &objPos[1], &objPos[2] );
        points[pickedPointIndex].position = makeVector2f(objPos[0], objPos[1]);
        pickedPointIndex = -1;
        emit widgetContentChanged();
    }
    if (mode == CAM)
    {
        // nothing to do here
    }
    updateGL();
}

void GeoXGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(mode == CAM)
    {
        if(event->buttons() & Qt::RightButton)
        {
            int diffx = event->x() - startX;
            int diffy = event->y() - startY;
            if (abs(diffx) > abs(diffy))
                zoom *= 1 + diffx * 0.01;
            else
                zoom *= 1 + diffy * 0.01;
        }
        if(event->buttons() & Qt::LeftButton)
        {
            int diffx = event->x() - startX;
            int diffy = event->y() - startY;
            transX += diffx * 0.03 * zoom;
            transY -= diffy * 0.03 * zoom;
        }
    }
    if((mode == PICK) && (pickedPointIndex != -1))
    {
        double modelview[16], projection[16];
        int viewport[4];
        glGetDoublev( GL_PROJECTION_MATRIX, projection );
        glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
        glGetIntegerv( GL_VIEWPORT, viewport );
        Vector3d objPos;
        gluUnProject(event->x(), viewport[3]-event->y(), 0.0, modelview, projection, viewport, &objPos[0], &objPos[1], &objPos[2] );
        points[pickedPointIndex].position = makeVector2f(objPos[0], objPos[1]);
    }
    if(mode == CREATE)
    {
        Point2D pNew;
        int newIndex = this->addPoint(pNew);
        double modelview[16], projection[16];
        int viewport[4];
        glGetDoublev( GL_PROJECTION_MATRIX, projection );
        glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
        glGetIntegerv( GL_VIEWPORT, viewport );
        Vector3d objPos;
        gluUnProject(event->x(), viewport[3]-event->y(), 0.0, modelview, projection, viewport, &objPos[0], &objPos[1], &objPos[2] );
        points[newIndex].position = makeVector2f(objPos[0], objPos[1]);
    }

    startX = event->x();
    startY = event->y();
    updateGL();
}

void GeoXGLWidget::resizeGL(int width, int height)
{
    this->resize(width,height);
    glViewport(0,0,this->width(),this->height());
}

void GeoXGLWidget::findClosestPoint(int x, int y)
{
    int ind = -1;
    double modelview[16], projection[16];
    int viewport[4];
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetIntegerv( GL_VIEWPORT, viewport );
    double winX, winY, winZ;
    for(int i = 0; i < (int)points.size(); i++)
    {
        gluProject(points[i].position[0],points[i].position[1],0.0,
                   modelview,projection,viewport,&winX,&winY,&winZ);
        if((makeVector2f(winX,viewport[3]-winY)-makeVector2f(x,y)).getSqrNorm() < 16.0f)
        {
            if(points[i].canBeModified)
            {
                pickedPointIndex = i;
                lastPickedPointIndex = i;
                break;
            }
        }
    }
}

#ifndef CMAKE_INTDIR
#ifdef _WIN32
#include "moc/GeoXGLWidget_moc.cpp"
#else
#ifndef CMAKE_UNIX_INCLUDE_MOC
#include "moc_GeoXGLWidget.cpp"
#endif
#endif
#endif

