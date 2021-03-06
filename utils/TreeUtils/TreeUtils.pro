TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle

LIBS += \
       -lboost_program_options\

QMAKE_CXXFLAGS += -std=c++14
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3
QMAKE_CXXFLAGS += -Wall
DEPENDPATH += libs glm
INCLUDEPATH += libs glm

SOURCES += src/main.cpp \
    src/parser.cpp \
    src/pointcloud.cpp \
    src/pathparser.cpp

HEADERS += \
    src/parser.h \
    src/CommonIncludes.h \
    src/pointcloud.h \
    src/pathparser.h
