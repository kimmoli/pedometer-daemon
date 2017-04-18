TARGET = stpcntrd

QT += dbus network
QT -= gui

CONFIG += link_pkgconfig
PKGCONFIG += sensord-qt5

# DBus
system(qdbusxml2cpp config/com.kimmoli.$${TARGET}.xml -i src/stepcounter.h -a src/adaptor)

target.path = /usr/bin/

dbusService.files = config/com.kimmoli.$${TARGET}.service
dbusService.path = /usr/share/dbus-1/services/

dbusInterface.files = config/com.kimmoli.$${TARGET}.xml
dbusInterface.path = /usr/share/dbus-1/interfaces/

systemduser.files = config/$${TARGET}.service
systemduser.path = /etc/systemd/user/

sensorfwconf.files = config/90-stepcounter.conf
sensorfwconf.path = /etc/sensorfw/sensord.conf.d/

INSTALLS += target dbusService dbusInterface systemduser sensorfwconf

DEFINES += "APPVERSION=\\\"$${SPECVERSION}\\\""

SOURCES += \
    src/$${TARGET}.cpp \
    src/adaptor.cpp \
    src/stepcounter.cpp

OTHER_FILES += \
    rpm/$${TARGET}.spec \
    config/com.kimmoli.$${TARGET}.service \
    config/com.kimmoli.$${TARGET}.xml \
    config/$${TARGET}.service \
    config/90-stepcounter.conf

HEADERS += \
    src/adaptor.h \
    src/stepcounter.h
