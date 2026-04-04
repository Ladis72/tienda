-- Script de Migración para añadir el campo "baseDatos" a la tabla "tiendas"
-- Ejecutar en la base de datos principal de la aplicación.

ALTER TABLE tiendas 
ADD COLUMN baseDatos VARCHAR(255) DEFAULT 'tiendaNueva';

ALTER TABLE tiendas 
ADD COLUMN puerto INT DEFAULT 3306;

-- Migración: Añadir campo "ssl_ca" para soporte de conexiones SSL con certificado CA
-- Almacena la ruta local al fichero PEM del Certificate Authority chain.
-- Vacío significa que la conexión no usa SSL.
ALTER TABLE tiendas
ADD COLUMN ssl_ca VARCHAR(500) DEFAULT '';

-- Migración: Añadir campo "imagenPromo" a la tabla "configTicket"
-- Almacena la ruta de la imagen promocional que se imprime al final del ticket
-- cuando el checkbox "Imprimir Ticket promocional" está activado.
ALTER TABLE configTicket
ADD COLUMN imagenPromo VARCHAR(500) DEFAULT '';
