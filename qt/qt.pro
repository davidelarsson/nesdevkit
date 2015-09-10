TEMPLATE = app
QT += widgets
TARGET = ../bin/nesdevkit-qt
INCLUDEPATH += . ../core
LIBS += ../core/libnescore.a
SOURCES += qt.cpp
