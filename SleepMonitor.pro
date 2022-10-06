QT       += core gui
QT       +=serialport
QT       +=charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
ICON
SOURCES += \
    ContralDataWidget.cpp \
    ControlWnd.cpp \
    commsetwidget.cpp \
    controldata.cpp \
    dynamicplot.cpp \
    eegwnd.cpp \
    filepath.cpp \
    helpers.cpp \
    magnifywindow.cpp \
    main.cpp \
    myenumpropertymanager.cpp \
    mytracer.cpp \
    outputwnd.cpp \
    plotwidget.cpp \
    qcustomplot.cpp \
    sensordata.cpp \
    sleepinterventionwidget.cpp \
    sleepmonitor.cpp \
    test.cpp

HEADERS += \
    ContralDataWidget.h \
    ControlWnd.h \
    commsetwidget.h \
    controldata.h \
    dynamicplot.h \
    eegwnd.h \
    filepath.h \
    helpers.h \
    magnifywindow.h \
    myenumpropertymanager.h \
    mytracer.h \
    outputwnd.h \
    plotwidget.h \
    qcustomplot.h \
    sensordata.h \
    sleepinterventionwidget.h \
    sleepmonitor.h \
    struct_data.h \
    test.h

FORMS += \
    ControlWnd.ui \
    commsetwidget.ui \
    dynamicplot.ui \
    eegwnd.ui \
    filepath.ui \
    magnifywindow.ui \
    outputwnd.ui \
    plotwidget.ui \
    sleepinterventionwidget.ui \
    sleepmonitor.ui \
    test.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include($$[QT_INSTALL_PREFIX]/../Src/qttools/src/shared/qtpropertybrowser/qtpropertybrowser.pri)

RESOURCES += \
    res.qrc \
    resources.qrc
RC_ICONS=icon/sleepmonitor.ico
