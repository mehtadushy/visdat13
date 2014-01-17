//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "GeoXGLWidget3D.h"
#include <QtGui/qevent.h>
#include "GeoXOutput.h"
//---------------------------------------------------------------------------

// lightsource parameters
GLfloat light_ambient[]            = {0.2f, 0.2f,  0.2f, 1.0f};
GLfloat light_diffuse[]            = {1.0f, 1.0f,  1.0f, 1.0f};
GLfloat light_specular[]           = {1.0f, 1.0f,  1.0f, 1.0f};
GLfloat light_position[]           = {0.0f, 0.0f,  10.0f, 0.0f};
GLfloat light_spotDirection[]      = {0.0f, 0.0f, -1.0f};
GLfloat light_spotExponent         =   0.0f;
GLfloat light_spotCutOff           = 180.0f;
GLfloat light_attenuationConst     =   1.0f;
GLfloat light_attenuationLinear    =   0.0f;
GLfloat light_attenuationQuadratic =   0.0f;

// material parameters
GLfloat material_ambient[]         = {0.23f, 0.23f, 0.23f, 1.0f};
GLfloat material_specular[]        = {0.77f, 0.77f, 0.77f, 1.0f};
GLfloat material_emission[]        = {0.0f,  0.0f,  0.0f,  1.0f};
GLfloat material_shininess         =  89.6f;


GeoXGLWidget3D::GeoXGLWidget3D(QWidget *parent, const QGLWidget *shareWidget, Qt::WindowFlags f)
: QGLWidget(parent, shareWidget, f)
{
  initializeGL();
  camControl = new ExaminerCameraController();
  camControl->bindCamera(&camera);
  camControl->setNotifyOnCameraChange(QDelegate(this, &GeoXGLWidget3D::updateGL));
  mode = CAM;
  pickedPointIndex = -1;
  pickedZ = -1;
  fillMode = GL_FILL;
  bgColor = makeVector3f(0.2f,0.2f,0.4f);
  drawAxisFlag = true;
  drawHandlePoints = false;
  recomputeNormal = false;
  faceLighting = true;
}

void GeoXGLWidget3D::initializeGL()
{
  camera.setPosition(makeVector3f(0.0f,0.0f,10.0f));
  camera.setLookAt(makeVector3f(0.0f,0.0f,0.0f));
  camera.setUp(makeVector3f(0.0f,1.0f,0.0f));
  glViewport(0,0,width(),height());
  fillMode = GL_FILL;
  glPolygonMode(GL_FRONT_AND_BACK,fillMode);

  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  lightOnOff = true;
  glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );

  glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
  glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);
  glLightfv(GL_LIGHT0,GL_POSITION,light_position);
  glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,light_spotDirection);
  glLightf (GL_LIGHT0,GL_SPOT_CUTOFF,light_spotCutOff);
  glLightf (GL_LIGHT0,GL_SPOT_EXPONENT,light_spotExponent);
  glLightf (GL_LIGHT0,GL_CONSTANT_ATTENUATION,light_attenuationConst);
  glLightf (GL_LIGHT0,GL_LINEAR_ATTENUATION,light_attenuationLinear);
  glLightf (GL_LIGHT0,GL_QUADRATIC_ATTENUATION,light_attenuationQuadratic);

  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,material_ambient);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,material_specular);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,material_emission);
  glMaterialf (GL_FRONT_AND_BACK,GL_SHININESS,material_shininess);
}


void GeoXGLWidget3D::resetCamera()
{
  camControl->setup(makeVector3f(0.0f,0.0f,10.0f),makeVector3f(0.0f,0.0f,0.0f),makeVector3f(0.0f,1.0f,0.0f));
}


void GeoXGLWidget3D::clear()
{
  points.clear();
  lines.clear();
  triangles.clear();
  updateGL();
}

void GeoXGLWidget3D::clearLines()
{
  lines.clear();
  updateGL();
}

void GeoXGLWidget3D::clearTriangles()
{
  triangles.clear();
  updateGL();
}

void GeoXGLWidget3D::swapLighting()
{
  if(lightOnOff)
    glDisable(GL_LIGHTING);
  else
    glEnable(GL_LIGHTING);
  lightOnOff = !lightOnOff;
  updateGL();
}

