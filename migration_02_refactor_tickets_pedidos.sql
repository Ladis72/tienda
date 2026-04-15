-- ---------------------------------------------------------
-- MIGRACIÓN 02: REFACTORIZACIÓN DE TICKETS Y PEDIDOS
-- Fecha: 2026-04-14
-- Descripción: Consolida columnas de IVA y actualiza tipos de datos.
-- ---------------------------------------------------------

-- Modificar tabla TICKETS (Consolidación)
ALTER TABLE `tickets`
    CHANGE COLUMN `base0` `base` DOUBLE NOT NULL DEFAULT 0,
    CHANGE COLUMN `iva0` `iva` DOUBLE NOT NULL DEFAULT 0,
    DROP COLUMN `base1`,
    DROP COLUMN `iva1`,
    DROP COLUMN `base5`,
    DROP COLUMN `iva5`,
    DROP COLUMN `base2`,
    DROP COLUMN `iva2`,
    DROP COLUMN `base3`,
    DROP COLUMN `iva3`;

-- Modificar tabla PEDIDOS
ALTER TABLE `pedidos`
    DROP COLUMN `re5`,
    DROP COLUMN `iva5`,
    DROP COLUMN `base5`,
    DROP COLUMN `re0`,
    DROP COLUMN `iva0`,
    DROP COLUMN `base0`,
    DROP COLUMN `re3`,
    DROP COLUMN `iva3`,
    DROP COLUMN `base3`,
    DROP COLUMN `re2`,
    DROP COLUMN `iva2`,
    DROP COLUMN `base2`,
    DROP COLUMN `re1`,
    DROP COLUMN `iva1`,
    DROP COLUMN `base1`;

-- Modificar LINEASPEDIDO y LINEASPEDIDO_TMP
ALTER TABLE `lineaspedido`
    CHANGE COLUMN `tipoIva` `tipoIva` DOUBLE NOT NULL DEFAULT 0;

ALTER TABLE `lineaspedido_tmp`
    CHANGE COLUMN `tipoIva` `tipoIva` DOUBLE NOT NULL DEFAULT 0;

-- Modificar LINEASTICKET y LINEASTICKET_TMP
ALTER TABLE `lineasticket`
    CHANGE COLUMN `iva` `iva` DOUBLE NOT NULL DEFAULT 0,
    CHANGE COLUMN `precio` `precio` DOUBLE NOT NULL DEFAULT 0,
    CHANGE COLUMN `descuento` `descuento` DOUBLE NOT NULL DEFAULT 0,
    CHANGE COLUMN `totallinea` `totallinea` DOUBLE NOT NULL DEFAULT 0;

ALTER TABLE `lineasticket_tmp`
    CHANGE COLUMN `iva` `iva` DOUBLE NOT NULL DEFAULT 0,
    CHANGE COLUMN `precio` `precio` DOUBLE NOT NULL DEFAULT 0,
    CHANGE COLUMN `descuento` `descuento` DOUBLE NOT NULL DEFAULT 0,
    CHANGE COLUMN `totallinea` `totallinea` DOUBLE NOT NULL DEFAULT 0;

-- Otros ajustes (Albaranes)
ALTER TABLE `albaranes` ADD COLUMN IF NOT EXISTS `idFactura` VARCHAR(50) DEFAULT NULL;
