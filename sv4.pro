# -------------------------------------------------
# Project created by QtCreator 2010-05-14T15:49:46
# -------------------------------------------------
QT += sql
TARGET = tobacco
TEMPLATE = app
SOURCES += main.cpp \
    userdialog.cpp \
    tobacco.cpp \
    timelabel.cpp \
    pwddialog.cpp \
    mystatus.cpp \
    mysqltableview.cpp \
    myplot.cpp \
    mypb.cpp \
    mylineedit.cpp \
    mylabel.cpp \
    myknob.cpp \
    modifydialog.cpp \
    kbdialog.cpp \
    index.cpp \
    digitdialog.cpp \
    clocklabel.cpp
HEADERS += index.h \
    userdialog.h \
    tobacco.h \
    timelabel.h \
    pwddialog.h \
    mystatus.h \
    mysqltableview.h \
    mypushbutton.h \
    myplot.h \
    mypb.h \
    mylineedit.h \
    mylabel.h \
    myknob.h \
    modifydialog.h \
    kbdialog.h \
    digitdialog.h \
    clocklabel.h
FORMS += index.ui \
    userdialog.ui \
    pwddialog.ui \
    modifydialog.ui \
    kbdialog.ui \
    digitdialog.ui
RESOURCES += source.qrc
OTHER_FILES += sources/tobacco.qss
OBJECTS_DIR = tmp
MOC_DIR = tmp
UI_DIR = tmp
RCC_DIR = tmp
DESTDIR = build
LIBS += -lqwt
INCLUDEPATH += /usr/local/qtsdk-2010.02/qt/include/Qwt
#INCLUDEPATH += /usr/local/qte-x86/include/Qwt