void GeoXGLWidget3D::swapLightingMode()
{
  faceLighting = !faceLighting;
  updateGL();
}

void GeoXGLWidget3D::swapFillMode()
{
  switch(fillMode)
  {
    case GL_FILL:
      fillMode = GL_LINE;
      break;
    case GL_LINE:
      fillMode = GL_FILL;
      break;
  }
  glPolygonMode(GL_FRONT_AND_BACK,fillMode);
  updateGL();
}

void GeoXGLWidget3D::swapDrawAxis()
{
  drawAxisFlag = !drawAxisFlag;
  updateGL();
}

void GeoXGLWidget3D::swapHandleDrawing()
{
  drawHandlePoints = !drawHandlePoints;
  updateGL();
}

void GeoXGLWidget3D::drawAxis()
{
  if(lightOnOff)
    glDisable(GL_LIGHTING);
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
  glColor4f(0.0f,0.0f,1.0f,1.0f);
  glBegin(GL_LINES);
  glVertex3f(0.0f, 0.0f, -2000.0f);
  glVertex3f(0.0f, 0.0f,  2000.0f);
  glEnd();
  if(lightOnOff)
    glEnable(GL_LIGHTING);

}

void GeoXGLWidget3D::paintGL()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float32 aspect;
  if (height() > 0) {
    aspect = ((float)width())/((float)height());
  } else {
    aspect = 1;
  }
  vf.setXToYAspectRatio(aspect);
  if (mode == CAM_ORTHO)
    glOrtho(-10.0, 10.0, -10.0, 10.0, vf.getZNearClip(), vf.getZFarClip());
  else
    gluPerspective(vf.getVerticalFieldOfView(), aspect, vf.getZNearClip(), vf.getZFarClip());
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  Vector3f pos = camera.getPosition();
  Vector3f lookAt = camera.getLookAt();
  Vector3f up = camera.getUp();
  gluLookAt(pos[0],pos[1],pos[2],lookAt[0],lookAt[1],lookAt[2],up[0],up[1],up[2]);
  glClearColor(bgColor[0],bgColor[1],bgColor[2],0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if(drawAxisFlag)
    drawAxis();

  if(drawHandlePoints)
  {
    if(lightOnOff)
      glDisable(GL_LIGHTING);
    for(unsigned int i = 0;i<points.size();i++)
    {
      if(!points[i].canBeModified)
        continue;

      glPointSize(points[i].size);
      glColor4f(points[i].color[0],points[i].color[1],points[i].color[2],points[i].color[3]);
      glBegin(GL_POINTS);
        glVertex3f(	points[i].position[0],
                points[i].position[1],
                points[i].position[2]);
      glEnd();
    }
    if(lightOnOff)
      glEnable(GL_LIGHTING);
  }
  for(unsigned int j = 0;j<lines.size();j++)
  {
     glLineWidth(lines[j].thickness);
    glColor4f(lines[j].color[0],lines[j].color[1],lines[j].color[2],lines[j].color[3]);
    glBegin(GL_LINES);
      glVertex3f(	points[lines[j].vertices[0]].position[0],
              points[lines[j].vertices[0]].position[1],
              points[lines[j].vertices[0]].position[2]);
      glVertex3f(	points[lines[j].vertices[1]].position[0],
              points[lines[j].vertices[1]].position[1],
              points[lines[j].vertices[1]].position[2]);
    glEnd();
  }

  for(unsigned int k = 0;k<triangles.size();k++)
  {
    glColor4f(triangles[k].color[0],triangles[k].color[1],triangles[k].color[2],triangles[k].color[3]);
    glBegin(GL_TRIANGLES);
    if(faceLighting)
    {
      GLfloat material_diffuse[] = {triangles[k].color[0], triangles[k].color[1],
                          triangles[k].color[2], triangles[k].color[3]};
      glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,material_diffuse);

      Vector3f zero = makeVector3f(0.0,0.0,0.0);
      if((triangles[k].normal == zero) || recomputeNormal)
      {
        Vector3f d1 = points[triangles[k].vertices[0]].position - points[triangles[k].vertices[1]].position;
        Vector3f d2 = points[triangles[k].vertices[0]].position - points[triangles[k].vertices[2]].position;
        Vector3f n = d1.crossProduct(d2);
        n = normalize(n);
        triangles[k].normal = n;
      }
      glNormal3f(triangles[k].normal[0],triangles[k].normal[1],triangles[k].normal[2]);

      glVertex3f(	points[triangles[k].vertices[0]].position[0],
              points[triangles[k].vertices[0]].position[1],
              points[triangles[k].vertices[0]].position[2]);
      glVertex3f(	points[triangles[k].vertices[1]].position[0],
              points[triangles[k].vertices[1]].position[1],
              points[triangles[k].vertices[1]].position[2]);
      glVertex3f( points[triangles[k].vertices[2]].position[0],
              points[triangles[k].vertices[2]].position[1],
              points[triangles[k].vertices[2]].position[2]);
    }
    else
    {
      GLfloat material_diffuse0[] = {points[triangles[k].vertices[0]].color[0],
                           points[triangles[k].vertices[0]].color[1],
                           points[triangles[k].vertices[0]].color[2],
                           points[triangles[k].vertices[0]].color[3]};
      glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,material_diffuse0);
      glNormal3f(points[triangles[k].vertices[0]].normal[0],
                points[triangles[k].vertices[0]].normal[1],
                points[triangles[k].vertices[0]].normal[2]);
      glVertex3f(	points[triangles[k].vertices[0]].position[0],
              points[triangles[k].vertices[0]].position[1],
              points[triangles[k].vertices[0]].position[2]);

      GLfloat material_diffuse1[] = {points[triangles[k].vertices[1]].color[0],
                           points[triangles[k].vertices[1]].color[1],
                               points[triangles[k].vertices[1]].color[2],
                                    points[triangles[k].vertices[1]].color[3]};
      glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,material_diffuse1);
      glNormal3f(points[triangles[k].vertices[1]].normal[0],
               points[triangles[k].vertices[1]].normal[1],
                points[triangles[k].vertices[1]].normal[2]);
      glVertex3f(	points[triangles[k].vertices[1]].position[0],
              points[triangles[k].vertices[1]].position[1],
              points[triangles[k].vertices[1]].position[2]);

      GLfloat material_diffuse2[] = {points[triangles[k].vertices[2]].color[0],
                           points[triangles[k].vertices[2]].color[1],
                           points[triangles[k].vertices[2]].color[2],
                           points[triangles[k].vertices[2]].color[3]};
      glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,material_diffuse2);
      glNormal3f(points[triangles[k].vertices[2]].normal[0],
              points[triangles[k].vertices[2]].normal[1],
              points[triangles[k].vertices[2]].normal[2]);
      glVertex3f(	points[triangles[k].vertices[2]].position[0],
              points[triangles[k].vertices[2]].position[1],
              points[triangles[k].vertices[2]].position[2]);
    }
    glEnd();
  }
}

