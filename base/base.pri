!contains(DEFINES,COMMON_LIBRARY) : DEFINES += COMMON_LIBRARY

include($$PWD/../common/common.pri)
include($$PWD/application/application.pri)
include($$PWD/asio/asio.pri)
include($$PWD/http/http.pri)
include($$PWD/watchers/watchers.pri)

LIB_BASE_NAME = htl.base

#QMAKE_CLEAN += $${BUILD_LIB_PATH}/lib$${LIB_BASE_NAME}$${PROJECT_SUFFIX}*

