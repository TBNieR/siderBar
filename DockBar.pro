QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
RC_ICONS = logo.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controller.cpp \
    dropfilepath.cpp \
    iconwidget.cpp \
    main.cpp \
    mylineedit.cpp \
    netinfo.cpp \
    openfilebtn.cpp \
    weather.cpp \
    widget.cpp

HEADERS += \
    controller.h \
    dropfilepath.h \
    iconwidget.h \
    mylineedit.h \
    netinfo.h \
    openfilebtn.h \
    weather.h \
    widget.h \
    worker.h

FORMS += \
    weather.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
uinx|win32:LIBS+=-liphlpapi

