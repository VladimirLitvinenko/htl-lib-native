include($$PWD/../db/db.pri)

LIB_MBTILES_NAME = htl.mbtiles

MBTILES_HEADERS   = $$files($${INCLUDE_MBTILES_PATH}/*.h)
MBTILES_SOURCES   = $$files($$PWD/src/*.cpp)

!contains(INCLUDEPATH,$${INCLUDE_MBTILES_PATH}): INCLUDEPATH += $${INCLUDE_MBTILES_PATH}

#QMAKE_CLEAN += \
#  $${target_mbtiles_include.path}/*.h \
##  $${target_mbtiles_conf.path}/*.h \

