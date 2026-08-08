#-------------------------------------------------
#
# Project created by QtCreator 2013-04-23T15:31:17
#Ladis
#-------------------------------------------------



QT       += sql charts webenginewidgets concurrent network
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS     += -lqrencode

CONFIG -= lto ltcg

TARGET = tienda
TEMPLATE = app


SOURCES += main.cpp\
    ClickableLabel.cpp \
    conexionesremotas.cpp \
    generarvales.cpp \
    graficoventaswidget.cpp \
    gestorpermisos.cpp \
    imprimirfacturaproveedor.cpp \
    imprimirpedido.cpp \
    impuestos.cpp \
    informes1.cpp \
    login.cpp \
    printermanager.cpp \
    tienda.cpp \
    tpv.cpp \
    base_datos.cpp \
    buscarproducto.cpp \
    buscarpornotas.cpp \
    buscarcliente.cpp \
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
    ventasusuariowidget.cpp \
    estadisticas.cpp \
    editorpermisos.cpp \
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
    imprimirfactura.cpp \
    configticket.cpp \
    configbase.cpp \
    configlocal.cpp \
    verifactuclass.cpp \
    visorimagenes.cpp \
    visorfacturas.cpp \
    dateeditdelegate.cpp \
    configuracionotros.cpp \
    verifactudialog.cpp \
    listadosalidas.cpp \
    listadoarqueos.cpp \
    listadocaducados.cpp \
    formatos.cpp \
    directorios.cpp \
    tiendas.cpp \
    comprasventasremoto.cpp \
    facturaralbaranes.cpp \
    notaswidget.cpp \
    encargosdialog.cpp \
    gestorencargosdialog.cpp \
    syncmanager.cpp \
    unificarproveedores.cpp \
    saneadorglobal.cpp \
    unificarmaestros.cpp \
    dialogcomparararticulos.cpp \
    dialoganadirapedido.cpp \
    historialprecios.cpp \
    dialogcambiocodigo.cpp \
    visorlog.cpp \
    dialogdetallearqueo.cpp \
    monitorcaducidades.cpp \
    dialogrecomendaciones.cpp \
    verificadorbasedatos.cpp \
    dialogdetallesverificador.cpp \
    dialogasistenteinstalacion.cpp \
    dialogtrazabilidad.cpp

HEADERS  += tienda.h \
    ClickableLabel.h \
    conexionesremotas.h \
    generarvales.h \
    graficoventaswidget.h \
    gestorpermisos.h \
    imprimirfacturaproveedor.h \
    imprimirpedido.h \
    impuestos.h \
    informes1.h \
    login.h \
    printermanager.h \
    tpv.h \
    base_datos.h \
    conexion.h \
    buscarproducto.h \
    buscarpornotas.h \
    buscarcliente.h \
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
    ventasusuariowidget.h \
    estadisticas.h \
    editorpermisos.h \
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
    imprimirfactura.h \
    configticket.h \
    configbase.h \
    configlocal.h \
    verifactuclass.h \
    visorimagenes.h \
    visorfacturas.h \
    dateeditdelegate.h \
    configuracionotros.h \
    verifactudialog.h \
    listadosalidas.h \
    listadoarqueos.h \
    listadocaducados.h \
    formatos.h \
    directorios.h \
    tiendas.h \
    comprasventasremoto.h \
    facturaralbaranes.h \
    notaswidget.h \
    encargosdialog.h \
    gestorencargosdialog.h \
    syncmanager.h \
    unificarproveedores.h \
    saneadorglobal.h \
    unificarmaestros.h \
    dialogcomparararticulos.h \
    dialoganadirapedido.h \
    historialprecios.h \
    dialogcambiocodigo.h \
    hashutil.h \
    visorlog.h \
    dialogdetallearqueo.h \
    monitorcaducidades.h \
    dialogrecomendaciones.h \
    verificadorbasedatos.h \
    dialogdetallesverificador.h \
    dialogasistenteinstalacion.h \
    dialogtrazabilidad.h

FORMS    += tienda.ui \
    generarvales.ui \
    impuestos.ui \
    login.ui \
    tpv.ui \
    buscarproducto.ui \
    buscarpornotas.ui \
    buscarcliente.ui \
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
    ventasusuariowidget.ui \
    estadisticas.ui \
    editorpermisos.ui \
    verfacturas.ui \
    listadoventas.ui \
    dialogfecha.ui \
    listadoventaarticulos.ui \
    stock.ui \
    entradamercancia.ui \
    caducidades.ui \
    salidas.ui \
    etiquetas.ui \
    configticket.ui \
    configbase.ui \
    configlocal.ui \
    visorimagenes.ui \
    visorfacturas.ui \
    configuracionotros.ui \
    verifactudialog.ui \
    listadosalidas.ui \
    listadoarqueos.ui \
    listadocaducados.ui \
    formatos.ui \
    directorios.ui \
    tiendas.ui \
    comprasventasremoto.ui \
    encargosdialog.ui \
    gestorencargosdialog.ui \
    unificarproveedores.ui \
    saneadorglobal.ui \
    unificarmaestros.ui \
    dialogcomparararticulos.ui \
    dialoganadirapedido.ui \
    historialprecios.ui \
    dialogcambiocodigo.ui \
    dialogdetallearqueo.ui \
    dialogrecomendaciones.ui \
    visorlog.ui \
    verificadorbasedatos.ui \
    dialogdetallesverificador.ui \
    dialogasistenteinstalacion.ui \
    dialogtrazabilidad.ui

#include($$PWD/QtRptProject/QtRPT/QtRPT.pri)

RESOURCES += \
    recursos.qrc

DISTFILES += \
    Instalación.txt \
    canbiosVersiónNueva
