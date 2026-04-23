-- SCRIPT DE MIGRACIÓN: Casablanca a Local
-- Excluyendo tablas/columnas autogeneradas por el código C++ en tienda.cpp

-- ATENCIÓN: La tabla 'Tickets' no existe en Casablanca.
-- Generando estructura básica para 'Tickets' (Ajustar si es necesario)
;

-- ATENCIÓN: La tabla 'config_nube' no existe en Casablanca.
-- Generando estructura básica para 'config_nube' (Ajustar si es necesario)
CREATE TABLE `config_nube` (
  `id` int(11) NOT NULL DEFAULT 1,
  `servidor` varchar(255) NOT NULL DEFAULT '',
  `puerto` int(11) NOT NULL DEFAULT 3306,
  `baseDatos` varchar(100) NOT NULL DEFAULT '',
  `usuario` varchar(100) NOT NULL DEFAULT '',
  `clave` varchar(255) NOT NULL DEFAULT '',
  `ssl_ca` varchar(512) DEFAULT '',
;

-- ATENCIÓN: La tabla 'sync_cola' no existe en Casablanca.
-- Generando estructura básica para 'sync_cola' (Ajustar si es necesario)
CREATE TABLE `sync_cola` (
  `id` bigint(20) unsigned NOT NULL,
  `tabla` varchar(64) NOT NULL COMMENT 'Nombre de la tabla modificada',
  `id_registro` varchar(64) NOT NULL COMMENT 'PK del registro modificado',
  `accion` enum('INSERT','UPDATE','DELETE') NOT NULL,
  `fecha` datetime NOT NULL DEFAULT current_timestamp(),
  `subido` tinyint(1) NOT NULL DEFAULT 0 COMMENT '1=subido a la nube',
;

-- ATENCIÓN: La tabla 'sync_control' no existe en Casablanca.
-- Generando estructura básica para 'sync_control' (Ajustar si es necesario)
CREATE TABLE `sync_control` (
  `tabla` varchar(64) NOT NULL,
  `ultima_sync` datetime NOT NULL DEFAULT '2000-01-01 00:00:00',
;

-- ATENCIÓN: La tabla 'sync_unificaciones' no existe en Casablanca.
-- Generando estructura básica para 'sync_unificaciones' (Ajustar si es necesario)
CREATE TABLE `sync_unificaciones` (
  `id` bigint(20) unsigned NOT NULL,
  `tabla` varchar(64) NOT NULL,
  `id_perdedor` varchar(64) NOT NULL,
  `id_ganador` varchar(64) NOT NULL,
  `subido` tinyint(1) NOT NULL DEFAULT 0,
  `fecha` datetime NOT NULL DEFAULT current_timestamp(),
;

-- ATENCIÓN: La tabla 'caducados' existe en Casablanca pero NO en local.
-- DROP TABLE IF EXISTS `caducados`; 

ALTER TABLE `articulos` ADD COLUMN `id_tienda_origen` int(11);
ALTER TABLE `articulos` ADD COLUMN `updated_at` timestamp;
ALTER TABLE `clientes` ADD COLUMN `id_tienda_origen` int(11);
ALTER TABLE `clientes` ADD COLUMN `updated_at` timestamp;
ALTER TABLE `codaux` ADD COLUMN `id_tienda_origen` int(11);
ALTER TABLE `codaux` ADD COLUMN `updated_at` timestamp;
ALTER TABLE `fabricantes` ADD COLUMN `id_tienda_origen` int(11);
ALTER TABLE `fabricantes` ADD COLUMN `updated_at` timestamp;
ALTER TABLE `familias` ADD COLUMN `id_tienda_origen` int(11);
ALTER TABLE `familias` ADD COLUMN `updated_at` timestamp;
ALTER TABLE `formatos` ADD COLUMN `id_tienda_origen` int(11);
ALTER TABLE `formatos` ADD COLUMN `updated_at` timestamp;
ALTER TABLE `fpago` ADD COLUMN `id_tienda_origen` int(11);
ALTER TABLE `fpago` ADD COLUMN `updated_at` timestamp;
ALTER TABLE `impuestos` ADD COLUMN `id_tienda_origen` int(11);
ALTER TABLE `impuestos` ADD COLUMN `updated_at` timestamp;
ALTER TABLE `motivosEntrada` ADD COLUMN `id_tienda_origen` int(11);
ALTER TABLE `motivosEntrada` ADD COLUMN `updated_at` timestamp;
ALTER TABLE `permisos` ADD COLUMN `id_tienda_origen` int(11);
ALTER TABLE `permisos` ADD COLUMN `updated_at` timestamp;
ALTER TABLE `proveedores` ADD COLUMN `id_tienda_origen` int(11);
ALTER TABLE `proveedores` ADD COLUMN `updated_at` timestamp;
ALTER TABLE `tickets` MODIFY COLUMN `descuento` double;
ALTER TABLE `ticketss` MODIFY COLUMN `base` double;
ALTER TABLE `ticketss` MODIFY COLUMN `descuento` double;
ALTER TABLE `ticketss` MODIFY COLUMN `iva` double;
ALTER TABLE `ticketss` MODIFY COLUMN `total` decimal(10,2);
ALTER TABLE `tiendas` MODIFY COLUMN `password` varchar(100);
ALTER TABLE `usuarios` ADD COLUMN `id_tienda_origen` int(11);
ALTER TABLE `usuarios` ADD COLUMN `updated_at` timestamp;
ALTER TABLE `vales` ADD COLUMN `id_tienda_origen` int(11);
ALTER TABLE `vales` ADD COLUMN `updated_at` timestamp;
ALTER TABLE `verifactu_logs` MODIFY COLUMN `id` int(10) unsigned;
ALTER TABLE `verifactu_logs` MODIFY COLUMN `id_factura` int(10) unsigned;

-- TRIGGERS DE SINCRONIZACIÓN (Generados automáticamente)
DROP TRIGGER IF EXISTS sync_articulos_insert;
CREATE TRIGGER sync_articulos_insert AFTER INSERT ON articulos FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'articulos', NEW.cod, 'INSERT' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_articulos_update;
CREATE TRIGGER sync_articulos_update AFTER UPDATE ON articulos FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'articulos', NEW.cod, 'UPDATE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_articulos_delete;
CREATE TRIGGER sync_articulos_delete AFTER DELETE ON articulos FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'articulos', OLD.cod, 'DELETE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_clientes_insert;
CREATE TRIGGER sync_clientes_insert AFTER INSERT ON clientes FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'clientes', NEW.idCliente, 'INSERT' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_clientes_update;
CREATE TRIGGER sync_clientes_update AFTER UPDATE ON clientes FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'clientes', NEW.idCliente, 'UPDATE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_clientes_delete;
CREATE TRIGGER sync_clientes_delete AFTER DELETE ON clientes FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'clientes', OLD.idCliente, 'DELETE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_codaux_insert;
CREATE TRIGGER sync_codaux_insert AFTER INSERT ON codaux FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'codaux', NEW.id, 'INSERT' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_codaux_update;
CREATE TRIGGER sync_codaux_update AFTER UPDATE ON codaux FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'codaux', NEW.id, 'UPDATE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_codaux_delete;
CREATE TRIGGER sync_codaux_delete AFTER DELETE ON codaux FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'codaux', OLD.id, 'DELETE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_fabricantes_insert;
CREATE TRIGGER sync_fabricantes_insert AFTER INSERT ON fabricantes FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'fabricantes', NEW.id, 'INSERT' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_fabricantes_update;
CREATE TRIGGER sync_fabricantes_update AFTER UPDATE ON fabricantes FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'fabricantes', NEW.id, 'UPDATE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_fabricantes_delete;
CREATE TRIGGER sync_fabricantes_delete AFTER DELETE ON fabricantes FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'fabricantes', OLD.id, 'DELETE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_familias_insert;
CREATE TRIGGER sync_familias_insert AFTER INSERT ON familias FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'familias', NEW.id, 'INSERT' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_familias_update;
CREATE TRIGGER sync_familias_update AFTER UPDATE ON familias FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'familias', NEW.id, 'UPDATE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_familias_delete;
CREATE TRIGGER sync_familias_delete AFTER DELETE ON familias FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'familias', OLD.id, 'DELETE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_formatos_insert;
CREATE TRIGGER sync_formatos_insert AFTER INSERT ON formatos FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'formatos', NEW.idformato, 'INSERT' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_formatos_update;
CREATE TRIGGER sync_formatos_update AFTER UPDATE ON formatos FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'formatos', NEW.idformato, 'UPDATE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_formatos_delete;
CREATE TRIGGER sync_formatos_delete AFTER DELETE ON formatos FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'formatos', OLD.idformato, 'DELETE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_fpago_insert;
CREATE TRIGGER sync_fpago_insert AFTER INSERT ON fpago FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'fpago', NEW.id, 'INSERT' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_fpago_update;
CREATE TRIGGER sync_fpago_update AFTER UPDATE ON fpago FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'fpago', NEW.id, 'UPDATE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_fpago_delete;
CREATE TRIGGER sync_fpago_delete AFTER DELETE ON fpago FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'fpago', OLD.id, 'DELETE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_impuestos_insert;
CREATE TRIGGER sync_impuestos_insert AFTER INSERT ON impuestos FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'impuestos', NEW.tipoIva, 'INSERT' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_impuestos_update;
CREATE TRIGGER sync_impuestos_update AFTER UPDATE ON impuestos FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'impuestos', NEW.tipoIva, 'UPDATE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_impuestos_delete;
CREATE TRIGGER sync_impuestos_delete AFTER DELETE ON impuestos FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'impuestos', OLD.tipoIva, 'DELETE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_motivosEntrada_insert;
CREATE TRIGGER sync_motivosEntrada_insert AFTER INSERT ON motivosEntrada FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'motivosEntrada', NEW.idtiposEntrada, 'INSERT' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_motivosEntrada_update;
CREATE TRIGGER sync_motivosEntrada_update AFTER UPDATE ON motivosEntrada FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'motivosEntrada', NEW.idtiposEntrada, 'UPDATE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_motivosEntrada_delete;
CREATE TRIGGER sync_motivosEntrada_delete AFTER DELETE ON motivosEntrada FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'motivosEntrada', OLD.idtiposEntrada, 'DELETE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_permisos_insert;
CREATE TRIGGER sync_permisos_insert AFTER INSERT ON permisos FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'permisos', NEW.id, 'INSERT' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_permisos_update;
CREATE TRIGGER sync_permisos_update AFTER UPDATE ON permisos FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'permisos', NEW.id, 'UPDATE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_permisos_delete;
CREATE TRIGGER sync_permisos_delete AFTER DELETE ON permisos FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'permisos', OLD.id, 'DELETE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_proveedores_insert;
CREATE TRIGGER sync_proveedores_insert AFTER INSERT ON proveedores FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'proveedores', NEW.idProveedor, 'INSERT' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_proveedores_update;
CREATE TRIGGER sync_proveedores_update AFTER UPDATE ON proveedores FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'proveedores', NEW.idProveedor, 'UPDATE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_proveedores_delete;
CREATE TRIGGER sync_proveedores_delete AFTER DELETE ON proveedores FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'proveedores', OLD.idProveedor, 'DELETE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_usuarios_insert;
CREATE TRIGGER sync_usuarios_insert AFTER INSERT ON usuarios FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'usuarios', NEW.id, 'INSERT' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_usuarios_update;
CREATE TRIGGER sync_usuarios_update AFTER UPDATE ON usuarios FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'usuarios', NEW.id, 'UPDATE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_usuarios_delete;
CREATE TRIGGER sync_usuarios_delete AFTER DELETE ON usuarios FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'usuarios', OLD.id, 'DELETE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_vales_insert;
CREATE TRIGGER sync_vales_insert AFTER INSERT ON vales FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'vales', NEW.idvales, 'INSERT' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_vales_update;
CREATE TRIGGER sync_vales_update AFTER UPDATE ON vales FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'vales', NEW.idvales, 'UPDATE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_vales_delete;
CREATE TRIGGER sync_vales_delete AFTER DELETE ON vales FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'vales', OLD.idvales, 'DELETE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_verifactu_logs_insert;
CREATE TRIGGER sync_verifactu_logs_insert AFTER INSERT ON verifactu_logs FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'verifactu_logs', NEW.id, 'INSERT' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_verifactu_logs_update;
CREATE TRIGGER sync_verifactu_logs_update AFTER UPDATE ON verifactu_logs FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'verifactu_logs', NEW.id, 'UPDATE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

DROP TRIGGER IF EXISTS sync_verifactu_logs_delete;
CREATE TRIGGER sync_verifactu_logs_delete AFTER DELETE ON verifactu_logs FOR EACH ROW
INSERT INTO sync_cola (tabla, id_registro, accion) SELECT 'verifactu_logs', OLD.id, 'DELETE' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;

-- TRIGGERS PARA CONTROL DE STOCK EN LOTES
DROP TRIGGER IF EXISTS actStockInsert;
CREATE TRIGGER actStockInsert AFTER INSERT ON lotes
FOR EACH ROW UPDATE articulos SET stock= (SELECT sum(cantidad) FROM lotes WHERE ean = NEW.ean) where articulos.cod = NEW.ean;

DROP TRIGGER IF EXISTS actStockUpdate;
CREATE TRIGGER actStockUpdate AFTER UPDATE ON lotes
FOR EACH ROW UPDATE articulos SET stock= (SELECT sum(cantidad) FROM lotes WHERE ean = OLD.ean) where articulos.cod = OLD.ean;

