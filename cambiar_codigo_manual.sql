-- SCRIPT PARA CAMBIAR EL CÓDIGO DE UN ARTÍCULO MANUALMENTE EN TODAS LAS TABLAS
-- Instrucciones:
-- 1. Cambia el valor de @oldCod por el código que quieres quitar.
-- 2. Cambia el valor de @newCod por el código nuevo que quieres poner.
-- 3. Ejecuta todo el script de golpe en tu cliente SQL.

SET @oldCod = 'CÓDIGO_VIEJO';
SET @newCod = 'CÓDIGO_NUEVO';

-- Desactivar temporalmente las restricciones de claves foráneas
SET FOREIGN_KEY_CHECKS = 0;

-- Obtener la descripción original del artículo (por si las tablas históricas la necesitan)
SET @desc = (SELECT descripcion FROM articulos WHERE cod = @oldCod);

-- 1. Actualizar la tabla principal (articulos)
UPDATE articulos SET cod = @newCod WHERE cod = @oldCod;

-- 2. Actualizar dependencias que solo usan el código
UPDATE encargos SET cod_articulo = @newCod WHERE cod_articulo = @oldCod;
UPDATE precios_tienda SET cod_articulo = @newCod WHERE cod_articulo = @oldCod;
UPDATE lotes SET ean = @newCod WHERE ean = @oldCod;
UPDATE etiquetas SET cod = @newCod WHERE cod = @oldCod;
UPDATE codaux SET cod = @newCod WHERE cod = @oldCod;
UPDATE historico_stock SET ean = @newCod WHERE ean = @oldCod;

-- 3. Actualizar dependencias históricas que usan el código y la descripción desnormalizada
UPDATE lineasticket SET cod = @newCod, descripcion = @desc WHERE cod = @oldCod;
UPDATE lineaspedido SET cod = @newCod, descripcion = @desc WHERE cod = @oldCod;
UPDATE lineaspedido_tmp SET cod = @newCod, descripcion = @desc WHERE cod = @oldCod;
UPDATE entradaGenero_tmp SET cod = @newCod, descripcion = @desc WHERE cod = @oldCod;
UPDATE salidaGenero_tmp SET cod = @newCod, descripcion = @desc WHERE cod = @oldCod;
UPDATE lineasticket_tmp SET cod = @newCod, descripcion = @desc WHERE cod = @oldCod;
UPDATE salidaGenero SET cod = @newCod, descripcion = @desc WHERE cod = @oldCod;

-- Reactivar las restricciones de claves foráneas
SET FOREIGN_KEY_CHECKS = 1;