int GeoXGLWidget3D::addPoint(Point3D p)
{
  points.push_back(p);
  return points.size()-1;
}

int GeoXGLWidget3D::addLine(Line l)
{
  lines.push_back(l);
  return lines.size()-1;
}

int GeoXGLWidget3D::addTriangle(Triangle t)
{
  triangles.push_back(t);
  return triangles.size()-1;
}

Point3D GeoXGLWidget3D::getPoint(int index)
{
  if ((index < 0) || (index > (int)points.size()))
  {
    output << "WARNING: No Point entry at index " << index << " ! A default value is returned!\n";
    return Point3D();
  }
  return points[index];
}

Line GeoXGLWidget3D::getLine(int index)
{
  if ((index < 0) || (index > (int)lines.size()))
  {
    output << "WARNING: No Line entry at index " << index << " ! A default value is returned!\n";
    return Line();
  }
  return lines[index];
}

Triangle GeoXGLWidget3D::getTriangle(int index)
{
  if ((index < 0) || (index > (int)triangles.size()))
  {
    output << "WARNING: No Triangle entry at index " << index << " ! A default value is returned!\n";
    return Triangle();
  }
  return triangles[index];
}

void GeoXGLWidget3D::setPoint(int index, Point3D p)
{
  if ((index < 0) || (index > ((int)points.size()-1)))
  {
    output << "WARNING: Index " << index << " out of bounds!\n";
    output << "WARNING: No changes applied!\n";
    return;
  }
  points[index] = p;
}

