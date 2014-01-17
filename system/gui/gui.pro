# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./../system/gui
# Target is a library:  

CONFIG += debug \
          warn_on \
          staticlib 
TEMPLATE = lib 
FORMS += ObjectBrowserWidgetDesigner.ui \
         ObjectCreationDialogDesigner.ui 

HEADERS += ClassSelectionDialog.h \
           Editor.h \
           FileDialogs.h \
           GeneralKey.h \
           GeoXOutput.h \
           KeyboardController.h \
           MemberBrowser.h \
           MethodBrowser.h \
           Modifiers.h \
           MouseButtons.h \
           MouseController.h \
           ObjectBrowserWidget.h \
           ObjectCreationDialog.h \
           ObjectViewsTable.h \
           PopupWindow.h \
           PropertyBrowser.h \
           VerticalScrollView.h 

SOURCES += ClassSelectionDialog.cpp \
           FileDialogs.cpp \
           GeneralKey.cpp \
           GeoXOutput.cpp \
           MemberBrowser.cpp \
           MethodBrowser.cpp \
           Modifiers.cpp \
           ObjectBrowserWidget.cpp \
           ObjectCreationDialog.cpp \
           ObjectViewsTable.cpp \
           PopupWindow.cpp \
           PropertyBrowser.cpp \
           VerticalScrollView.cpp

CONFIG -= release


INCLUDEPATH += ../../viewers/widgets \
  ../../viewers/cameraControl \
  ../../geoX \
  ../../math \
  ../../windows \
  ../../system/basics \
  ../../system/streaming \
  ../../system/properties \
  ../../system/misc \
  ../../system/gui \
  $QTDIR/include/

