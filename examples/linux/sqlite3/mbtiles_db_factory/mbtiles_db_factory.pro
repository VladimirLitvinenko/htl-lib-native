APP_NAME = mbtiles_db_factory

include($$PWD/../../../examples_body.pri)

SOURCES += *.cpp

OTHER_FILES += \
    $$files($$PWD/rpm/*)
