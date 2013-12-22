#-------------------------------------------------
#
# Project created by QtCreator 2012-07-08T14:01:40
#
#-------------------------------------------------



QT       += core gui network script

TARGET = ipangui
#CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
TRANSLATIONS = cn_zh.ts
LIBS += -L./
SOURCES += main.cpp \
    ipangui.cpp \
    IOauth.cpp \
    login.cpp \
    preferences.cpp \
    inotifythread.cpp \
    uploadthread.cpp \
    filetree.cpp \
    treedialog.cpp \
    filedownuplist.cpp

HEADERS += \
    ipangui.h \
    IOauth.h \
    login.h \
    preferences.h \
    public.h \
    inotifythread.h \
    uploadthread.h \
    filetree.h \
    treedialog.h \
    filedownuplist.h

FORMS += \
    ui/preferences.ui \
    ui/login.ui \
    ui/treedialog.ui \
    ui/filelist.ui
