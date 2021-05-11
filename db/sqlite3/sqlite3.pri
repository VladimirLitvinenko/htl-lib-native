include($$PWD/../../htl_common.pri)
include($$PWD/../../common/common.pri)

SQLITE3_HEADERS   = $$files($${INCLUDE_SQLITE3_PATH}/*.h)

SQLITE3_MODERN_HEADERS   = \
    $$files($${INCLUDE_SQLITE3_PATH}/sqlite_modern_cpp/*.h)

SQLITE3_MODERN_LISTS_HEADERS   = \
    $$files($${INCLUDE_SQLITE3_PATH}/sqlite_modern_cpp/lists/*.h)

SQLITE3_MODERN_UTIL_HEADERS   = \
    $$files($${INCLUDE_SQLITE3_PATH}/sqlite_modern_cpp/utility/*.h)

!contains(INCLUDEPATH,$${INCLUDE_SQLITE3_PATH}) : INCLUDEPATH += $${INCLUDE_SQLITE3_PATH}

#QMAKE_CLEAN += \
#  $${target_sqlite3_include.path}/*.h \
#  $${target_sqlite3_m_include.path}/*.h \
#  $${target_sqlite3_ml_include.path}/*.h \
#  $${target_sqlite3_ul_include.path}/*.h \


