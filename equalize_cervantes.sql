-- ----------------------------------------------------------------------------
-- SCRIPT DE IGUALACIÓN: CERVANTES -> EMEICJAC
-- ----------------------------------------------------------------------------
-- Este script realiza los cambios que el programa NO hace automáticamente 
-- al arrancar. Ejecutar sobre la base de datos 'cervantes'.
-- ----------------------------------------------------------------------------

-- 1. ELIMINAR COLUMNAS SOBRANTES (Limpieza de tablas no maestras)
-- Estas columnas existen en Cervantes pero no en Emeicjac.
ALTER TABLE `albaranes` DROP COLUMN IF EXISTS `updated_at`;
ALTER TABLE `albaranes` DROP COLUMN IF EXISTS `id_tienda_origen`;
ALTER TABLE `lotes` DROP COLUMN IF EXISTS `updated_at`;
ALTER TABLE `lotes` DROP COLUMN IF EXISTS `id_tienda_origen`;

-- 2. ELIMINAR ÍNDICES SOBRANTES EN ARTICULOS
-- Cervantes tiene 'idx_nombre', pero Emeicjac no.
ALTER TABLE `articulos` DROP INDEX IF EXISTS `idx_nombre`;

-- 3. AÑADIR COLUMNAS DE CONFIGURACIÓN Y CONTROL
-- Estas columnas están en Emeicjac pero no se crean por auto-inicio básico.
ALTER TABLE `configuracion` 
    ADD COLUMN IF NOT EXISTS `nube_comunes` TINYINT(1) DEFAULT '0';

ALTER TABLE `sync_control` 
    ADD COLUMN IF NOT EXISTS `pendientes` INT DEFAULT '0';

-- 4. CREAR TABLAS FALTANTES
-- La tabla 'caducados' se infiere de los scripts de migración existentes.
CREATE TABLE IF NOT EXISTS `caducados` (
  `id` INT AUTO_INCREMENT PRIMARY KEY,
  `cod` VARCHAR(15) NOT NULL,
  `fechaCaducidad` DATE DEFAULT NULL,
  `cantidad` DOUBLE(10,2) DEFAULT '0.00',
  `fecha` DATETIME DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- La tabla 'novadiet' se crea con estructura básica de productos.
CREATE TABLE IF NOT EXISTS `novadiet` (
  `cod` VARCHAR(15) PRIMARY KEY,
  `descripcion` VARCHAR(100),
  `pvp` DOUBLE(10,3) DEFAULT '0.000'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 5. RE-SINCROINZAR ÍNDICES CRÍTICOS
-- Emeicjac tiene idx_ean (aunque cod sea PK, lo añadimos para igualar).
ALTER TABLE `articulos` ADD INDEX IF NOT EXISTS `idx_ean` (`cod`);

-- 6. ACTUALIZAR VISTA DE TICKETS
-- Reemplaza la vista con la definición estándar.
CREATE OR REPLACE VIEW `Tickets` AS 
SELECT 
    1 AS `ticket`, 1 AS `usuario`, 1 AS `cliente`, 1 AS `fecha`, 1 AS `hora`, 
    1 AS `base`, 1 AS `iva`, 1 AS `descuento`, 1 AS `total`, 1 AS `fpago`, 
    1 AS `entrega`, 1 AS `cambio`, 1 AS `descripcion`, 1 AS `cantidad`, 
    1 AS `precio`, 1 AS `descuentoLinea`, 1 AS `totallinea`;

-- ----------------------------------------------------------------------------
-- NOTA: Al ejecutar el programa C++ (después de compilar con el parche de índices),
-- se crearán automáticamente el resto de objetos:
-- (sync_cola, sync_control, triggers, columnas de auditoría en tablas maestras, etc.)
-- de forma LIMPIA y sin duplicar índices.
-- ----------------------------------------------------------------------------
