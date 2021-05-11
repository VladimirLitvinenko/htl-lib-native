include($$PWD/../htl_common.pri)
#include($$PWD/../spdlog/spdlog.pri)

LIB_NMEA_NAME = nmea

NMEA_SOURCES = $$files($$PWD/src/*.c)
NMEA_HEADERS = $$files($${INCLUDE_NMEA_PATH}/*.h)

!contains(INCLUDEPATH,$${INCLUDE_NMEA_PATH}): INCLUDEPATH += $${INCLUDE_NMEA_PATH}
!contains(DEPENDPATH,$${INCLUDE_NMEA_PATH}): DEPENDPATH  += $${INCLUDE_NMEA_PATH}

