include($$PWD/../documents/documents.pri)

!contains(QT,network)): QT += network

HTTP_HEADERS = $$files($${INCLUDE_HTTP_PATH}/*.h)
HTTP_SOURCES = $$files($$PWD/src/*.cpp)

!contains(INCLUDEPATH,$${INCLUDE_HTTP_PATH}) : INCLUDEPATH += $${INCLUDE_HTTP_PATH}

OTHER_FILES += \
  $$PWD/README.md

#QMAKE_CLEAN += \
#  $${target_http_include}/*.h \
