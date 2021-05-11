TARGET = $${APP_NAME}$${FILE_SUFFIX}

include($$PWD/../libs.pri)

QT += core
#QT -= gui
QT += network

CONFIG += console
#CONFIG += link_pkgconfig
CONFIG -= app_bundle

TEMPLATE = app

target.path=$${INSTALL_BIN_PATH}/$${TARGET}
INSTALLS += target

#QMAKE_DISTCLEAN += \
#    $${target.path}/$${TARGET}