void GeoXGLWidget3D::setLine(int index, Line l)
{
  if ((index < 0) || (index > ((int)lines.size()-1)))
  {
    output << "WARNING: Index " << index << " out of bounds!\n";
    output << "WARNING: No changes applied!\n";
    return;
  }
  lines[index] = l;
}

void GeoXGLWidget3D::setTriangle(int index, Triangle t)
{
  if ((index < 0) || (index > ((int)triangles.size()-1)))
  {
    output << "WARNING: Index " << index << " out of bounds!\n";
    output << "WARNING: No changes applied!\n";
    return;
  }
  triangles[index] = t;
}

GeoXGLWidget3D::~GeoXGLWidget3D()
{
}

void GeoXGLWidget3D::mousePressEvent(QMouseEvent *event)
{
  if (mode == PICK)
  {
    findClosestPoint(event->x(),event->y());
  }
  if (mode == CAM || pickedPointIndex == -1)
  {
    camControl->mouseDown(event->x(),event->y(),MouseButtons(event->button()==Qt::LeftButton,event->button()==Qt::RightButton, event->button()==Qt::MidButton),event->modifiers());
  }

  updateGL();
}

void GeoXGLWidget3D::mouseReleaseEvent (QMouseEvent * event)
{
  if((mode == PICK) && (pickedPointIndex != -1))
  {
    double modelview[16], projection[16];
    int viewport[4];
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetIntegerv( GL_VIEWPORT, viewport );
    Vector3d objPos;
    gluUnProject(event->x(), viewport[3]-event->y(), pickedZ, modelview, projection, viewport, &objPos[0], &objPos[1], &objPos[2] );
    points[pickedPointIndex].position = makeVector3f(objPos[0], objPos[1],objPos[2]);
    pickedPointIndex = -1;
    pickedZ = -1;
    recomputeNormal = false;
    emit widgetContentChanged();
  }
  if (mode == CAM || pickedPointIndex == -1)
  {
    camControl->mouseUp(event->x(),event->y(),MouseButtons(event->button()==Qt::LeftButton,event->button()==Qt::RightButton, event->button()==Qt::MidButton),event->modifiers());
  }
  updateGL();
}

void GeoXGLWidget3D::mouseMoveEvent(QMouseEvent *event)
{
  if(mode == CAM || pickedPointIndex == -1)
  {
    camControl->mouseMoved(event->x(),event->y());
    GLfloat new_light_pos[] = {camera.getPosition()[0], camera.getPosition()[1], camera.getPosition()[2], 0.0f};
    glLightfv(GL_LIGHT0,GL_POSITION,new_light_pos);
    GLfloat new_light_spotDir[] = {-camera.getPosition()[0], -camera.getPosition()[1], -camera.getPosition()[2]};
    glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,new_light_spotDir);
  }
  else if(mode == PICK)
  {
    double modelview[16], projection[16];
    int viewport[4];
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetIntegerv( GL_VIEWPORT, viewport );
    Vector3d objPos;
    gluUnProject(event->x(), viewport[3]-event->y(), pickedZ, modelview, projection, viewport, &objPos[0], &objPos[1], &objPos[2] );
    points[pickedPointIndex].position = makeVector3f(objPos[0], objPos[1],objPos[2]);
    recomputeNormal = true;
  }

  updateGL();
}

void GeoXGLWidget3D::resizeGL(int width, int height)
{
  this->resize(width,height);
  glViewport(0,0,this->width(),this->height());
}

void GeoXGLWidget3D::findClosestPoint(int x, int y)
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
    gluProject(points[i].position[0],points[i].position[1],points[i].position[2],
            modelview,projection,viewport,&winX,&winY,&winZ);
    if((makeVector2f(winX,viewport[3]-winY)-makeVector2f(x,y)).getSqrNorm() < 16.0f)
    {
      if(points[i].canBeModified)
      {
        pickedPointIndex = i;
        pickedZ = winZ;
        break;
      }
    }
  }
}

#ifndef CMAKE_INTDIR
#ifdef _WIN32
#include "moc/GeoXGLWidget3D_moc.cpp"
#else
#ifndef CMAKE_UNIX_INCLUDE_MOC
#include "moc_GeoXGLWidget3D.cpp"
#endif
#endif
#endif
