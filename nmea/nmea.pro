include($$PWD/nmea.pri)

TEMPLATE = lib
#CONFIG += staticlib
TARGET = $${LIB_NMEA_NAME}$${PROJECT_SUFFIX}

OTHER_FILES += \
  $$PWD/README.md \
  $$files($$PWD/rpm/*)

SOURCES += $$NMEA_SOURCES
HEADERS += $$NMEA_HEADERS

target_nmea_include.path  = $${BUILD_INCLUDE_NMEA_PATH}
target_nmea_include.files = $${NMEA_HEADERS}

target_nmea_lib.path  = $${INSTALL_LIB_PATH}
target_nmea_lib.files = $${NMEA_LIB}

#!isEmpty(target_envoy_include.path):
INSTALLS += target_nmea_include
INSTALLS += target_nmea_lib

target.path = $${INSTALL_LIB_PATH}
INSTALLS += target
