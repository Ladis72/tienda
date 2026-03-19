-- Script de Migración para añadir el campo "baseDatos" a la tabla "tiendas"
-- Ejecutar en la base de datos principal de la aplicación.

ALTER TABLE tiendas 
ADD COLUMN baseDatos VARCHAR(255) DEFAULT 'tiendaNueva';
