#-------------------------------------------------
#
# Project created by QtCreator 2013-04-23T15:31:17
#
#-------------------------------------------------

QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tienda
TEMPLATE = app


SOURCES += main.cpp\
        tienda.cpp \
    tpv.cpp \
    base_datos.cpp \
    buscarproducto.cpp \
    tabwidget.cpp \
    articulos.cpp \
    familias.cpp \
    fabricantes.cpp \
    totalizar.cpp \
    formaspago.cpp \
    clientes.cpp

HEADERS  += tienda.h \
    tpv.h \
    base_datos.h \
    conexion.h \
    buscarproducto.h \
    tabwidget.h \
    articulos.h \
    familias.h \
    fabricantes.h \
    totalizar.h \
    formaspago.h \
    clientes.h

FORMS    += tienda.ui \
    tpv.ui \
    buscarproducto.ui \
    tabwidget.ui \
    articulos.ui \
    familias.ui \
    fabricantes.ui \
    totalizar.ui \
    formaspago.ui \
    clientes.ui
