TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    GPIOController.cpp

HEADERS += \
    GPIOController.h

LIBS += -lpthread
