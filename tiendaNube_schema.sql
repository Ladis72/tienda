/*M!999999\- enable the sandbox mode */ 
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*M!100616 SET @OLD_NOTE_VERBOSITY=@@NOTE_VERBOSITY, NOTE_VERBOSITY=0 */;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8mb4;
/*!50001 CREATE VIEW `Tickets` AS SELECT
 1 AS `ticket`,
  1 AS `usuario`,
  1 AS `cliente`,
  1 AS `fecha`,
  1 AS `hora`,
  1 AS `base`,
  1 AS `iva`,
  1 AS `descuento`,
  1 AS `total`,
  1 AS `fpago`,
  1 AS `entrega`,
  1 AS `cambio`,
  1 AS `descripcion`,
  1 AS `cantidad`,
  1 AS `precio`,
  1 AS `descuentoLinea`,
  1 AS `totallinea` */;
SET character_set_client = @saved_cs_client;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `articulos` (
  `cod` char(13) NOT NULL,
  `descripcion` char(90) DEFAULT NULL,
  `pvp` float(10,2) NOT NULL,
  `iva` int(3) NOT NULL DEFAULT 10,
  `stock` int(3) NOT NULL,
  `min` int(3) NOT NULL DEFAULT 0,
  `max` int(3) NOT NULL DEFAULT 0,
  `pendientes_pedido` int(11) NOT NULL,
  `encargados` int(11) NOT NULL,
  `ultima_venta` date DEFAULT NULL,
  `ultimo_pedido` date DEFAULT NULL,
  `familia` int(3) NOT NULL DEFAULT 0,
  `precio_compra` decimal(10,3) NOT NULL,
  `fabricante` int(3) NOT NULL DEFAULT 0,
  `foto` char(100) DEFAULT NULL,
  `notas` mediumtext NOT NULL,
  `formato` varchar(45) DEFAULT NULL,
  `cantformato` float(10,2) DEFAULT NULL,
  `updated_at` timestamp NOT NULL DEFAULT current_timestamp(),
  `id_tienda_origen` tinyint(3) unsigned DEFAULT 1,
  PRIMARY KEY (`cod`),
  UNIQUE KEY `ka` (`cod`),
  KEY `updated_at` (`updated_at`),
  KEY `updated_at_2` (`updated_at`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `clientes` (
  `idCliente` int(11) NOT NULL,
  `nombre` char(25) NOT NULL,
  `apellidos` char(50) NOT NULL,
  `direccion` char(50) NOT NULL,
  `cp` char(5) NOT NULL,
  `localidad` char(25) NOT NULL,
  `provincia` char(25) NOT NULL,
  `nif` char(15) NOT NULL,
  `telefono` char(9) NOT NULL,
  `telefono2` char(9) NOT NULL,
  `mail` char(50) NOT NULL,
  `descuento` float NOT NULL DEFAULT 0,
  `fechaAlta` date NOT NULL,
  `notas` mediumtext NOT NULL,
  `updated_at` timestamp NOT NULL DEFAULT current_timestamp(),
  `id_tienda_origen` tinyint(3) unsigned DEFAULT 1,
  PRIMARY KEY (`idCliente`),
  UNIQUE KEY `idCliente` (`idCliente`),
  KEY `apellidos` (`apellidos`),
  KEY `updated_at` (`updated_at`),
  KEY `updated_at_2` (`updated_at`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `codaux` (
  `id` int(11) NOT NULL,
  `cod` char(13) NOT NULL,
  `aux` char(13) NOT NULL,
  `updated_at` timestamp NOT NULL DEFAULT current_timestamp(),
  `id_tienda_origen` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`,`aux`),
  KEY `updated_at` (`updated_at`),
  KEY `updated_at_2` (`updated_at`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `fabricantes` (
  `id` int(3) NOT NULL,
  `nombre` char(100) NOT NULL,
  `updated_at` timestamp NOT NULL DEFAULT current_timestamp(),
  `id_tienda_origen` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `updated_at` (`updated_at`),
  KEY `updated_at_2` (`updated_at`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `familias` (
  `id` int(4) NOT NULL,
  `descripcion` char(100) NOT NULL,
  `updated_at` timestamp NOT NULL DEFAULT current_timestamp(),
  `id_tienda_origen` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `updated_at` (`updated_at`),
  KEY `updated_at_2` (`updated_at`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `formatos` (
  `idformato` int(11) NOT NULL,
  `formato` varchar(45) DEFAULT NULL,
  `updated_at` timestamp NOT NULL DEFAULT current_timestamp(),
  `id_tienda_origen` int(11) DEFAULT NULL,
  PRIMARY KEY (`idformato`),
  KEY `updated_at` (`updated_at`),
  KEY `updated_at_2` (`updated_at`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `fpago` (
  `id` int(11) NOT NULL,
  `tipo` char(25) NOT NULL,
  `efectivo` tinyint(1) NOT NULL,
  `updated_at` timestamp NOT NULL DEFAULT current_timestamp(),
  `id_tienda_origen` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `updated_at` (`updated_at`),
  KEY `updated_at_2` (`updated_at`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `impuestos` (
  `tipoIva` tinyint(4) NOT NULL,
  `porcentaje` double DEFAULT NULL,
  `recargo` double DEFAULT NULL,
  `updated_at` timestamp NOT NULL DEFAULT current_timestamp(),
  `id_tienda_origen` int(11) DEFAULT NULL,
  PRIMARY KEY (`tipoIva`),
  KEY `updated_at` (`updated_at`),
  KEY `updated_at_2` (`updated_at`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `motivosEntrada` (
  `idtiposEntrada` int(11) NOT NULL,
  `descripcion` varchar(45) DEFAULT NULL,
  `updated_at` timestamp NOT NULL DEFAULT current_timestamp(),
  `id_tienda_origen` int(11) DEFAULT NULL,
  PRIMARY KEY (`idtiposEntrada`),
  KEY `updated_at` (`updated_at`),
  KEY `updated_at_2` (`updated_at`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `permisos` (
  `idPermiso` int(11) NOT NULL,
  `id` int(11) DEFAULT NULL,
  `rol` int(11) DEFAULT NULL,
  `clave` varchar(64) DEFAULT NULL,
  `updated_at` timestamp NOT NULL DEFAULT current_timestamp(),
  `deleted_at` timestamp NULL DEFAULT NULL,
  `id_tienda_origen` int(11) DEFAULT NULL,
  PRIMARY KEY (`idPermiso`),
  KEY `updated_at` (`updated_at`),
  KEY `updated_at_2` (`updated_at`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `proveedores` (
  `idProveedor` int(11) NOT NULL,
  `nombre` char(50) DEFAULT NULL,
  `nif` char(15) DEFAULT NULL,
  `direccion` char(50) DEFAULT NULL,
  `cp` char(5) DEFAULT NULL,
  `localidad` char(50) DEFAULT NULL,
  `provincia` char(25) DEFAULT NULL,
  `representante` char(50) DEFAULT NULL,
  `telefonor` char(15) DEFAULT NULL,
  `mailr` char(50) DEFAULT NULL,
  `telefono` char(15) DEFAULT NULL,
  `mail` char(50) DEFAULT NULL,
  `descuento` float NOT NULL,
  `fechaultimacompra` date NOT NULL DEFAULT '2000-01-01',
  `formapago` int(11) DEFAULT NULL,
  `notas` text NOT NULL,
  `updated_at` timestamp NOT NULL DEFAULT current_timestamp(),
  `id_tienda_origen` int(11) DEFAULT NULL,
  PRIMARY KEY (`idProveedor`),
  UNIQUE KEY `proveedores_nif_IDX` (`nif`),
  KEY `updated_at` (`updated_at`),
  KEY `updated_at_2` (`updated_at`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `tiendas` (
  `id` int(11) NOT NULL,
  `nombre` varchar(45) DEFAULT NULL,
  `direccion` varchar(45) DEFAULT NULL,
  `ciudad` varchar(45) DEFAULT NULL,
  `telefono` varchar(15) DEFAULT NULL,
  `whatsapp` varchar(15) DEFAULT NULL,
  `email` varchar(45) DEFAULT NULL,
  `ip` varchar(100) DEFAULT NULL,
  `usuario` varchar(15) DEFAULT NULL,
  `password` varchar(100) DEFAULT NULL,
  `master` tinyint(4) DEFAULT NULL,
  `local` tinyint(4) DEFAULT NULL,
  `baseDatos` varchar(255) DEFAULT NULL,
  `puerto` int(11) DEFAULT NULL,
  `ssl_ca` varchar(500) DEFAULT NULL,
  `updated_at` timestamp NOT NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `usuarios` (
  `id` int(11) NOT NULL,
  `nombre` char(20) DEFAULT NULL,
  `apellido` char(40) DEFAULT NULL,
  `direccion` char(50) DEFAULT NULL,
  `localidad` char(30) DEFAULT NULL,
  `provincia` char(30) DEFAULT NULL,
  `cp` char(5) DEFAULT NULL,
  `tlfn` char(15) DEFAULT NULL,
  `nif` char(15) DEFAULT NULL,
  `mail` char(30) DEFAULT NULL,
  `usuario` char(10) DEFAULT NULL,
  `clave` char(10) DEFAULT NULL,
  `notas` char(250) DEFAULT NULL,
  `rol` int(1) DEFAULT NULL,
  `foto` char(255) DEFAULT NULL,
  `updated_at` timestamp NOT NULL DEFAULT current_timestamp(),
  `id_tienda_origen` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `updated_at` (`updated_at`),
  KEY `updated_at_2` (`updated_at`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `vales` (
  `idvales` int(11) DEFAULT NULL,
  `idCliente` int(11) DEFAULT NULL,
  `cantidad` double(10,2) DEFAULT NULL,
  `estado` tinyint(1) DEFAULT NULL,
  `fechaEmision` date DEFAULT NULL,
  `fechaUso` date DEFAULT NULL,
  `updated_at` timestamp NOT NULL DEFAULT current_timestamp(),
  `id_tienda_origen` int(11) DEFAULT NULL,
  KEY `updated_at` (`updated_at`),
  KEY `updated_at_2` (`updated_at`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50001 DROP VIEW IF EXISTS `Tickets`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8mb4 */;
/*!50001 SET character_set_results     = utf8mb4 */;
/*!50001 SET collation_connection      = utf8mb4_uca1400_ai_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`dbpgf38423283`@`%` SQL SECURITY DEFINER */
/*!50001 VIEW `Tickets` AS select 1 AS `ticket`,1 AS `usuario`,1 AS `cliente`,1 AS `fecha`,1 AS `hora`,1 AS `base`,1 AS `iva`,1 AS `descuento`,1 AS `total`,1 AS `fpago`,1 AS `entrega`,1 AS `cambio`,1 AS `descripcion`,1 AS `cantidad`,1 AS `precio`,1 AS `descuentoLinea`,1 AS `totallinea` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*M!100616 SET NOTE_VERBOSITY=@OLD_NOTE_VERBOSITY */;

