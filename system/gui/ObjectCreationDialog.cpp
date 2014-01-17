//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "ObjectCreationDialog.h"
#include "ClassEditor.h"
#include "ClassCreator.h"
#include "ExceptionHandling.h"
//---------------------------------------------------------------------------
#include <QtGui/QVBoxLayout>
//---------------------------------------------------------------------------


ObjectCreationDialog::ObjectCreationDialog(QWidget* parent) {
  ui.setupUi(this);
}

Object *ObjectCreationDialog::execute(QWidget *parent, ClassCreator *cc) {
  Object *result = NULL;

  STD_GLOBAL_EXCEPTION_HANDLER_START
  ObjectCreationDialog *diag = new ObjectCreationDialog(parent);
  pAssert(cc != NULL);
  if (cc == NULL) return NULL;
  const MetaClass *edmc = cc->getInstanceClass()->getGUIDescriptor()->getDefaultClassEditor();
  pAssert(edmc != NULL);
  if (edmc == NULL) return NULL;
  ClassEditor *ed = (ClassEditor*)edmc->newInstance();
  QVBoxLayout *l = new QVBoxLayout(diag->ui.frameCreatorEditor);
  l->setMargin(0);
  l->setSpacing(0);
  l->addWidget(ed->makeEditor(diag->ui.frameCreatorEditor, cc));
  if (diag->exec() == QDialog::Accepted) {
    result = cc->makeObject();
  }
  delete ed;
  STD_GLOBAL_EXCEPTION_HANDLER_END

  return result;
}

#ifndef CMAKE_INTDIR
#ifdef _WIN32
#include "moc/ObjectCreationDialog_moc.cpp"
#else
#ifndef CMAKE_UNIX_INCLUDE_MOC
#include "moc_ObjectCreationDialog.cpp"
#endif
#endif
#endif
