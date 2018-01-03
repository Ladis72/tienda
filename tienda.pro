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
    clientes.cpp \
    proveedores.cpp \
    gestionpedidos.cpp \
    pedidonuevo.cpp \
    buscarproveedor.cpp \
    pedidos.cpp \
    cajas.cpp \
    tiposentradassalidas.cpp \
    entradasalida.cpp \
    aceptarpedido.cpp \
    configuracion.cpp \
    cambiodatosproducto.cpp \
    historicotickets.cpp \
    verfacturas.cpp \
    listadoventas.cpp \
    dialogfecha.cpp \
    listadoventaarticulos.cpp \
    stock.cpp \
    entradamercancia.cpp \
    caducidades.cpp \
    salidas.cpp \
    etiquetas.cpp \
    formateo.cpp \
    imprimirticket.cpp \
    caducados.cpp \
    imprimirfactura.cpp \
    configticket.cpp \
    configbase.cpp \
    visorimagenes.cpp \
    visorfacturas.cpp \
    prestamos.cpp \
    dateeditdelegate.cpp \
    configuracionotros.cpp

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
    clientes.h \
    proveedores.h \
    gestionpedidos.h \
    pedidonuevo.h \
    buscarproveedor.h \
    pedidos.h \
    cajas.h \
    tiposentradassalidas.h \
    entradasalida.h \
    aceptarpedido.h \
    configuracion.h \
    cambiodatosproducto.h \
    historicotickets.h \
    verfacturas.h \
    listadoventas.h \
    dialogfecha.h \
    listadoventaarticulos.h \
    stock.h \
    entradamercancia.h \
    caducidades.h \
    salidas.h \
    etiquetas.h \
    formateo.h \
    imprimirticket.h \
    caducados.h \
    imprimirfactura.h \
    configticket.h \
    configbase.h \
    visorimagenes.h \
    visorfacturas.h \
    prestamos.h \
    dateeditdelegate.h \
    configuracionotros.h

FORMS    += tienda.ui \
    tpv.ui \
    buscarproducto.ui \
    tabwidget.ui \
    articulos.ui \
    familias.ui \
    fabricantes.ui \
    totalizar.ui \
    formaspago.ui \
    clientes.ui \
    proveedores.ui \
    gestionpedidos.ui \
    pedidonuevo.ui \
    buscarproveedor.ui \
    pedidos.ui \
    cajas.ui \
    tiposentradassalidas.ui \
    entradasalida.ui \
    aceptarpedido.ui \
    cambiodatosproducto.ui \
    historicotickets.ui \
    verfacturas.ui \
    listadoventas.ui \
    dialogfecha.ui \
    listadoventaarticulos.ui \
    stock.ui \
    entradamercancia.ui \
    caducidades.ui \
    salidas.ui \
    etiquetas.ui \
    caducados.ui \
    configticket.ui \
    configbase.ui \
    visorimagenes.ui \
    visorfacturas.ui \
    prestamos.ui \
    configuracionotros.ui
include($$PWD/QtRptProject/QtRPT/QtRPT.pri)

RESOURCES += \
    recursos.qrc
