//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "InitGeoX.h"
//---------------------------------------------------------------------------
#include "Camera.h"
#include "ViewFrustum.h"
#include "CameraController.h"
#include "ClassCreator.h"
#include "ClassEditor.h"
#include "ExaminerCameraController.h"
#include "Persistent.h"
#include "PropertyBrowser.h"
#include "PropertyEditor.h"
#include "WalkthroughCameraController.h"
#include "StreamClassTable.h"
#include "LegacyClassNameTranslation.h"

#include "BooleanPropertyEditor.h"
#include "ObjectPropertyEditor.h"
#include "FixedArrayPropertyEditor.h"
#include "ClassTypeClassProperty.h"
#include "ClassTypeClassPropertyEditorWidget.h"

#include "Experiment.h"
#include "ExampleExperiment.h"
#include "ExampleExperiment2DGraphics.h"
#include "ExampleExperiment3DGraphics.h"
#include "ExampleExperimentMarchingCubes.h"
#include "ExampleExperimentFields.h"
#include "ExampleExperimentScatterPlot.h"
#include "ExerimentSimple.h"

// ...
// Visualization and Data Analaysis 2013/14 Experiments
#include "Experiment1_1.h"
#include "Experiment1_2.h"
#include "Experiment3_1.h"
#include "Experiment5_1.h"
#include "Experiment5_2.h"
#include "Experiment5_3.h"
#include "Experiment7_1.h"
#include "Experiment9_1.h"

// Geometric Modelling 2014 Experiments
#include "GMExperiment2_1.h"
#include "GMExperiment4_1.h"
#include "GMExperiment6_1.h"
#include "GMExperiment8_1.h"

// ...

//---------------------------------------------------------------------------

void init()
{
    Object::init(NULL);
    ClassCreator::init(Object::getClass());
    PropertyEditor::init(Object::getClass());
    BooleanPropertyEditor::init(PropertyEditor::getClass());
    ObjectPropertyEditor::init(PropertyEditor::getClass());
    FixedArrayPropertyEditor::init(PropertyEditor::getClass());
    ClassEditor::init(Object::getClass());
    PropertyBrowser::init(ClassEditor::getClass());
    Persistent::init(Object::getClass());
    StreamClassTable::init(Persistent::getClass());
    Camera::init(Persistent::getClass());
    ViewFrustum::init(Persistent::getClass());
    CameraController::init(Persistent::getClass());
    ExaminerCameraController::init(CameraController::getClass());
    WalkthroughCameraController::init(CameraController::getClass());

    Experiment::init(Persistent::getClass());
    ExampleExperiment::init(Experiment::getClass());
    ExampleExperiment2DGraphics::init(Experiment::getClass());
    ExampleExperiment3DGraphics::init(Experiment::getClass());
    ExampleExperimentMarchingCubes::init(Experiment::getClass());
    ExampleExperimentFields::init(Experiment::getClass());
    ExampleExperimentScatterPlot::init(Experiment::getClass());

    // ...
    // register your experiments here

    ExerimentSimple::init(Experiment::getClass());
    //Visdat 13/14 Experiments
    Experiment1_1::init(Experiment::getClass());
    Experiment1_2::init(Experiment::getClass());
    Experiment3_1::init(Experiment::getClass());
    Experiment5_1::init(Experiment::getClass());
    Experiment5_2::init(Experiment::getClass());
    Experiment5_3::init(Experiment::getClass());
    Experiment7_1::init(Experiment::getClass());
    Experiment9_1::init(Experiment::getClass());

    //GeoMod 14 Experiments
    GMExperiment2_1::init(Experiment::getClass());
    GMExperiment4_1::init(Experiment::getClass());
    GMExperiment6_1::init(Experiment::getClass());
    GMExperiment8_1::init(Experiment::getClass());

    // ...
}

void shutdown()
{
    Object::shutdown();
    ClassEditor::shutdown();
    Persistent::shutdown();
    StreamClassTable::shutdown();
    Camera::shutdown();
    ViewFrustum::shutdown();
    CameraController::shutdown();
    ExaminerCameraController::shutdown();
    WalkthroughCameraController::shutdown();
    BooleanPropertyEditor::shutdown();
    ObjectPropertyEditor::shutdown();
    FixedArrayPropertyEditor::shutdown();
    PropertyBrowser::shutdown();

    Experiment::shutdown();
    ExampleExperiment::shutdown();
    ExampleExperiment2DGraphics::shutdown();
    ExampleExperiment3DGraphics::shutdown();
    ExampleExperimentMarchingCubes::shutdown();
    ExampleExperimentFields::shutdown();
    ExampleExperimentScatterPlot::shutdown();

    // ...
    // unregister your experiments here

    ExerimentSimple::shutdown();
    //Visdat 13/14 Experiments
    Experiment1_1::shutdown();
    Experiment1_2::shutdown();
    Experiment3_1::shutdown();
    Experiment5_1::shutdown();
    Experiment5_2::shutdown();
    Experiment5_3::shutdown();
    Experiment7_1::shutdown();
    Experiment9_1::shutdown();
    //GeoMod 14 Experiments
    GMExperiment2_1::shutdown();
    GMExperiment4_1::shutdown();
    GMExperiment6_1::shutdown();
    GMExperiment8_1::shutdown();
    // ...
}
