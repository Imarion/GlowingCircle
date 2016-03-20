QT += gui core

CONFIG += c++11

TARGET = GlowingCircle
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    GlowingCircle.cpp \
    vertex.cpp \
    vertexcol.cpp \
    vertextex.cpp

HEADERS += \
    GlowingCircle.h \
    vertex.h \
    vertexcol.h \
    vertextex.h

OTHER_FILES += \
    fshader.txt \
    vshader.txt \

RESOURCES += \
    shaders.qrc
