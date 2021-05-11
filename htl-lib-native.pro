

TEMPLATE = subdirs

lib_spdlog.subdir    = $$PWD/spdlog
lib_nmea.subdir      = $$PWD/nmea
lib_common.subdir    = $$PWD/common
lib_base.subdir      = $$PWD/base
lib_db.subdir        = $$PWD/db
lib_mbtiles.subdir   = $$PWD/mbtiles
lib_qt.subdir        = $$PWD/qt
lib_server.subdir    = $$PWD/server
examples.subdir      = $$PWD/examples

SUBDIRS += \
    lib_spdlog \
    lib_nmea \
    lib_common \
    lib_base \
    lib_db \
    lib_qt \
    lib_mbtiles \
    lib_server \
    examples \

lib_spdlog.CONFIG    = recursive
lib_nmea.CONFIG      = recursive
lib_common.CONFIG    = recursive
lib_base.CONFIG       = recursive
lib_db.CONFIG         = recursive
lib_mbtiles.CONFIG    = recursive
lib_qt.CONFIG         = recursive
lib_server.CONFIG     = recursive
examples.CONFIG       = recursive

lib_nmea.depends      = lib_spdlog
lib_common.depends    = lib_spdlog
lib_base.depends      = lib_spdlog lib_common
lib_db.depends        = lib_spdlog lib_common lib_base
lib_mbtiles.depends   = lib_spdlog lib_common lib_base lib_db lib_nmea
lib_qt.depends        = lib_spdlog lib_common lib_base lib_db lib_nmea lib_mbtiles
lib_server.depends    = lib_spdlog lib_common lib_base lib_db lib_nmea lib_mbtiles lib_qt lib_envoy
examples.depends      = lib_spdlog lib_common lib_base lib_db lib_nmea lib_mbtiles lib_qt lib_envoy lib_server

OTHER_FILES += \
  $$PWD/README.md \
  $$PWD/libs.pri \

