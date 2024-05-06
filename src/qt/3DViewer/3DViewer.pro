QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(../../3rdParty/gifimage/qtgifimage.pri)

SOURCES += \
    ../../graphics/matrix.c \
    ../../graphics/matrix_transform.c \
    ../../graphics/obj.c \
    ../../graphics/obj_transform.c \
    ../../graphics/vertex.c \
    color_pick.cpp \
    main.cpp \
    mainwindow.cpp \
    openglwindow.cpp

HEADERS += \
    ../../graphics/matrix.h \
    ../../graphics/matrix_transform.h \
    ../../graphics/obj.h \
    ../../graphics/obj_transform.h \
    ../../graphics/vertex.h \
    color_pick.h \
    mainwindow.h \
    openglwindow.h

FORMS += \
    color_pick.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
