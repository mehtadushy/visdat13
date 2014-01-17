//---------------------------------------------------------------------------
#ifndef GeoXMainWindowH
#define GeoXMainWindowH
//---------------------------------------------------------------------------
#include "PTypes.h"
#include "ClassInstanceSet.h"
#include "VerticalScrollView.h"
//---------------------------------------------------------------------------
#include "ui_GeoXMainWindowDesigner.h"
//---------------------------------------------------------------------------
#include <string>
//---------------------------------------------------------------------------

class QCloseEvent;
class QTimer;
class Experiment;

class GeoXMainWindow : public QMainWindow {
  Q_OBJECT

 private:
   Ui_GeoXMainWindow ui;
   QTimer *initTimer;
   QWidget *displayWidget;

   // experiments editor panel
   ClassEditor *experimentsEditor;
   QWidget *experimentsEditorControl;
  Experiment *currentExperiment;
   ClassInstanceSet oldExperiments;
   VerticalScrollView *experimentView;


 private slots:
   void on_actionOpen_triggered();
   void on_actionSaveAs_triggered();
   void on_actionExit_triggered();
   void initWindow();
   void sltExperimentChanged(const QString &newExperimentName);
  void sltViewerContentChanged();

 protected:
    void closeEvent(QCloseEvent *event);
    void changeExperiment(const MetaClass *newExperimentClass);

 public:
  GeoXMainWindow(QWidget * parent=NULL, Qt::WFlags f=0);

   void setDisplayWidget(QWidget *w);
   QWidget *getDisplayWidget() {return displayWidget;}

   void output(bool b);
   void output(int i);
   void output(unsigned int i);
   void output(long unsigned int i);
   void output(int64 i);
   void output(card64 i);
   void output(long double d);
   void output(std::string s);
   void warningMessage(std::string message);
   void errorMessage(std::string message);

  ~GeoXMainWindow();
};

extern GeoXMainWindow *mainWindow;





#endif

