//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "GLGeometryViewer.h"
//---------------------------------------------------------------------------
#include "ExceptionHandling.h"
//---------------------------------------------------------------------------

GLGeometryViewer::GLGeometryViewer(QWidget * parent, Qt::WFlags f)
: QWidget(parent, f)
{
   ui.setupUi(this);
  connect(ui.glFrame,SIGNAL(widgetContentChanged()),this,SLOT(sltWidgetContentChanged()));
  connect(ui.glFrame,SIGNAL(pointSelected(int)),this,SLOT(sltPointSelected(int)));

  cbPointSelected = NULL;
}

GLGeometryViewer::~GLGeometryViewer()
{
}

int GLGeometryViewer::addPoint(Point2D p)
{
  return ui.glFrame->addPoint(p);
}

Point2D GLGeometryViewer::getPoint(int index)
{
  return ui.glFrame->getPoint(index);
}

void GLGeometryViewer::setPoint(int index, Point2D p)
{
  ui.glFrame->setPoint(index,p);
}


int GLGeometryViewer::addLine(Line l)
{
  return ui.glFrame->addLine(l);
}

int GLGeometryViewer::addLine(Vector2f pos1, Vector2f pos2,
                              const Vector4f& color /*= makeVector4f(0.0f,1.0f,1.0f,1.0f)*/,
                              const float32 thickness /*= 1.0f*/, const float32 point_thickness /*= 0.0f*/)
{
  Point2D P1(pos1);
  P1.size = point_thickness;
  Point2D P2(pos2);
  P2.size = point_thickness;

  Line l;
  l.vertices[0] = ui.glFrame->addPoint(P1);
  l.vertices[1] = ui.glFrame->addPoint(P2);
  l.color = color;
  l.thickness = thickness;
  return ui.glFrame->addLine(l);
}

int GLGeometryViewer::addLine(float pos1X, float pos1Y, float pos2X, float pos2Y,
                              const Vector4f& color /*= makeVector4f(0.0f,1.0f,1.0f,1.0f)*/,
                              const float32 thickness /*= 1.0f*/, const float32 point_thickness /*= 0.0f*/)
{
  Vector2f p1;
  p1[0]= pos1X;
  p1[1]= pos1Y;
  Vector2f p2;
  p2[0]= pos2X;
  p2[1]= pos2Y;

  return addLine(p1, p2, color, thickness, point_thickness);
}

Line GLGeometryViewer::getLine(int index)
{
  return ui.glFrame->getLine(index);
}

void GLGeometryViewer::setLine(int index, Line l)
{
  ui.glFrame->setLine(index,l);
}

int GLGeometryViewer::addTriangle(Triangle t)
{
  return ui.glFrame->addTriangle(t);
}

int GLGeometryViewer::addTriangle(Vector2f pos1, Vector2f pos2, Vector2f pos3)
{
  Triangle t;
  t.vertices[0] = ui.glFrame->addPoint(Point2D(pos1));
  t.vertices[1] = ui.glFrame->addPoint(Point2D(pos2));
  t.vertices[2] = ui.glFrame->addPoint(Point2D(pos3));
  return ui.glFrame->addTriangle(t);
}

int GLGeometryViewer::addTriangle(float pos1X, float pos1Y, float pos2X, float pos2Y, float pos3X, float pos3Y)
{
  Triangle t;
  t.vertices[0] = ui.glFrame->addPoint(Point2D(pos1X,pos1Y));
  t.vertices[1] = ui.glFrame->addPoint(Point2D(pos2X,pos2Y));
  t.vertices[2] = ui.glFrame->addPoint(Point2D(pos3X,pos3Y));
  return ui.glFrame->addTriangle(t);
}

Triangle GLGeometryViewer::getTriangle(int index)
{
  return ui.glFrame->getTriangle(index);
}

void GLGeometryViewer::setTriangle(int index, Triangle t)
{
  ui.glFrame->setTriangle(index,t);
}

void GLGeometryViewer::clear()
{
  ui.glFrame->clear();
}

void GLGeometryViewer::clearLines()
{
  ui.glFrame->clearLines();
}

void GLGeometryViewer::clearTriangles()
{
  ui.glFrame->clearTriangles();
}

void GLGeometryViewer::on_btnCamera_pressed()
{
  ui.glFrame->setCameraMode();
}
void GLGeometryViewer::on_btnPick_pressed()
{
  ui.glFrame->setPickMode();
}
void GLGeometryViewer::on_btnClear_pressed()
{
  ui.glFrame->clear();
}
void GLGeometryViewer::on_btnCreate_pressed()
{
  ui.glFrame->setCreateMode();
}

void GLGeometryViewer::on_btnDrawAxis_pressed()
{
  ui.glFrame->swapDrawAxis();
}

void GLGeometryViewer::on_btnDrawPoints_pressed()
{
  ui.glFrame->swapDrawPoints();
}

void GLGeometryViewer::on_btnDrawLines_pressed()
{
  ui.glFrame->swapDrawLines();
}

void GLGeometryViewer::on_btnDrawTriangles_pressed()
{
  ui.glFrame->swapDrawTriangles();
}

void GLGeometryViewer::on_btnDrawWithGlow_pressed()
{
  ui.glFrame->swapDrawWithGlow();
}

void GLGeometryViewer::on_btnDrawTexture_pressed()
{
  ui.glFrame->swapDrawTexture();
}

void GLGeometryViewer::sltWidgetContentChanged()
{
  emit viewerContentChanged();
}

void GLGeometryViewer::sltPointSelected(int index)
{
  emit pointSelected(index);
  if (cbPointSelected) cbPointSelected(index);
}

#ifndef CMAKE_INTDIR
#ifdef _WIN32
#include "moc/GLGeometryViewer_moc.cpp"
#else
#ifndef CMAKE_UNIX_INCLUDE_MOC
#include "moc_GLGeometryViewer.cpp"
#endif
#endif
#endif

