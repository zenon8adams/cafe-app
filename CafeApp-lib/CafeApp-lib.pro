#-------------------------------------------------
#
# Project created by QtCreator 2019-12-18T23:53:12
#
#-------------------------------------------------

QT       += qml sql

TARGET = CafeApp-lib
TEMPLATE = lib
CONFIG += c++1z
DEFINES += CAFEAPPLIB_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
            model/table-model.cpp \
            model/menu-list-model.cpp \
            controller/master-controller.cpp \
            controller/staff-view-controller.cpp \
            controller/master-view-controller.cpp \
            controller/login-form-controller.cpp \
            controller/signup-form-controller.cpp \
            controller/db-controller.cpp \
            controller/menu-form-controller.cpp \
            controller/tooltip-description-controller.cpp \
            framework/error-handler.cpp \
            utility/rsa-encryption.cpp


HEADERS += \
        cafeapp-lib_global.h  \
        model/table-model.h \
        model/menu-list-model.h \
        controller/master-controller.h \
        controller/staff-view-controller.h \
        controller/master-view-controller.h \
        controller/login-form-controller.h \
        controller/signup-form-controller.h \
        controller/idb-controller.h \
        controller/db-controller.h \
    controller/menu-form-controller.h \
    controller/tooltip-description-controller.h \
    framework/error-handler.h \
    utility/rsa-encryption.h
    ../CafeApp-test/ext-debug.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}
