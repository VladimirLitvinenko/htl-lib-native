APP_NAME = test_Extract_Tile

include($$PWD/../../../examples_body.pri)

SOURCES += *.cpp \
  ../../../../../mapbox-gl-native/platform/default/src/mbgl/util/compression.cpp

OTHER_FILES += \
    $$files($$PWD/rpm/*)
