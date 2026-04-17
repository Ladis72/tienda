-- ---------------------------------------------------------
-- MIGRACIÓN 04: LIMPIEZA DE COLUMNAS REDUNDANTES EN ARTICULOS
-- Fecha: 2026-04-17
-- Descripción: Elimina columnas redundantes (calculadas dinámicamente) 
--              y asegura que la tabla en la nube solo tenga datos maestros.
-- ---------------------------------------------------------

-- 1. LIMPIEZA EN LOCAL (Eliminar campos redundantes que ya se calculan en tiempo real)
ALTER TABLE `articulos` 
    DROP COLUMN IF EXISTS `stock`,
    DROP COLUMN IF EXISTS `pendientes_pedido`,
    DROP COLUMN IF EXISTS `encargados`,
    DROP COLUMN IF EXISTS `minimo_pedido`;

-- 2. LIMPIEZA PARA LA NUBE (Estos comandos deben ejecutarse en la BD de la nube)
-- En la nube, TAMBIÉN eliminamos min, max y fechas de venta/pedido ya que son locales de cada tienda.
-- El código de la aplicación ya ha sido actualizado para ignorar estas columnas.

/*
ALTER TABLE `articulos` 
    DROP COLUMN IF EXISTS `stock`,
    DROP COLUMN IF EXISTS `pendientes_pedido`,
    DROP COLUMN IF EXISTS `encargados`,
    DROP COLUMN IF EXISTS `minimo_pedido`,
    DROP COLUMN IF EXISTS `min`,
    DROP COLUMN IF EXISTS `max`,
    DROP COLUMN IF EXISTS `ultima_venta`,
    DROP COLUMN IF EXISTS `ultimo_pedido`;
*/
