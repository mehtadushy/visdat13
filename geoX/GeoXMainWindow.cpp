//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "GeoXMainWindow.h"
#include "GeoXGLWidget.h"
//---------------------------------------------------------------------------
#include "ExceptionHandling.h"
#include "StringHelper.h"
#include "Experiment.h"
#include "FileDialogs.h"
#include "BinaryObjectStreams.h"
//---------------------------------------------------------------------------
#include <QtGui/qevent.h>
#include <QtGui/QMessageBox>
#include <QtCore/QTimer>
#include <QtGui/QFrame>
#include <QtGui/QVBoxLayout>
#include <sstream>
#include <iostream>
//---------------------------------------------------------------------------

GeoXMainWindow *mainWindow = NULL;

GeoXMainWindow::GeoXMainWindow(QWidget * parent, Qt::WFlags f)
:QMainWindow(parent, f)
{
  ui.setupUi(this);
  //move(0,0);
  ui.dockOutput->setMaximumHeight(100);
   QVBoxLayout* tl=new QVBoxLayout;
   tl->setMargin(0);
   tl->setSpacing(0);
   ui.mainFrame->setLayout(tl);

  setWindowTitle( "GeoX Main Widget" );
  setAttribute(Qt::WA_DeleteOnClose);
  initTimer = new QTimer(this);
   connect(initTimer, SIGNAL(timeout()), this, SLOT(initWindow()));
   initTimer->start(100);
   displayWidget = NULL;

   experimentsEditor = NULL;
   experimentsEditorControl = NULL;
   currentExperiment = NULL;

   QVBoxLayout* tv=new QVBoxLayout;
   tv->setMargin(0);
   tv->setSpacing(0);
   ui.experimentObjectViewFrame->setLayout(tv);
   experimentView = new VerticalScrollView( ui.experimentObjectViewFrame );
   ui.experimentObjectViewFrame->layout()->addWidget( experimentView );

   ui.experimentSelector->collectClasses("Experiment", false);
   QWidget::connect(ui.experimentSelector, SIGNAL(currentIndexChanged(QString)), this , SLOT(sltExperimentChanged(QString)));

   GeoXGLWidget::initOpenGLFormat();
}

void GeoXMainWindow::initWindow()
{
   initTimer->stop();
   delete initTimer;
   initTimer = NULL;
   ui.dockOutput->setMaximumHeight(10000);
   ui.dockObjectView->setMinimumWidth(50);
}


GeoXMainWindow::~GeoXMainWindow()
{
}


void GeoXMainWindow::on_actionOpen_triggered()
{
  STD_GLOBAL_EXCEPTION_HANDLER_START
   string filename = FileDialogs::getOpenFileName(this, "Open Experiment File...",
      "GeoX Objects (*.object);;All Files (*.*)");
  if (!filename.empty()) {
     BinaryInputObjectStream in(filename.c_str());
     Persistent *p = NULL;
     in.readAnyObject(p);
     if (dynamic_cast<Experiment*>(p) == NULL) {
        delete p;
        errorMessage("file is not an Experiment");
     } else {
         if (currentExperiment != NULL) {
            currentExperiment->deactivate();
         }
         currentExperiment = NULL;

         if (experimentsEditor != NULL) {
            delete experimentsEditor;
         }
         experimentsEditor = NULL;
         if (experimentsEditorControl != NULL) {
            delete experimentsEditorControl;
         }
         experimentsEditorControl = NULL;

        oldExperiments.insertObject(p);
        changeExperiment(p->getInstanceClass());
     }
  }
  STD_GLOBAL_EXCEPTION_HANDLER_END
}

void GeoXMainWindow::on_actionSaveAs_triggered()
{
  STD_GLOBAL_EXCEPTION_HANDLER_START
  if (currentExperiment == NULL) {
      warningMessage("no active experiment, cannot save");
     return;
  }
   string filename = FileDialogs::getSaveFileName(this, "Save Experiment File...",
      "GeoX Objects (*.object);;All Files (*.*)");
   if (!filename.empty()) {
      BinaryOutputObjectStream out(filename.c_str());
      out.writeObject(currentExperiment);
   }
  STD_GLOBAL_EXCEPTION_HANDLER_END
}

void GeoXMainWindow::on_actionExit_triggered()
{
  close();
}

