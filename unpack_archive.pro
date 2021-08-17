QT       += gui core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    unpackerthread.cpp

HEADERS += \
    mainwindow.h \
    unpackerthread.h

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/build-quazip-Desktop_Qt_5_15_2_MinGW_32_bit-Release/quazip/ -llibquazip1-qt5.dll

INCLUDEPATH += $$PWD/quazip/quazip
DEPENDPATH += $$PWD/quazip/quazip

DISTFILES += \
    quazip/quazip/quazip.pc.cmakein
