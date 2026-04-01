-- Script de Migración para añadir el campo "usuario" a la tabla "entradasSalidas"
-- Ejecutar en la base de datos de cada tienda o donde se guarden los movimientos.

ALTER TABLE entradasSalidas 
ADD COLUMN usuario VARCHAR(45) DEFAULT 'Sin Usuario';
