# Proyecto de tests unitarios (QTest) — Fase 4.4
# Compila las suites en un único binario "tests" enlazando solo lo necesario.

QT       += testlib core gui widgets network sql
CONFIG   += console c++17
CONFIG   -= app_bundle

TEMPLATE = app
TARGET   = tests

INCLUDEPATH += ..

SOURCES += \
    main.cpp \
    test_hashutil.cpp \
    test_verifactu.cpp \
    test_formateo.cpp \
    ../verifactuclass.cpp \
    ../formateo.cpp

HEADERS += \
    test_hashutil.h \
    test_verifactu.h \
    test_formateo.h \
    ../verifactuclass.h \
    ../hashutil.h \
    ../formateo.h

# libqrencode se usa en verifactuClass::generarCodigoQR
LIBS += -lqrencode
