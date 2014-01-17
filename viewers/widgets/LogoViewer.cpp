//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "LogoViewer.h"
//---------------------------------------------------------------------------
#include "ExceptionHandling.h"
//---------------------------------------------------------------------------

LogoViewer::LogoViewer(QWidget * parent, Qt::WFlags f)
: QWidget(parent, f)
{
   ui.setupUi(this);
}

LogoViewer::~LogoViewer()
{
}

#ifndef CMAKE_INTDIR
#ifdef _WIN32
#include "moc/LogoViewer_moc.cpp"
#else
#ifndef CMAKE_UNIX_INCLUDE_MOC
#include "moc_LogoViewer.cpp"
#endif
#endif
#endif
