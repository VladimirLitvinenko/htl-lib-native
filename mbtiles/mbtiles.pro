include($$PWD/mbtiles.pri)

TEMPLATE = lib
#CONFIG += staticlib
TARGET = $${LIB_MBTILES_NAME}$${PROJECT_SUFFIX}

!contains(HEADERS,$${MBTILES_HEADERS}) : HEADERS += $${MBTILES_HEADERS}
!contains(SOURCES,$${MBTILES_SOURCES}) : SOURCES += $${MBTILES_SOURCES}

!contains(LIBS,-L$${INSTALL_LIB_PATH}) : LIBS += -L$${INSTALL_LIB_PATH}
!contains(LIBS,-l$${LIB_SPLOG_NAME}$${PROJECT_SUFFIX}) : LIBS += -l$${LIB_SPLOG_NAME}$${PROJECT_SUFFIX}
!contains(LIBS,-l$${LIB_COMMON_NAME}$${PROJECT_SUFFIX}) : LIBS += -l$${LIB_COMMON_NAME}$${PROJECT_SUFFIX}
!contains(LIBS,-l$${LIB_BASE_NAME}$${PROJECT_SUFFIX}) : LIBS += -l$${LIB_BASE_NAME}$${PROJECT_SUFFIX}
!contains(LIBS,-l$${LIB_DB_NAME}$${PROJECT_SUFFIX}) : LIBS += -l$${LIB_DB_NAME}$${PROJECT_SUFFIX}

OTHER_FILES += \
  $$PWD/README.md \
  $$files($$PWD/rpm/*) \
  $$files($$PWD/res/*.conf) \
  $$PWD/res/proto.sh

target_mbtiles_include.path  = $${BUILD_INCLUDE_MBTILES_PATH}
target_mbtiles_include.files = $${MBTILES_HEADERS}

target_mbtiles_conf.path  = $${INSTALL_CONF_PATH}
target_mbtiles_conf.files = $$PWD/res/*.conf

target.path = $${INSTALL_LIB_PATH}

INSTALLS += \
  target \
  target_mbtiles_include \
#  target_mbtiles_conf \
