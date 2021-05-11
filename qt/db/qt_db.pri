include($$PWD/../../base/base.pri)

!contains (QT, core): QT += core

QT_DB_HEADERS = $$files($${INCLUDE_QT_DB_PATH}/*.h)
QT_DB_SOURCES = $$files($$PWD/src/*.cpp)

INCLUDEPATH += $${INCLUDE_QT_DB_PATH}

OTHER_FILES += \
    $$PWD/README.md


#QMAKE_CLEAN += \
#  $${target_qt_app_include.path}/*.h \

