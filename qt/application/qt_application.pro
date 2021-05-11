include($$PWD/application.pri)
include($$PWD/../asio/asio.pri)
include($$PWD/../core/threads/threads.pri)

TARGET = $${APP_LIB_NAME}$${FILE_SUFFIX}
TEMPLATE = lib

SOURCES += \
    $$APP_SOURCES \

HEADERS += \
    $$APP_HEADERS \

core_headers_app.path = $${OUTPUT_INCLUDE_PATH}/application
core_headers_app.files = $$APP_HEADERS
INSTALLS += core_headers_app

target.path = $${OUTPUT_LIBRARY_PATH}
#target.commands += sudo ldconfig

INSTALLS += target

QMAKE_CLEAN += \
  $${core_headers_app.path}/*.h \
  $${OUTPUT_LIBRARY_PATH}/lib$${APP_LIB_NAME}*

################################################################################
#contains(DEFINES,PKG_BUILD) : {

#include($$PWD/../../user_functions.pri)

#PKG_NAME=libPKORI-SPOApp-1$${FILE_SUFFIX}

#DEBIAN_PATH = $$OUT_PWD/$$QT_VERSION
#DESTINITION_PATH=$${OUTPUT_LIBRARY_PATH}
#APP_DESCR_SHORT=SPO PKORI Application Library
#APP_DESCR=SPO PKORI Application Library


#isEmpty(PKG_NAME) : {
#  error('Значение переменной "PKG_NAME" qmake не должно быть пустым.')
#}

#!defined(prepareDebian) {
#  error('Не найдена функция "prepareDebian"!')
#}
#!defined(buildDebianControl) {
# error('Не найдена функция "buildDebianControl"!')
#}
#!defined(buildDebianPackage) {
# error('Не найдена функция "buildDebianPackage"!')
#}
#!defined(buildDebianPostInstRm) {
# error('Не найдена функция "buildDebianPostInstRm"!')
#}

#debian_path.path = $$OUT_PWD
#debian_path.commands = $$prepareDebian( $$DESTINITION_PATH, \
#                                        false, \
#                                        $${APP_DESCR_SHORT}, \
#                                        $${APP_DESCR})

#debian_ctl.path = $$OUT_PWD
#debian_ctl.commands = $$buildDebianControl($${PKG_NAME},\
#                                          "PKORI SPO Application Library",\
#                                          "NPO Strela(SPb) Development Group \x3csupport@strela.ru\x3e",\
#                                          "https://www.strela.ru/")

#debian_post_rm.path = $$OUT_PWD
#debian_post_rm.commands = $$buildDebianPostInstRm($${DESTINITION_PATH})

#deb_build.path = $$OUT_PWD
#deb_build.commands = $$buildDebianPackage($${PKG_NAME})

#INSTALLS += \
#    debian_path \
#    debian_ctl \
#    debian_post_rm \
#    deb_build
#}
#################################################################################
