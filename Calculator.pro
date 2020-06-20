#-------------------------------------------------
#
# Project created by QtCreator 2019-04-05T15:26:16
#
#-------------------------------------------------
RC_ICONS=Calculator.ico


greaterThan(QT_MAJOR_VERSION, 4):
QT += widgets
QT += core gui

TARGET = Calculator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
	MainWindow.cpp \
	Settings.cpp \
	TextBrowser.cpp \
	Timer_StatusBar.cpp \
        main.cpp \
    Window_Settings.cpp \
    Text.cpp \
    Window_Info.cpp

HEADERS += \
    MainWindow.h \
    Settings.h \
    TextBrowser.h \
    Timer_StatusBar.h \
    Window_Settings.h \
    Text.h \
    Window_Info.h

FORMS += \
        mainwindow.ui

RESOURCES += \
	resource.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


