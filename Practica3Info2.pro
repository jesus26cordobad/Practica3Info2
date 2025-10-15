TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        binaryOperations.cpp \
        encodingMethods.cpp \
        fileOperations.cpp \
        main.cpp \
        utilitiesMain.cpp \
        validations.cpp

HEADERS += \
    binaryOperations.h \
    encodingMethods.h \
    fileOperations.h \
    utilitiesMain.h \
    validations.h
