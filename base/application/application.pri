include($$PWD/../documents/documents.pri)

APP_HEADERS = $$files($${INCLUDE_APP_PATH}/*.h)
APP_SOURCES = $$files($$PWD/src/*.cpp)

!contains(INCLUDEPATH,$${INCLUDE_APP_PATH}): INCLUDEPATH += $${INCLUDE_APP_PATH}

OTHER_FILES += \
  $$PWD/README.md

!contains(QT, core): QT += core

#QMAKE_CLEAN += \
#  $${target_app_include.path}/*.h \

