include($$PWD/../qt/qt.pri)

LIB_SERVER_NAME = htl.server

LIBS += -lpthread

!contains(INCLUDEPATH,$${INCLUDE_SERVER_PATH}) : INCLUDEPATH += $${INCLUDE_SERVER_PATH}

SERVER_HEADERS = $$files($${INCLUDE_SERVER_PATH}/*.h)
SERVER_SOURCES = $$files($$PWD/src/*.cpp)
