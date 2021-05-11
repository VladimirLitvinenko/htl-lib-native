include($$PWD/../htl_common.pri)
include($$PWD/../spdlog/spdlog.pri)

TEMPLATE = lib
LIB_COMMON_NAME = htl.common

COMMON_HEADERS   = $$files($${INCLUDE_COMMON_PATH}/*.h )
COMMON_SOURCES   = $$files($${PWD}/src/*.cpp)

!contains(INCLUDEPATH,$${INCLUDE_COMMON_PATH}) : INCLUDEPATH += $${INCLUDE_COMMON_PATH}

#QMAKE_CLEAN += \
#  $${target_common_include.path}/*.h \


