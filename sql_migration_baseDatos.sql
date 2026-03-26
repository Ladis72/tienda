-- Script de Migración para añadir el campo "baseDatos" a la tabla "tiendas"
-- Ejecutar en la base de datos principal de la aplicación.

ALTER TABLE tiendas 
ADD COLUMN baseDatos VARCHAR(255) DEFAULT 'tiendaNueva';

-- Migración: Añadir campo "imagenPromo" a la tabla "configTicket"
-- Almacena la ruta de la imagen promocional que se imprime al final del ticket
-- cuando el checkbox "Imprimir Ticket promocional" está activado.
ALTER TABLE configTicket
ADD COLUMN imagenPromo VARCHAR(500) DEFAULT '';
