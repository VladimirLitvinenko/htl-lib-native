include($$PWD/spdlog.pri)

TEMPLATE = lib
TARGET = $${LIB_SPLOG_NAME}$${PROJECT_SUFFIX}

OTHER_FILES += \
  $$PWD/README.md \

!contains(HEADERS,$${SPDLOG_HEADERS}): HEADERS += $${SPDLOG_HEADERS}
!contains(SOURCES,$${SPDLOG_SOURCES}): SOURCES += $${SPDLOG_SOURCES}

target.path = $${INSTALL_LIB_PATH}

INSTALLS += target

target_spdlog_include.path  = $${BUILD_INCLUDE_SPDLOG_PATH}
target_spdlog_include.files = $${INCLUDE_SPDLOG_PATH}/*.h

target_spdlog_cfg_include.path  = $${BUILD_INCLUDE_SPDLOG_PATH}/cfg
target_spdlog_cfg_include.files = $${INCLUDE_SPDLOG_PATH}/cfg/*.h

target_spdlog_details_include.path  = $${BUILD_INCLUDE_SPDLOG_PATH}/details
target_spdlog_details_include.files = $${INCLUDE_SPDLOG_PATH}/details/*.h

target_spdlog_fmt_include.path  = $${BUILD_INCLUDE_SPDLOG_PATH}/fmt
target_spdlog_fmt_include.files = $${INCLUDE_SPDLOG_PATH}/fmt/*.h
target_spdlog_fmtb_include.path  = $${BUILD_INCLUDE_SPDLOG_PATH}/fmt/bundled
target_spdlog_fmtb_include.files = $${INCLUDE_SPDLOG_PATH}/fmt/bundled/*.h

target_spdlog_sinks_include.path  = $${BUILD_INCLUDE_SPDLOG_PATH}/sinks
target_spdlog_sinks_include.files = $${INCLUDE_SPDLOG_PATH}/sinks/*.h

INSTALLS += \
  target_spdlog_include \
  target_spdlog_cfg_include \
  target_spdlog_details_include \
  target_spdlog_fmt_include \
  target_spdlog_fmtb_include \
  target_spdlog_sinks_include \

#QMAKE_CLEAN += \
#    $${target_spdlog_include.path}/*.h \
#    $${target_spdlog_cfg_include.path}/*.h \
#    $${target_spdlog_details_include.path}/*.h \
#    $${target_spdlog_fmt_include.path}/*.h \
#    $${target_spdlog_fmtb_include.path}/*.h \
#    $${target_spdlog_sinks_include.path}/*.h \
