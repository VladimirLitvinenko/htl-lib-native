APP_NAME = boost_optional

include($$PWD/../../../examples_body.pri)

SOURCES += *.cpp

OTHER_FILES += \
    $$files($$PWD/rpm/*)
