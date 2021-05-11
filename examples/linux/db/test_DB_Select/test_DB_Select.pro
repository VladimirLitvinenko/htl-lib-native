APP_NAME = test_DB_Select

include($$PWD/../../../examples_body.pri)

SOURCES += *.cpp \
  ../../../../../mapbox-gl-native/platform/default/src/mbgl/util/compression.cpp

OTHER_FILES += \
    $$files($$PWD/rpm/*)
