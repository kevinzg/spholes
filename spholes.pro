#-------------------------------------------------
#
# Project created by QtCreator 2017-10-20T16:00:46
#
#-------------------------------------------------

QT       += core gui
qtHaveModule(printsupport): QT += printsupport
qtHaveModule(opengl): QT += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = spholes
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp \
    src/View.cpp \
    src/Scene.cpp \
    src/PointItem.cpp \
    src/Style.cpp \
    src/spholes/ShortestPath.cpp \
    src/spholes/VisibilityGraph.cpp

HEADERS += \
    src/MainWindow.h \
    src/View.h \
    src/Scene.h \
    src/PointItem.h \
    src/Style.h \
    src/spholes/Point.h \
    src/spholes/Common.h \
    src/spholes/Polygon.h \
    src/spholes/LineSegment.h \
    src/spholes/ShortestPath.h \
    src/spholes/Path.h \
    src/spholes/Hash.h \
    src/spholes/Graph.h \
    src/spholes/VisibilityGraph.h

RESOURCES += \
    resources/images.qrc
