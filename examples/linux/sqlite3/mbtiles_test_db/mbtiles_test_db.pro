APP_NAME = mbtile_test_db

include($$PWD/../../../examples_body.pri)

SOURCES += *.cpp

OTHER_FILES += \
    $$files($$PWD/rpm/*)
