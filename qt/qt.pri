include($$PWD/../mbtiles/mbtiles.pri)

!contains(DEFINES,COMMON_LIBRARY) : DEFINES += COMMON_LIBRARY
!contains(QT,core): QT += core
greaterThan(QT_MAJOR_VERSION, 4): !contains(QT,widgets): QT += widgets

LIB_QT_NAME = htl.qt

include($$PWD/application/qt_application.pri)
include($$PWD/db/qt_db.pri)

QT_HEADERS = $$files($${INCLUDE_QT_PATH}/*.h)
QT_SOURCES = $$files($$PWD/src/*.cpp)

!contains(INCLUDEPATH,$${INCLUDE_QT_PATH}) : INCLUDEPATH += $${INCLUDE_QT_PATH}

#QMAKE_CLEAN += \
#  $${target_qt_include.path}/*.h \

#QMAKE_CLEAN += \
#  $${BUILD_LIB_PATH}/lib$${LIB_QT_NAME}$${PROJECT_SUFFIX}* \
