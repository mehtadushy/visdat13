cmake_minimum_required(VERSION 2.8)

PROJECT(GeoX)

FIND_PACKAGE(Qt4 REQUIRED QtCore QtGui QtOpenGL)
INCLUDE(${QT_USE_FILE})

FIND_PACKAGE(OpenGL REQUIRED)

QT4_WRAP_UI(UISrcs
    geoX/GeoXMainWindowDesigner.ui
    viewers/widgets/GLGeometryViewer3DDesigner.ui
    viewers/widgets/GLGeometryViewerDesigner.ui
    viewers/widgets/LogoViewerDesigner.ui
    system/gui/ObjectBrowserWidgetDesigner.ui
    system/gui/ObjectCreationDialogDesigner.ui
)

QT4_WRAP_CPP(MOCSrcs
    geoX/GeoXMainWindow.h
    system/gui/ClassSelectionDialog.h
    system/gui/MemberBrowser.h
    system/gui/MethodBrowser.h
    system/gui/ObjectBrowserWidget.h
    system/gui/ObjectCreationDialog.h
    system/gui/PopupWindow.h
    system/gui/PropertyBrowser.h
    system/gui/VerticalScrollView.h
    system/properties/BooleanElementEditor.h
    system/properties/ClassTypeClassPropertyEditorWidget.h
    system/properties/ElementEditor.h
    system/properties/EmbeddedObjectElementEditor.h
    system/properties/EnumerationElementEditor.h
    system/properties/FileNameElementEditor.h
    system/properties/ObjectElementEditor.h
    system/properties/ObjectListElementEditor.h
    system/properties/StringElementEditor.h
    system/properties/VariableArrayElementEditor.h
    viewers/widgets/GeoXGLWidget.h
    viewers/widgets/GeoXGLWidget3D.h
    viewers/widgets/GLGeometryViewer.h
    viewers/widgets/GLGeometryViewer3D.h
    viewers/widgets/LogoViewer.h
)

SET(GeoX_SOURCES
    InitGeoX.cpp
    geoX/GeoXMainWindow.cpp
    math/BoundingBox.cpp
    math/DynamicLinearAlgebra.cpp
    math/DynamicLinearAlgebraTools.cpp
    math/Frustum.cpp
    math/HalfSpace.cpp
    math/IterativeSolvers.cpp
    math/LinearAlgebra.cpp
    math/MarchingCubes3D.cpp
    math/PolarCoordinates.cpp
    math/SparseLinearAlgebra.cpp
    math/ViewFrustum.cpp
    system/basics/ClassCreator.cpp
    system/basics/ClassEditor.cpp
    system/basics/ClassMethods.cpp
    system/basics/ClassProperty.cpp
    system/basics/CopyObjectProperties.cpp
    system/basics/ExceptionHandling.cpp
    system/basics/HashTables.cpp
    system/basics/MetaClass.cpp
    system/basics/Object.cpp
    system/basics/Persistent.cpp
    system/gui/ClassSelectionDialog.cpp
    system/gui/FileDialogs.cpp
    system/gui/GeneralKey.cpp
    system/gui/GeoXOutput.cpp
    system/gui/MemberBrowser.cpp
    system/gui/MethodBrowser.cpp
    system/gui/Modifiers.cpp
    system/gui/ObjectBrowserWidget.cpp
    system/gui/ObjectCreationDialog.cpp
    system/gui/ObjectViewsTable.cpp
    system/gui/PopupWindow.cpp
    system/gui/PropertyBrowser.cpp
    system/gui/VerticalScrollView.cpp
    system/misc/ClassInstanceSet.cpp
    system/misc/ImageLoader.cpp
    system/misc/qtHelpers.cpp
    system/misc/StringHelper.cpp
    system/misc/Timer.cpp
    system/misc/Tools.cpp
    system/properties/BooleanClassProperty.cpp
    system/properties/BooleanElementEditor.cpp
    system/properties/BooleanPropertyEditor.cpp
    system/properties/ClassTypeClassProperty.cpp
    system/properties/ClassTypeClassPropertyEditorWidget.cpp
    system/properties/ClassTypePropertyEditor.cpp
    system/properties/CompoundClassProperty.cpp
    system/properties/ElementEditor.cpp
    system/properties/EmbeddedObjectClassProperty.cpp
    system/properties/EmbeddedObjectClassPropertyEditor.cpp
    system/properties/EmbeddedObjectElementEditor.cpp
    system/properties/EnumerationElementEditor.cpp
    system/properties/EnumerationPropertyEditor.cpp
    system/properties/FileNameElementEditor.cpp
    system/properties/FileNamePropertyEditor.cpp
    system/properties/FixedArrayClassProperty.cpp
    system/properties/FixedArrayElementEditor.cpp
    system/properties/FixedArrayPropertyEditor.cpp
    system/properties/NumericalClassProperty.cpp
    system/properties/ObjectClassProperty.cpp
    system/properties/ObjectElementEditor.cpp
    system/properties/ObjectListBase.cpp
    system/properties/ObjectListElementEditor.cpp
    system/properties/ObjectListProperty.cpp
    system/properties/ObjectListPropertyEditor.cpp
    system/properties/ObjectPropertyEditor.cpp
    system/properties/PropertyEditor.cpp
    system/properties/SeparatorClassProperty.cpp
    system/properties/SeparatorPropertyEditor.cpp
    system/properties/SingleClassProperty.cpp
    system/properties/StringClassProperty.cpp
    system/properties/StringElementEditor.cpp
    system/properties/StringPropertyEditor.cpp
    system/properties/StructClassProperty.cpp
    system/properties/VariableArrayClassProperty.cpp
    system/properties/VariableArrayClassPropertyEditor.cpp
    system/properties/VariableArrayElementEditor.cpp
    system/streaming/BinaryObjectStreams.cpp
    system/streaming/LegacyClassNameTranslation.cpp
    system/streaming/ObjectStreams.cpp
    system/streaming/StreamClassTable.cpp
    viewers/cameraControl/Camera.cpp
    viewers/cameraControl/CameraController.cpp
    viewers/cameraControl/ExaminerCameraController.cpp
    viewers/cameraControl/WalkthroughCameraController.cpp
    viewers/widgets/GeoXGLWidget.cpp
    viewers/widgets/GeoXGLWidget3D.cpp
    viewers/widgets/GLGeometryViewer.cpp
    viewers/widgets/GLGeometryViewer3D.cpp
    viewers/widgets/LogoViewer.cpp

    system/misc/AMLoader.cpp
)

