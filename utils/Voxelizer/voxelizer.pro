QT += core gui opengl

TARGET = Voxelizer
TEMPLATE = app

win32 {
    DEFINES += GLEW_STATIC
    LIBS += -lopengl32
}

LIBS += \
       -lboost_program_options\

CONFIG += g++ -sdt=c++11
QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += src libs glm libs/glew-1.10.0/include
DEPENDPATH += src libs glm libs/glew-1.10.0/include


SOURCES += \
    src/engine/graphics/camera.cpp \
    src/view.cpp \
    src/engine/base/screen.cpp \
    src/mainwindow.cpp \
    src/main.cpp \
    libs/glew-1.10.0/src/glew.c \
    src/engine/base/application.cpp \
    src/engine/math/interval.cpp \
    src/engine/math/collision.cpp \
    src/util/ResourceLoader.cpp \
    src/engine/graphics/shape.cpp \
    src/engine/graphics/graphics.cpp \
    src/engine/math/procedural.cpp \
    src/engine/geometric/response.cpp \
    src/util/obj.cpp \
    src/engine/geometric/mesh/graphicalmesh.cpp \
    src/engine/graphics/shader.cpp \
    src/engine/voxelizer.cpp \
    src/engine/geometric/geometriccollision.cpp \
    src/debugscreen.cpp

HEADERS += \
    libs/glew-1.10.0/include/GL/glew.h \
    src/view.h \
    src/engine/base/screen.h \
    src/mainwindow.h \
    src/engine/base/application.h \
    src/engine/graphics/camera.h \
    src/engine/math/interval.h \
    src/engine/math/collision.h \
    src/util/ResourceLoader.h \
    src/util/CylinderData.h \
    src/util/CommonIncludes.h \
    src/engine/graphics/shape.h \
    src/engine/graphics/graphics.h \
    src/engine/math/procedural.h \
    src/engine/geometric/ellipsoid.h \
    src/engine/geometric/response.h \
    src/util/obj.h \
    src/util/SphereData.h \
    src/engine/geometric/mesh/graphicalmesh.h \
    src/engine/geometric/mesh/meshtriangle.h \
    src/engine/graphics/shader.h \
    src/engine/voxelizer.h \
    src/engine/datastructs/octree.h \
    src/engine/geometric/geometriccollision.h \
    src/debugscreen.h \
    src/ui_mainwindow.h

DISTFILES += \
    res/shaders/shader.vert \
    res/shaders/shader.frag
