TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

LIBS += \
       -lboost_program_options\

QMAKE_CXXFLAGS += -std=c++11
DEPENDPATH += libs glm
INCLUDEPATH += libs glm

SOURCES += src/main.cpp \
    src/parser.cpp \
    src/pointcloud.cpp

HEADERS += \
    src/parser.h \
    src/CommonIncludes.h \
    src/pointcloud.h
