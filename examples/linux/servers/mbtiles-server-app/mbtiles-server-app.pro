APP_NAME = mbtiles-server-app

include($$PWD/../../../examples_body.pri)
SOURCES += $$files(*.cpp)

OTHER_FILES += \
    $$PWD/rpm/*

