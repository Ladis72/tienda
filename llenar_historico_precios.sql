-- ==============================================================================
-- Script para poblar la tabla historico_precios con los cambios históricos reales
-- ==============================================================================

-- 1. Asegurarnos de que la tabla existe
CREATE TABLE IF NOT EXISTS `historico_precios` (
  `id` INT AUTO_INCREMENT PRIMARY KEY,
  `cod_articulo` VARCHAR(64) NOT NULL,
  `tipo` VARCHAR(20) NOT NULL,
  `precio_viejo` DOUBLE(10,3),
  `precio_nuevo` DOUBLE(10,3),
  `fecha_cambio` DATETIME DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 2. Rellenar historial de PVP usando las variaciones de precio encontradas en lineasticket
INSERT INTO `historico_precios` (`cod_articulo`, `tipo`, `precio_viejo`, `precio_nuevo`, `fecha_cambio`)
WITH HistorialPVP AS (
    SELECT 
        cod AS cod_articulo,
        precio AS precio_nuevo,
        LAG(precio) OVER (PARTITION BY cod ORDER BY fecha, hora) AS precio_viejo,
        CAST(CONCAT(fecha, ' ', hora) AS DATETIME) AS fecha_cambio
    FROM (
        -- Agrupamos por ticket para evitar fluctuaciones por líneas duplicadas en un mismo ticket
        SELECT cod, fecha, hora, MAX(precio) AS precio
        FROM `lineasticket`
        WHERE cod IS NOT NULL AND cod != ''
        GROUP BY cod, fecha, hora
    ) AS agrupado
)
SELECT cod_articulo, 'PVP', precio_viejo, precio_nuevo, fecha_cambio
FROM HistorialPVP
WHERE precio_viejo IS NOT NULL AND precio_nuevo != precio_viejo;

-- 3. Rellenar historial de COSTO usando las variaciones encontradas en los pedidos de compras
INSERT INTO `historico_precios` (`cod_articulo`, `tipo`, `precio_viejo`, `precio_nuevo`, `fecha_cambio`)
WITH HistorialCOSTO AS (
    SELECT 
        agrupado.cod AS cod_articulo,
        agrupado.costo AS precio_nuevo,
        LAG(agrupado.costo) OVER (PARTITION BY agrupado.cod ORDER BY agrupado.fechaPedido) AS precio_viejo,
        CAST(agrupado.fechaPedido AS DATETIME) AS fecha_cambio
    FROM (
        -- Agrupamos por pedido para evitar lineas duplicadas con ligeras diferencias en el mismo día
        SELECT lp.cod, p.fechaPedido, MAX(lp.costo) as costo
        FROM `lineaspedido` lp
        JOIN `pedidos` p ON lp.nDocumento = p.npedido
        WHERE lp.cod IS NOT NULL AND lp.cod != ''
        GROUP BY lp.cod, p.fechaPedido
    ) AS agrupado
)
SELECT cod_articulo, 'COSTO', precio_viejo, precio_nuevo, fecha_cambio
FROM HistorialCOSTO
WHERE precio_viejo IS NOT NULL AND precio_nuevo != precio_viejo;
