//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "GLGeometryViewer3D.h"
//---------------------------------------------------------------------------
#include "ExceptionHandling.h"
//---------------------------------------------------------------------------

GLGeometryViewer3D::GLGeometryViewer3D(QWidget * parent, Qt::WFlags f)
: QWidget(parent, f)
{
   ui.setupUi(this);
  connect(ui.glFrame,SIGNAL(widgetContentChanged()),this,SLOT(sltWidgetContentChanged()));
}

GLGeometryViewer3D::~GLGeometryViewer3D()
{
}

int GLGeometryViewer3D::addPoint(Point3D p)
{
  return ui.glFrame->addPoint(p);
}

Point3D GLGeometryViewer3D::getPoint(int index)
{
  return ui.glFrame->getPoint(index);
}

void GLGeometryViewer3D::setPoint(int index, Point3D p)
{
  return ui.glFrame->setPoint(index,p);
}

int GLGeometryViewer3D::addLine(Line l)
{
  return ui.glFrame->addLine(l);
}

int GLGeometryViewer3D::addLine(Vector3f pos1, Vector3f pos2)
{
  Line l;
  l.vertices[0] = ui.glFrame->addPoint(Point3D(pos1));
  l.vertices[1] = ui.glFrame->addPoint(Point3D(pos2));
  return ui.glFrame->addLine(l);
}

Line GLGeometryViewer3D::getLine(int index)
{
  return ui.glFrame->getLine(index);
}

void GLGeometryViewer3D::setLine(int index, Line l)
{
  return ui.glFrame->setLine(index,l);
}


int GLGeometryViewer3D::addTriangle(Triangle t)
{
  return ui.glFrame->addTriangle(t);
}

int GLGeometryViewer3D::addTriangle(Vector3f pos1, Vector3f pos2, Vector3f pos3)
{
  Triangle t;
  t.vertices[0] = ui.glFrame->addPoint(Point3D(pos1));
  t.vertices[1] = ui.glFrame->addPoint(Point3D(pos2));
  t.vertices[2] = ui.glFrame->addPoint(Point3D(pos3));
  return ui.glFrame->addTriangle(t);
}

Triangle GLGeometryViewer3D::getTriangle(int index)
{
  return ui.glFrame->getTriangle(index);
}

void GLGeometryViewer3D::setTriangle(int index, Triangle t)
{
  return ui.glFrame->setTriangle(index,t);
}

void GLGeometryViewer3D::on_btnCamera_pressed()
{
  ui.glFrame->setCameraMode();
}
void GLGeometryViewer3D::on_btnPick_pressed()
{
  ui.glFrame->setPickMode();
}
void GLGeometryViewer3D::on_btnClear_pressed()
{
  ui.glFrame->clear();
}

void GLGeometryViewer3D::on_btnLight_pressed()
{
  ui.glFrame->swapLighting();
}

void GLGeometryViewer3D::on_btnFillMode_pressed()
{
  ui.glFrame->swapFillMode();
}

void GLGeometryViewer3D::on_btnDrawAxis_pressed()
{
  ui.glFrame->swapDrawAxis();
}

void GLGeometryViewer3D::on_btnHandles_pressed()
{
  ui.glFrame->swapHandleDrawing();
}

void GLGeometryViewer3D::on_btnResetCamera_pressed()
{
  ui.glFrame->resetCamera();
  ui.glFrame->updateGL();
}
void GLGeometryViewer3D::on_btnVertexLighting_pressed()
{
  ui.glFrame->swapLightingMode();
  ui.glFrame->updateGL();
}

void GLGeometryViewer3D::sltWidgetContentChanged()
{
  emit viewerContentChanged();
}

#ifndef CMAKE_INTDIR
#ifdef _WIN32
#include "moc/GLGeometryViewer3D_moc.cpp"
#else
#ifndef CMAKE_UNIX_INCLUDE_MOC
#include "moc_GLGeometryViewer3D.cpp"
#endif
#endif
#endif
