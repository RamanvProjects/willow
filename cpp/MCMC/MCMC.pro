TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += -sdt=c++11
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp \
    src/chainmcmc/chain.cc \
    src/chainmcmc/file.cc \
    src/chainmcmc/logger.cc \
    src/chainmcmc/prior.cc \
    src/chainmcmc/temperature.cc \
    src/chainmcmc/trace.cc

HEADERS += \
    src/chainmcmc/chain.hh \
    src/chainmcmc/chainmcmc.hh \
    src/chainmcmc/file.hh \
    src/chainmcmc/logger.hh \
    src/chainmcmc/parameter.hh \
    src/chainmcmc/prior.hh \
    src/chainmcmc/temperature.hh \
    src/chainmcmc/trace.hh

unix:!macx: LIBS += -L$$PWD/cppa/lib/ -lcppa

INCLUDEPATH += $$PWD/cppa/include
DEPENDPATH += $$PWD/cppa/include
