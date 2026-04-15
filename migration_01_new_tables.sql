-- ---------------------------------------------------------
-- MIGRACIÓN 01: NUEVAS TABLAS
-- Fecha: 2026-04-14
-- Descripción: Crea las tablas de impuestos, notas y encargos.
-- ---------------------------------------------------------

-- Tabla de Impuestos
CREATE TABLE IF NOT EXISTS `impuestos` (
    `tipoIva` TINYINT(4) NOT NULL AUTO_INCREMENT,
    `porcentaje` DOUBLE NULL DEFAULT NULL,
    `recargo` DOUBLE NULL DEFAULT NULL,
    `updated_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    `id_tienda_origen` INT DEFAULT 0,
    PRIMARY KEY (`tipoIva`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Insertar valores por defecto si la tabla está vacía
INSERT INTO `impuestos` (`tipoIva`, `porcentaje`, `recargo`) 
SELECT 1, 21.0, 5.2 WHERE NOT EXISTS (SELECT 1 FROM `impuestos` WHERE `tipoIva` = 1);
INSERT INTO `impuestos` (`tipoIva`, `porcentaje`, `recargo`) 
SELECT 2, 10.0, 1.4 WHERE NOT EXISTS (SELECT 1 FROM `impuestos` WHERE `tipoIva` = 2);
INSERT INTO `impuestos` (`tipoIva`, `porcentaje`, `recargo`) 
SELECT 3, 4.0, 0.5 WHERE NOT EXISTS (SELECT 1 FROM `impuestos` WHERE `tipoIva` = 3);
INSERT INTO `impuestos` (`tipoIva`, `porcentaje`, `recargo`) 
SELECT 4, 0.0, 0.0 WHERE NOT EXISTS (SELECT 1 FROM `impuestos` WHERE `tipoIva` = 4);

-- Sistema de Notas y Avisos
CREATE TABLE IF NOT EXISTS `notas` (
  `id` INT AUTO_INCREMENT PRIMARY KEY,
  `titulo` VARCHAR(200) NOT NULL,
  `descripcion` TEXT,
  `usuario` VARCHAR(100) NOT NULL,
  `fecha_creacion` DATETIME DEFAULT CURRENT_TIMESTAMP,
  `fecha_limite` DATE DEFAULT NULL,
  `estado` ENUM('Pendiente','Completada') DEFAULT 'Pendiente',
  `prioridad` ENUM('Alta','Normal','Baja') DEFAULT 'Normal',
  `updated_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `id_tienda_origen` INT DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Sistema de Encargos
CREATE TABLE IF NOT EXISTS `encargos` (
  `id_encargo` INT AUTO_INCREMENT PRIMARY KEY,
  `id_cliente` INT NOT NULL,
  `cod_articulo` VARCHAR(15) NOT NULL,
  `cantidad` INT NOT NULL,
  `fecha_encargo` DATETIME DEFAULT CURRENT_TIMESTAMP,
  `notas` VARCHAR(255) DEFAULT NULL,
  `empleado` VARCHAR(100) DEFAULT NULL,
  `anticipo` DOUBLE(10,2) DEFAULT '0.00',
  `estado` ENUM('Pendiente', 'Recibido', 'Entregado', 'Cancelado') DEFAULT 'Pendiente',
  `updated_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `id_tienda_origen` INT DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
