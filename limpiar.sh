#!/bin/bash
# Script para limpiar archivos de construcción en la raíz
echo "🧹 Limpiando archivos de objetos y temporales de la raíz..."
rm -f *.o moc_* ui_* qrc_* Makefile* .qmake.stash tienda
echo "✅ Raíz del proyecto limpia."
