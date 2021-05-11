include($$PWD/../base/base.pri)

!contains(DEFINES,COMMON_LIBRARY) : DEFINES += COMMON_LIBRARY

#!contains(QT,sql)     : QT += sql
#!contains(QT,xml)     : QT += xml

LIB_DB_NAME = htl.db

LIB_DB_HEADERS = $$files($${INCLUDE_DB_PATH}/*.h)
LIB_DB_SOURCES = \
  $${SQLITE3_SOURCES} \
  $$files($${PWD}/src/*.cpp)

!contains(INCLUDEPATH,$${INCLUDE_DB_PATH}) : INCLUDEPATH += $${INCLUDE_DB_PATH}

include($$PWD/sqlite3/sqlite3.pri)

##QMAKE_CLEAN += \
##  $${target_db_include.path}/*.h \

#QMAKE_CLEAN += \
#  $${BUILD_LIB_PATH}/lib$${LIB_DB_NAME}$${PROJECT_SUFFIX}* \
