include($$PWD/db.pri)
#-------------------------------------------------
#
# Project created by QtCreator 2020-04-21T16:24:33
#
#-------------------------------------------------

TEMPLATE = lib
#CONFIG += staticlib
TARGET = $${LIB_DB_NAME}$${PROJECT_SUFFIX}

OTHER_FILES += \
  $$PWD/README.md \

SOURCES += $${LIB_DB_SOURCES}
HEADERS += $${LIB_DB_HEADERS}

target_db_include.path  = $${BUILD_INCLUDE_DB_PATH}
target_db_include.files = $${LIB_DB_HEADERS}

HEADERS += \
  $${SQLITE3_HEADERS} \
  $${SQLITE3_MODERN_HEADERS} \
  $${SQLITE3_MODERN_LISTS_HEADERS} \
  $${SQLITE3_MODERN_UTIL_HEADERS}

#LIBS += -L$${OUT_PWD}/../spdlog -l$${LIB_SPLOG_NAME}$${PROJECT_SUFFIX}
##LIBS += -L$${OUT_PWD}/../nmea -l$${LIB_NMEA_NAME}$${PROJECT_SUFFIX}
#LIBS += -L$${OUT_PWD}/../common -l$${LIB_COMMON_NAME}$${PROJECT_SUFFIX}
#LIBS += -L$${OUT_PWD}/../base -l$${LIB_BASE_NAME}$${PROJECT_SUFFIX}
LIBS += \
    -L$${INSTALL_LIB_PATH} \
    -l$${LIB_SPLOG_NAME}$${PROJECT_SUFFIX} \
    -l$${LIB_COMMON_NAME}$${PROJECT_SUFFIX} \
    -l$${LIB_BASE_NAME}$${PROJECT_SUFFIX}

target_sqlite3_include.path  = $${BUILD_INCLUDE_SQLITE3_PATH}
target_sqlite3_include.files = $${SQLITE3_HEADERS}

target_sqlite3_m_include.path  = $${BUILD_INCLUDE_SQLITE3_PATH}/sqlite_modern_cpp
target_sqlite3_m_include.files = $${SQLITE3_MODERN_HEADERS}

target_sqlite3_ml_include.path  = $${BUILD_INCLUDE_SQLITE3_PATH}/sqlite_modern_cpp/lists
target_sqlite3_ml_include.files = $${SQLITE3_MODERN_LISTS_HEADERS}

target_sqlite3_ul_include.path  = $${BUILD_INCLUDE_SQLITE3_PATH}/sqlite_modern_cpp/utility
target_sqlite3_ul_include.files = $${SQLITE3_MODERN_UTIL_HEADERS}

!contains(LIBS,sqlite3) : LIBS += -L/usr/lib -lsqlite3
#!contains(LIBS,libjsoncpp_static) : LIBS += -L/usr/lib/libjsoncpp_static.a
!contains(LIBS,jsoncpp) : LIBS += -L/usr/lib -ljsoncpp

target.path = $${INSTALL_LIB_PATH}

INSTALLS += \
  target_db_include \
  target_sqlite3_include \
  target_sqlite3_m_include \
  target_sqlite3_ml_include \
  target_sqlite3_ul_include \
  target \

