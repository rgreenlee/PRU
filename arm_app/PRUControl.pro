QT += quick qml
CONFIG += c++17 console
CONFIG -= app_bundle

SOURCES += \
    main.cpp \
    prucontroller.cpp

HEADERS += \
    prucontroller.h \
    ../firmware/pru_shared.h

RESOURCES += qml.qrc