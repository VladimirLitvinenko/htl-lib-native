include($$PWD/../../base/base.pri)

!contains (QT, core): QT += core
#!contains (QT, gui) : QT += gui
!contains (QT, widgets) : greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT_APP_HEADERS = $$files($${INCLUDE_QT_APP_PATH}/*.h)

INCLUDEPATH += $${INCLUDE_QT_APP_PATH}

OTHER_FILES += \
    $$PWD/README.md


#QMAKE_CLEAN += \
#  $${target_qt_app_include.path}/*.h \

