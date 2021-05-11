include($$PWD/spdlog/spdlog.pri)
include($$PWD/nmea/nmea.pri)
include($$PWD/common/common.pri)
include($$PWD/base/base.pri)
include($$PWD/db/db.pri)
include($$PWD/mbtiles/mbtiles.pri)
include($$PWD/qt/qt.pri)
#include($$PWD/mapbox/mapbox.pri)
include($$PWD/server/server.pri)

DEPENDPATH += $$INSTALL_LIB_PATH

!contains(LIBS,-lrt) : LIBS += -lrt
!contains(LIBS,-lpthread) : LIBS += -lpthread
!contains(LIBS,-lz) : LIBS += -lz
#!contains(LIBS,-lcurl) : LIBS += -lcurl
!contains(LIBS,-lsqlite3) : LIBS += -lsqlite3
!contains(LIBS,-ljsoncpp) : LIBS += -ljsoncpp
#LIBS += -lcurl

QUAZIP_VER = 5
!contains(DEFINES,"IS_SAILFISH") {
  QUAZIP_VER = 5
}
!contains(LIBS,-linotifytools) : LIBS += -linotifytools
#!contains(LIBS,-lqmapboxgl) : LIBS += -L$${MAPBOX_BUILD_PATH} -lqmapboxgl

!contains(LIBS,-lboost_date_time) : LIBS += -lboost_date_time
!contains(LIBS,-lboost_system) : LIBS += -lboost_system
!contains(LIBS,-lboost_regex) : LIBS += -lboost_regex
!contains(LIBS,-lboost_filesystem) : LIBS += -lboost_filesystem
!contains(LIBS,-lboost_container) : LIBS += -lboost_container

!contains(LIBS,-L$${INSTALL_LIB_PATH}) : LIBS += -L$${INSTALL_LIB_PATH}
!contains(LIBS,-l$${LIB_SPLOG_NAME}$${PROJECT_SUFFIX}) : LIBS += -l$${LIB_SPLOG_NAME}$${PROJECT_SUFFIX}
!contains(LIBS,-l$${LIB_NMEA_NAME}$${PROJECT_SUFFIX}) : LIBS += -l$${LIB_NMEA_NAME}$${PROJECT_SUFFIX}
!contains(LIBS,-l$${LIB_COMMON_NAME}$${PROJECT_SUFFIX}) : LIBS += -l$${LIB_COMMON_NAME}$${PROJECT_SUFFIX}
!contains(LIBS,-l$${LIB_BASE_NAME}$${PROJECT_SUFFIX}) : LIBS += -l$${LIB_BASE_NAME}$${PROJECT_SUFFIX}
!contains(LIBS,-l$${LIB_DB_NAME}$${PROJECT_SUFFIX}) : LIBS += -l$${LIB_DB_NAME}$${PROJECT_SUFFIX}
!contains(LIBS,-l$${LIB_MBTILES_NAME}$${PROJECT_SUFFIX}) : LIBS += -l$${LIB_MBTILES_NAME}$${PROJECT_SUFFIX}
!contains(LIBS,-l$${LIB_QT_NAME}$${PROJECT_SUFFIX}) : LIBS += -l$${LIB_QT_NAME}$${PROJECT_SUFFIX}
#!contains(LIBS,-l$${LIB_ENVOY_NAME}$${PROJECT_SUFFIX}) : LIBS += -l$${LIB_ENVOY_NAME}$${PROJECT_SUFFIX}
#!contains(LIBS,-l$${LIB_MBOX_NAME}$${PROJECT_SUFFIX}) : LIBS += -l$${LIB_MBOX_NAME}$${PROJECT_SUFFIX}
!contains(LIBS,-l$${LIB_SERVER_NAME}$${PROJECT_SUFFIX}) : LIBS += -l$${LIB_SERVER_NAME}$${PROJECT_SUFFIX}

#isEmpty(ICM_COMPLETE) : {
#  ICM_COMPLETE=$$system('sudo sysctl -w net.ipv4.ping_group_range="0 1007"')
#}



