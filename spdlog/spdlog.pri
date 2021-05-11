include($$PWD/../htl_common.pri)

LIB_SPLOG_NAME = spdlog

SPDLOG_HEADERS   = \
  $$files($${INCLUDE_SPDLOG_PATH}/*.h) \
  $$files($${INCLUDE_SPDLOG_PATH}/cfg/*.h) \
  $$files($${INCLUDE_SPDLOG_PATH}/details/*.h) \
  $$files($${INCLUDE_SPDLOG_PATH}/fmt/*.h) \
  $$files($${INCLUDE_SPDLOG_PATH}/fmt/bundled/*.h) \
  $$files($${INCLUDE_SPDLOG_PATH}/sinks/*.h) \

SPDLOG_SOURCES = $$files($$PWD/*.cpp)

!contains(INCLUDEPATH,$${INCLUDE_SPDLOG_PATH}):INCLUDEPATH += $${INCLUDE_SPDLOG_PATH}

#contains(ANDROID_TARGET_ARCH,x86) {
#    ANDROID_EXTRA_LIBS += \
#    -lboost_context \
#    -lboost_thread \
#    -lboost_date_time \
#    -lboost_system \
#    -lboost_regex \
#    -lboost_filesystem \
#    -lboost_coroutine \
#        ..........
#} else {
!contains(LIBS,-lboost_date_time) : LIBS += -lboost_date_time
!contains(LIBS,-lboost_system) : LIBS += -lboost_system
!contains(LIBS,-lboost_regex) : LIBS += -lboost_regex
!contains(LIBS,-lboost_filesystem) : LIBS += -lboost_filesystem
!contains(LIBS,-lboost_container) : LIBS += -lboost_container