void GeoXMainWindow::closeEvent(QCloseEvent *event) {

  if (QMessageBox::question(this, "Confirm closing...", "Do you want to close GeoX?", QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok) {
    event->setAccepted(true);
    // // fix qt413 bug
    // QApplication::exit(0);
  } else {
    event->setAccepted(false);
  }
}

void GeoXMainWindow::sltExperimentChanged(const QString &newExperimentName) {
   STD_GLOBAL_EXCEPTION_HANDLER_START
   std::string  s = qString2STLString(newExperimentName);
   const MetaClass *newExperimentClass = MetaClass::getClassByName(s);
   if (newExperimentClass == NULL ) return;
   changeExperiment( newExperimentClass );
   STD_GLOBAL_EXCEPTION_HANDLER_END
}

void GeoXMainWindow::sltViewerContentChanged()
{
  STD_GLOBAL_EXCEPTION_HANDLER_START
  currentExperiment->update();
  STD_GLOBAL_EXCEPTION_HANDLER_END
}

void GeoXMainWindow::changeExperiment(const MetaClass * newExperimentClass)
{
   STD_GLOBAL_EXCEPTION_HANDLER_START
   string s = newExperimentClass->getClassName();

   if (newExperimentClass == NULL ) return;
   ui.experimentSelector->setSelection(s);

   if (currentExperiment != NULL) {
      currentExperiment->deactivate();
      oldExperiments.insertObject(currentExperiment);
      currentExperiment = NULL;
   }

   if (experimentsEditor != NULL) {
      delete experimentsEditor;
      experimentsEditor = NULL;
   }

   if (experimentsEditorControl != NULL) {
      delete experimentsEditorControl;
   }
   experimentsEditorControl = NULL;

   if (newExperimentClass == NULL || newExperimentClass->isAbstract()) return;

   if (!oldExperiments.containsObjectOfClass(newExperimentClass))
   {
      currentExperiment = (Experiment*)(newExperimentClass->newInstance());
      oldExperiments.insertObject(currentExperiment);
   } else {
      currentExperiment = (Experiment*)(oldExperiments.getInstance(newExperimentClass));
   }
   setDisplayWidget(currentExperiment->createViewer());
   currentExperiment->activate();
   const MetaClass *newExperimentEditorClass = newExperimentClass->getGUIDescriptor()->getDefaultClassEditor();

   if (newExperimentEditorClass == NULL || newExperimentEditorClass->isAbstract()) return;

   experimentsEditor = (ClassEditor*)(newExperimentEditorClass->newInstance());
   experimentsEditorControl = experimentsEditor->makeEditor(experimentView->viewport() , currentExperiment);
   experimentView->setWidget(experimentsEditorControl);
   experimentsEditorControl->show();

   STD_GLOBAL_EXCEPTION_HANDLER_END
}



void GeoXMainWindow::setDisplayWidget(QWidget *w)
{
   if (displayWidget != NULL) {
      try {
         delete displayWidget;
      } catch (...) {
         displayWidget = NULL;
         throw;
      }
      displayWidget = NULL;
   }
   displayWidget = w;
   ui.mainFrame->layout()->addWidget(w);
   connect(w,SIGNAL(viewerContentChanged()),this,SLOT(sltViewerContentChanged())); // viewer nicht widget, oberklasse hat das signal nicht, viewer hat kein glwidget zum angreifen, was ist im 3d fall
}

#pragma warning( disable : 4717 )
void GeoXMainWindow::output(bool b) {
   if (b) output("true"); else output("false");
}

void GeoXMainWindow::output(int i) {
   output(intToStr(i));
}

void GeoXMainWindow::output(unsigned int i) {
   output(intToStr((int)i));
}

void GeoXMainWindow::output(long unsigned int i) {
   output(intToStr((int)i));
}

void GeoXMainWindow::output(int64 i) {
   output(intToStr(i));
}

void GeoXMainWindow::output(card64 i) {
   output(intToStr((int64)i));
}

void GeoXMainWindow::output(long double d) {
   std::string s;
   ostringstream os;
   os << d;
   s = os.str();
   output(s);
}

void GeoXMainWindow::output(std::string s) {
   ui.outputWindow->moveCursor( QTextCursor::End );
   QString qs = s.c_str();
   qs.replace(QRegExp("a[\n]"), "\r\n");
   ui.outputWindow->insertPlainText(qs);
}

void GeoXMainWindow::warningMessage(string message) {
   output("!! - WARNING: ");
   output(message);
   output("\n");
}

void GeoXMainWindow::errorMessage(string message) {
   output("!! - ERROR: ");
   output(message);
   output("\n");
}

#ifndef CMAKE_INTDIR
#ifdef _WIN32
#include "moc/GeoXMainWindow_moc.cpp"
#else
#ifndef CMAKE_UNIX_INCLUDE_MOC
#include "moc_GeoXMainWindow.cpp"
#endif
#endif
#endif