SET(Experiment_SOURCES
    experiments/ExampleExperiment.cpp
    experiments/ExampleExperiment2DGraphics.cpp
    experiments/ExampleExperimentScatterPlot.cpp
    experiments/ExampleExperimentFields.cpp
    experiments/ExampleExperiment3DGraphics.cpp
    experiments/ExampleExperimentMarchingCubes.cpp
    experiments/Experiment.cpp
    experiments/ExerimentSimple.cpp
    assignments/Experiment1_1.cpp
    assignments/Experiment1_2.cpp
    assignments/Experiment3_1.cpp
    assignments/Experiment5_1.cpp
    assignments/Experiment5_2.cpp
    assignments/Experiment5_3.cpp
    assignments/Experiment7_1.cpp
)

SET(GeoX_SOURCES ${GeoX_SOURCES}
    #experiments/ExperimentParallelCoordinates.cpp
)

QT4_ADD_RESOURCES(GeoX_resources images/geoX.qrc)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${OPENGL_INCLUDE_DIR}
                    experiments assignments geoX math system/basics system/gui system/misc
                    system/properties system/streaming viewers/cameraControl viewers/widgets
                    windows)

SET(ALLLIBS ${QT_QTMAIN_LIBRARY} ${QT_LIBRARIES} ${OPENGL_LIBRARIES})

IF(WIN32) # Check if we are on Windows
  SET(GeoX_SOURCES ${GeoX_SOURCES} windows/stdafx.cpp windows/winmain.cpp unix/main.cpp) #We need the main.cpp, because CMake is stupid and always creates us with /SUBSYSTEM:CONSOLE
  SET(ALLLIBS ${ALLLIBS} winmm)
ELSE()
  SET(GeoX_SOURCES ${GeoX_SOURCES} unix/main.cpp)
ENDIF()

ADD_EXECUTABLE(GeoX ${GeoX_SOURCES} ${Experiment_SOURCES} ${MOCSrcs} ${UISrcs} ${GeoX_resources})

TARGET_LINK_LIBRARIES(GeoX ${ALLLIBS})

IF(WIN32) # Check if we are on Windows
  IF(MSVC) # Check if we are using the Visual Studio compiler
    SET_TARGET_PROPERTIES(GeoX PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
  ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
    # SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
  ELSE()
    MESSAGE(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
  ENDIF()
ELSEIF(UNIX)
  ADD_DEFINITIONS(-DCMAKE_UNIX_INCLUDE_MOC -Wno-conversion-null -Wno-multichar)
ELSE()
  #message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
ENDIF()
