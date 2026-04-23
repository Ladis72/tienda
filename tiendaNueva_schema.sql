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
CREATE TABLE `albaranes` (
  `id` int(11) NOT NULL,
  `nFactura` char(15) NOT NULL,
  `fechaFactura` date NOT NULL,
  `idProveedor` int(11) NOT NULL,
  `totalBase` double NOT NULL,
  `totalIva` double NOT NULL,
  `totalRe` double NOT NULL,
  `total` double NOT NULL,
  `facturada` tinyint(1) NOT NULL,
  `idFactura` varchar(50) DEFAULT NULL,
  KEY `id` (`id`),
  KEY `idFactura` (`idFactura`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `albaranes_tmp` (
  `id` int(11) NOT NULL,
  `idProveedor` int(11) NOT NULL,
  `npedido` char(15) NOT NULL,
  `fechaPedido` date NOT NULL,
  PRIMARY KEY (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `arqueos` (
  `id` int(11) NOT NULL,
  `fecha` date DEFAULT NULL,
  `hora` varchar(45) DEFAULT NULL,
  `ventasEfectivo` double DEFAULT NULL,
  `ventasTarjeta` double DEFAULT NULL,
  `entradas` double DEFAULT NULL,
  `efectivoReal` double DEFAULT NULL,
  `descuadre` double DEFAULT NULL,
  `efectivoContado` double DEFAULT 0,
  `usuario` varchar(50) DEFAULT '',
  PRIMARY KEY (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `arqueos_detalle` (
  `id` int(11) NOT NULL,
  `idArqueo` int(11) NOT NULL,
  `denominacion` decimal(5,2) NOT NULL,
  `cantidad` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  KEY `idArqueo` (`idArqueo`),
  CONSTRAINT `1` FOREIGN KEY (`idArqueo`) REFERENCES `arqueos` (`id`) ON DELETE CASCADE
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `articulos` (
  `cod` char(13) NOT NULL,
  `descripcion` char(90) DEFAULT NULL,
  `pvp` float(10,2) NOT NULL,
  `iva` int(3) NOT NULL DEFAULT 10,
  `stock` int(3) NOT NULL DEFAULT 0,
  `min` int(3) NOT NULL DEFAULT 0,
  `max` int(3) NOT NULL DEFAULT 0,
  `pendientes_pedido` int(11) NOT NULL DEFAULT 0,
  `encargados` int(11) NOT NULL DEFAULT 0,
  `ultima_venta` date DEFAULT NULL,
  `ultimo_pedido` date DEFAULT NULL,
  `familia` int(3) DEFAULT NULL,
  `precio_compra` decimal(10,3) DEFAULT NULL,
  `fabricante` int(3) DEFAULT NULL,
  `foto` char(100) DEFAULT NULL,
  `notas` text DEFAULT NULL,
  `formato` varchar(45) DEFAULT NULL,
  `cantformato` float(10,2) DEFAULT NULL,
  `id_tienda_origen` int(11) DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`cod`),
  KEY `updated_at` (`updated_at`),
  KEY `id_tienda_origen` (`id_tienda_origen`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_articulos_insert AFTER INSERT ON articulos FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('articulos', NEW.cod, 'INSERT');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_articulos_update AFTER UPDATE ON articulos FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('articulos', NEW.cod, 'UPDATE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_articulos_delete AFTER DELETE ON articulos FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('articulos', OLD.cod, 'DELETE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
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
  `notas` text NOT NULL,
  `id_tienda_origen` int(11) DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`idCliente`),
  KEY `apellidos` (`apellidos`),
  KEY `updated_at` (`updated_at`),
  KEY `id_tienda_origen` (`id_tienda_origen`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_clientes_insert AFTER INSERT ON clientes FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('clientes', NEW.idCliente, 'INSERT');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_clientes_update AFTER UPDATE ON clientes FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('clientes', NEW.idCliente, 'UPDATE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_clientes_delete AFTER DELETE ON clientes FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('clientes', OLD.idCliente, 'DELETE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `codaux` (
  `id` int(11) NOT NULL,
  `cod` char(13) NOT NULL,
  `aux` char(13) NOT NULL,
  `id_tienda_origen` int(11) DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`,`aux`),
  KEY `updated_at` (`updated_at`),
  KEY `id_tienda_origen` (`id_tienda_origen`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_codaux_insert AFTER INSERT ON codaux FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('codaux', NEW.id, 'INSERT');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_codaux_update AFTER UPDATE ON codaux FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('codaux', NEW.id, 'UPDATE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_codaux_delete AFTER DELETE ON codaux FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('codaux', OLD.id, 'DELETE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `comprasClienteTMP` (
  `idcompra` int(11) NOT NULL,
  `idCliente` int(11) DEFAULT NULL,
  `cantidad` double(10,2) DEFAULT NULL,
  `fecha` date DEFAULT NULL,
  PRIMARY KEY (`idcompra`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `configBase` (
  `id` varchar(45) NOT NULL,
  `servidor` varchar(45) DEFAULT NULL,
  `puerto` varchar(45) DEFAULT NULL,
  `baseDatos` varchar(45) DEFAULT NULL,
  `usuario` varchar(45) DEFAULT NULL,
  `clave` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `configTicket` (
  `id` varchar(45) NOT NULL,
  `cabecera` text DEFAULT NULL,
  `pie` text DEFAULT NULL,
  `promocion` text DEFAULT NULL,
  `boolPromocion` tinyint(1) DEFAULT NULL,
  `ruta` varchar(45) DEFAULT NULL,
  `codApertura` varchar(45) DEFAULT NULL,
  `codCorte` varchar(45) DEFAULT NULL,
  `imagenPromo` varchar(500) DEFAULT '',
  PRIMARY KEY (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `config_nube` (
  `id` int(11) NOT NULL DEFAULT 1,
  `servidor` varchar(255) NOT NULL DEFAULT '',
  `puerto` int(11) NOT NULL DEFAULT 3306,
  `baseDatos` varchar(100) NOT NULL DEFAULT '',
  `usuario` varchar(100) NOT NULL DEFAULT '',
  `clave` varchar(255) NOT NULL DEFAULT '',
  `ssl_ca` varchar(512) DEFAULT '',
  PRIMARY KEY (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `configuracion` (
  `idconfiguracion` int(11) NOT NULL,
  `recargoeq` tinyint(1) DEFAULT NULL,
  `precios_locales` tinyint(1) DEFAULT 0,
  PRIMARY KEY (`idconfiguracion`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `directorios` (
  `id` int(11) NOT NULL,
  `nombre` varchar(45) DEFAULT NULL,
  `directorio` varchar(155) DEFAULT NULL,
  PRIMARY KEY (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `encargos` (
  `id_encargo` int(11) NOT NULL,
  `id_cliente` int(11) NOT NULL,
  `cod_articulo` varchar(15) NOT NULL,
  `cantidad` int(11) NOT NULL,
  `fecha_encargo` datetime DEFAULT current_timestamp(),
  `notas` varchar(255) DEFAULT NULL,
  `empleado` varchar(100) DEFAULT NULL,
  `anticipo` double(10,2) DEFAULT 0.00,
  `estado` enum('Pendiente','Recibido','Entregado','Cancelado') DEFAULT 'Pendiente',
  PRIMARY KEY (`id_encargo`),
  KEY `id_cliente` (`id_cliente`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `entradaGenero` (
  `id` int(11) NOT NULL,
  `cod` char(13) NOT NULL,
  `fechaEntrada` date NOT NULL,
  `descripcion` char(100) NOT NULL,
  `cantidad` int(11) NOT NULL,
  `fechaCaducidad` date NOT NULL,
  `pvp` double NOT NULL,
  `idTienda` int(2) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `idTienda` (`idTienda`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `entradaGenero_tmp` (
  `id` int(11) NOT NULL,
  `cod` char(13) NOT NULL,
  `fechaEntrada` date NOT NULL,
  `descripcion` char(100) NOT NULL,
  `cantidad` int(11) NOT NULL,
  `fechaCaducidad` date NOT NULL,
  `pvp` double NOT NULL,
  `idTienda` int(2) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `idTienda` (`idTienda`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `entradasSalidas` (
  `identradasSalidas` int(11) NOT NULL,
  `fecha` date DEFAULT NULL,
  `hora` time DEFAULT NULL,
  `cantidad` double DEFAULT NULL,
  `idTiposRentrada` int(11) DEFAULT NULL,
  `descripcion` varchar(145) DEFAULT NULL,
  `usuario` varchar(45) DEFAULT 'Sin Usuario',
  PRIMARY KEY (`identradasSalidas`),
  KEY `idTiposRentrada` (`idTiposRentrada`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `etiquetas` (
  `cod` char(13) NOT NULL,
  PRIMARY KEY (`cod`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `fabricantes` (
  `id` int(3) NOT NULL,
  `nombre` char(100) NOT NULL,
  `id_tienda_origen` int(11) DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`id`),
  KEY `updated_at` (`updated_at`),
  KEY `id_tienda_origen` (`id_tienda_origen`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_fabricantes_insert AFTER INSERT ON fabricantes FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('fabricantes', NEW.id, 'INSERT');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_fabricantes_update AFTER UPDATE ON fabricantes FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('fabricantes', NEW.id, 'UPDATE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_fabricantes_delete AFTER DELETE ON fabricantes FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('fabricantes', OLD.id, 'DELETE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `facturas` (
  `id` int(11) NOT NULL,
  `nFactura` text NOT NULL,
  `fechaFactura` date NOT NULL,
  `idProveedor` int(11) NOT NULL,
  `totalBase` double NOT NULL,
  `totalIva` double NOT NULL,
  `totalRe` double NOT NULL,
  `total` double NOT NULL,
  `vencimiento` date NOT NULL,
  `pagada` tinyint(1) NOT NULL,
  KEY `id` (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `familias` (
  `id` int(4) NOT NULL,
  `descripcion` char(100) NOT NULL,
  `id_tienda_origen` int(11) DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`id`),
  KEY `updated_at` (`updated_at`),
  KEY `id_tienda_origen` (`id_tienda_origen`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_familias_insert AFTER INSERT ON familias FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('familias', NEW.id, 'INSERT');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_familias_update AFTER UPDATE ON familias FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('familias', NEW.id, 'UPDATE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_familias_delete AFTER DELETE ON familias FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('familias', OLD.id, 'DELETE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `formatos` (
  `idformato` int(11) NOT NULL,
  `formato` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `id_tienda_origen` int(11) DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`idformato`),
  KEY `updated_at` (`updated_at`),
  KEY `id_tienda_origen` (`id_tienda_origen`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_formatos_insert AFTER INSERT ON formatos FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('formatos', NEW.idformato, 'INSERT');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_formatos_update AFTER UPDATE ON formatos FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('formatos', NEW.idformato, 'UPDATE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_formatos_delete AFTER DELETE ON formatos FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('formatos', OLD.idformato, 'DELETE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `fpago` (
  `id` int(11) NOT NULL,
  `tipo` char(25) NOT NULL,
  `efectivo` tinyint(1) NOT NULL,
  `id_tienda_origen` int(11) DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`id`),
  KEY `updated_at` (`updated_at`),
  KEY `id_tienda_origen` (`id_tienda_origen`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_fpago_insert AFTER INSERT ON fpago FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('fpago', NEW.id, 'INSERT');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_fpago_update AFTER UPDATE ON fpago FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('fpago', NEW.id, 'UPDATE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_fpago_delete AFTER DELETE ON fpago FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('fpago', OLD.id, 'DELETE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `historico_stock` (
  `id` int(11) NOT NULL,
  `ean` varchar(15) NOT NULL,
  `lote` varchar(50) DEFAULT NULL,
  `fecha_caducidad_ant` date DEFAULT NULL,
  `fecha_caducidad_new` date DEFAULT NULL,
  `stock_ant` double(10,2) DEFAULT NULL,
  `stock_new` double(10,2) DEFAULT NULL,
  `motivo` varchar(255) DEFAULT NULL,
  `usuario` varchar(100) DEFAULT NULL,
  `fecha_hora` datetime DEFAULT current_timestamp(),
  PRIMARY KEY (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `impuestos` (
  `tipoIva` tinyint(4) NOT NULL,
  `porcentaje` double DEFAULT NULL,
  `recargo` double DEFAULT NULL,
  `id_tienda_origen` int(11) DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`tipoIva`),
  KEY `updated_at` (`updated_at`),
  KEY `id_tienda_origen` (`id_tienda_origen`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_impuestos_insert AFTER INSERT ON impuestos FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('impuestos', NEW.tipoIva, 'INSERT');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_impuestos_update AFTER UPDATE ON impuestos FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('impuestos', NEW.tipoIva, 'UPDATE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_impuestos_delete AFTER DELETE ON impuestos FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('impuestos', OLD.tipoIva, 'DELETE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `lineaspedido` (
  `id` int(11) NOT NULL,
  `nDocumento` char(13) NOT NULL,
  `idProveedor` int(11) NOT NULL,
  `cod` char(13) NOT NULL,
  `descripcion` char(50) NOT NULL,
  `cantidad` int(11) NOT NULL,
  `bonificacion` int(11) NOT NULL,
  `lote` char(13) NOT NULL,
  `fc` date NOT NULL,
  `costo` double NOT NULL,
  `descuento1` double NOT NULL,
  `base` double NOT NULL,
  `tipoIva` double NOT NULL DEFAULT 0,
  `totalbase` double NOT NULL,
  `iva` double NOT NULL,
  `re` double NOT NULL,
  `pvp` double NOT NULL,
  UNIQUE KEY `id` (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `lineaspedido_tmp` (
  `id` int(11) NOT NULL,
  `idPedido` int(11) NOT NULL,
  `cod` char(13) NOT NULL,
  `descripcion` char(50) NOT NULL,
  `cantidad` int(11) NOT NULL,
  `bonificacion` int(11) NOT NULL,
  `lote` char(13) NOT NULL,
  `fc` date NOT NULL,
  `costo` double NOT NULL,
  `descuento1` double NOT NULL,
  `base` double NOT NULL,
  `tipoIva` double NOT NULL DEFAULT 0,
  `totalbase` double NOT NULL,
  `iva` double NOT NULL,
  `re` double NOT NULL,
  `pvp` double NOT NULL,
  UNIQUE KEY `id` (`id`),
  KEY `idPedido` (`idPedido`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `lineasticket` (
  `id` int(11) NOT NULL,
  `nticket` char(11) NOT NULL,
  `cod` char(13) NOT NULL,
  `descripcion` char(50) NOT NULL,
  `cantidad` int(11) NOT NULL,
  `iva` double NOT NULL DEFAULT 0,
  `precio` double NOT NULL DEFAULT 0,
  `descuento` double NOT NULL DEFAULT 0,
  `totallinea` double NOT NULL DEFAULT 0,
  `fecha` date NOT NULL,
  `hora` time NOT NULL,
  PRIMARY KEY (`id`),
  KEY `nticket` (`nticket`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `lineasticket_tmp` (
  `id` int(11) NOT NULL,
  `orden` int(11) NOT NULL,
  `cod` char(13) NOT NULL,
  `descripcion` char(50) NOT NULL,
  `cantidad` int(11) NOT NULL,
  `iva` double NOT NULL DEFAULT 0,
  `precio` double NOT NULL DEFAULT 0,
  `descuento` double NOT NULL DEFAULT 0,
  `totallinea` double NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `logs` (
  `id` int(11) NOT NULL,
  `timestamp` datetime NOT NULL DEFAULT current_timestamp(),
  `categoria` varchar(50) NOT NULL,
  `usuario` varchar(50) DEFAULT NULL,
  `mensaje` text NOT NULL,
  PRIMARY KEY (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `lotes` (
  `id` int(11) NOT NULL,
  `ean` char(13) NOT NULL,
  `lote` char(13) NOT NULL,
  `fecha` date NOT NULL,
  `cantidad` int(3) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `ean` (`ean`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`ladis`@`%`*/ /*!50003 TRIGGER `tiendaNueva`.`actStockInsert` AFTER INSERT ON `lotes`
 FOR EACH ROW UPDATE `articulos` SET `stock`= (SELECT sum(`cantidad`) FROM `lotes` WHERE `ean` = NEW.`ean`) where `articulos`.`cod` = NEW.`ean` */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`ladis`@`%`*/ /*!50003 TRIGGER `tiendaNueva`.`actStockUpdate` AFTER UPDATE ON `lotes`
 FOR EACH ROW UPDATE `articulos` SET `stock`= (SELECT sum(`cantidad`) FROM `lotes` WHERE `ean` = OLD.`ean`) where `articulos`.`cod` = OLD.`ean` */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `motivosEntrada` (
  `idtiposEntrada` int(11) NOT NULL,
  `descripcion` varchar(45) DEFAULT NULL,
  `id_tienda_origen` int(11) DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`idtiposEntrada`),
  KEY `updated_at` (`updated_at`),
  KEY `id_tienda_origen` (`id_tienda_origen`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_motivosEntrada_insert AFTER INSERT ON motivosEntrada FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('motivosEntrada', NEW.idtiposEntrada, 'INSERT');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_motivosEntrada_update AFTER UPDATE ON motivosEntrada FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('motivosEntrada', NEW.idtiposEntrada, 'UPDATE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_motivosEntrada_delete AFTER DELETE ON motivosEntrada FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('motivosEntrada', OLD.idtiposEntrada, 'DELETE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `notas` (
  `id` int(11) NOT NULL,
  `titulo` varchar(200) NOT NULL,
  `descripcion` text DEFAULT NULL,
  `usuario` varchar(100) NOT NULL,
  `fecha_creacion` datetime DEFAULT current_timestamp(),
  `fecha_limite` date DEFAULT NULL,
  `estado` enum('Pendiente','Completada') DEFAULT 'Pendiente',
  `prioridad` enum('Alta','Normal','Baja') DEFAULT 'Normal',
  PRIMARY KEY (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `pedidos` (
  `id` int(11) NOT NULL,
  `idProveedor` int(11) NOT NULL,
  `npedido` char(15) NOT NULL,
  `fechaPedido` date NOT NULL,
  `nLineas` int(11) NOT NULL,
  `nArticulos` int(11) NOT NULL,
  `descuento` float NOT NULL,
  `totalbase` float NOT NULL,
  `totaliva` float NOT NULL,
  `totalre` float NOT NULL,
  `total` float NOT NULL,
  `nFactura` char(10) NOT NULL,
  PRIMARY KEY (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `permisos` (
  `id` int(11) NOT NULL,
  `rol` int(11) NOT NULL,
  `clave` varchar(64) NOT NULL,
  `id_tienda_origen` int(11) DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_rol_clave` (`rol`,`clave`),
  KEY `updated_at` (`updated_at`),
  KEY `id_tienda_origen` (`id_tienda_origen`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_permisos_insert AFTER INSERT ON permisos FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('permisos', NEW.id, 'INSERT');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_permisos_update AFTER UPDATE ON permisos FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('permisos', NEW.id, 'UPDATE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_permisos_delete AFTER DELETE ON permisos FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('permisos', OLD.id, 'DELETE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `precios_tienda` (
  `cod_articulo` varchar(15) NOT NULL,
  `pvp` double(10,3) DEFAULT 0.000,
  `precio_venta` double(10,3) DEFAULT 0.000,
  PRIMARY KEY (`cod_articulo`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `prestamos` (
  `id` int(11) NOT NULL,
  `ean` varchar(13) DEFAULT NULL,
  `producto` varchar(45) DEFAULT NULL,
  `tienda` varchar(15) DEFAULT NULL,
  `fecha` date DEFAULT NULL,
  `cantidad` int(11) DEFAULT NULL,
  `notas` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `proveedores` (
  `idProveedor` int(11) NOT NULL,
  `nombre` char(50) NOT NULL,
  `nif` char(15) NOT NULL,
  `direccion` char(50) NOT NULL,
  `cp` char(5) NOT NULL,
  `localidad` char(50) NOT NULL,
  `provincia` char(25) NOT NULL,
  `representante` char(50) NOT NULL,
  `telefonor` char(15) NOT NULL,
  `mailr` char(50) NOT NULL,
  `telefono` char(15) NOT NULL,
  `mail` char(50) NOT NULL,
  `descuento` float NOT NULL,
  `fechaultimacompra` date NOT NULL,
  `formapago` int(11) NOT NULL,
  `notas` text NOT NULL,
  `id_tienda_origen` int(11) DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`idProveedor`),
  UNIQUE KEY `nif` (`nif`),
  KEY `updated_at` (`updated_at`),
  KEY `id_tienda_origen` (`id_tienda_origen`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_proveedores_insert AFTER INSERT ON proveedores FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('proveedores', NEW.idProveedor, 'INSERT');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_proveedores_update AFTER UPDATE ON proveedores FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('proveedores', NEW.idProveedor, 'UPDATE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_proveedores_delete AFTER DELETE ON proveedores FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('proveedores', OLD.idProveedor, 'DELETE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `salidaGenero` (
  `id` int(11) NOT NULL,
  `cod` char(13) NOT NULL,
  `fechaEntrada` date NOT NULL,
  `descripcion` char(100) NOT NULL,
  `cantidad` int(11) NOT NULL,
  `fechaCaducidad` date NOT NULL,
  `pvp` double NOT NULL,
  `idTienda` int(2) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `idTienda` (`idTienda`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `salidaGenero_tmp` (
  `id` int(11) NOT NULL,
  `cod` char(13) NOT NULL,
  `fechaEntrada` date NOT NULL,
  `descripcion` char(100) NOT NULL,
  `cantidad` int(11) NOT NULL,
  `fechaCaducidad` date NOT NULL,
  `pvp` double NOT NULL,
  `idTienda` int(2) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `idTienda` (`idTienda`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `sujetoPrestamo` (
  `id` int(11) NOT NULL,
  `sujetoPrestamocol` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `sync_cola` (
  `id` bigint(20) unsigned NOT NULL,
  `tabla` varchar(64) NOT NULL COMMENT 'Nombre de la tabla modificada',
  `id_registro` varchar(64) NOT NULL COMMENT 'PK del registro modificado',
  `accion` enum('INSERT','UPDATE','DELETE') NOT NULL,
  `fecha` datetime NOT NULL DEFAULT current_timestamp(),
  `subido` tinyint(1) NOT NULL DEFAULT 0 COMMENT '1=subido a la nube',
  PRIMARY KEY (`id`),
  KEY `id_registro` (`id_registro`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `sync_control` (
  `tabla` varchar(64) NOT NULL,
  `ultima_sync` datetime NOT NULL DEFAULT '2000-01-01 00:00:00',
  PRIMARY KEY (`tabla`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `sync_unificaciones` (
  `id` bigint(20) unsigned NOT NULL,
  `tabla` varchar(64) NOT NULL,
  `id_perdedor` varchar(64) NOT NULL,
  `id_ganador` varchar(64) NOT NULL,
  `subido` tinyint(1) NOT NULL DEFAULT 0,
  `fecha` datetime NOT NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`id`),
  KEY `id_perdedor` (`id_perdedor`),
  KEY `id_ganador` (`id_ganador`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `ticket_tmp` (
  `orden` int(11) NOT NULL,
  `cliente` int(11) NOT NULL,
  `vendedor` int(11) NOT NULL,
  UNIQUE KEY `orden` (`orden`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `tickets` (
  `ticket` int(10) unsigned NOT NULL,
  `usuario` int(11) NOT NULL DEFAULT 1,
  `cliente` int(11) NOT NULL DEFAULT 1,
  `fecha` date NOT NULL,
  `hora` time NOT NULL,
  `base` double NOT NULL DEFAULT 0,
  `iva` double NOT NULL DEFAULT 0,
  `descuento` float NOT NULL,
  `total` decimal(10,2) NOT NULL,
  `fpago` int(11) NOT NULL,
  `cobrado` tinyint(1) NOT NULL,
  `entrega` double DEFAULT NULL,
  `cambio` double DEFAULT NULL,
  PRIMARY KEY (`ticket`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `ticketss` (
  `ticket` int(10) unsigned NOT NULL,
  `usuario` int(11) NOT NULL DEFAULT 1,
  `cliente` int(11) NOT NULL DEFAULT 1,
  `fecha` date NOT NULL,
  `hora` time NOT NULL,
  `base` float(10,4) NOT NULL DEFAULT 0.0000,
  `iva` float(10,4) NOT NULL DEFAULT 0.0000,
  `descuento` float NOT NULL,
  `total` float(10,2) NOT NULL,
  `fpago` int(11) NOT NULL,
  `cobrado` tinyint(1) NOT NULL,
  `entrega` double DEFAULT NULL,
  `cambio` double DEFAULT NULL,
  PRIMARY KEY (`ticket`)
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
  `usuario` varchar(100) DEFAULT NULL,
  `password` varchar(100) DEFAULT NULL,
  `master` tinyint(4) DEFAULT NULL,
  `local` tinyint(4) DEFAULT NULL,
  `baseDatos` varchar(255) DEFAULT 'tiendaNueva',
  `puerto` int(11) DEFAULT 3306,
  `ssl_ca` varchar(500) DEFAULT '',
  PRIMARY KEY (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `usuarios` (
  `id` int(2) NOT NULL,
  `nombre` char(20) NOT NULL,
  `apellido` char(40) NOT NULL,
  `direccion` char(50) NOT NULL,
  `localidad` char(30) NOT NULL,
  `provincia` char(30) NOT NULL,
  `cp` char(5) NOT NULL,
  `tlfn` char(15) NOT NULL,
  `nif` char(15) NOT NULL,
  `mail` char(30) NOT NULL,
  `usuario` char(10) NOT NULL,
  `clave` char(10) NOT NULL,
  `notas` char(250) NOT NULL,
  `rol` int(1) NOT NULL,
  `foto` char(255) DEFAULT 'imagenes/anonimo.jpg',
  `id_tienda_origen` int(11) DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`id`),
  KEY `updated_at` (`updated_at`),
  KEY `id_tienda_origen` (`id_tienda_origen`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_usuarios_insert AFTER INSERT ON usuarios FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('usuarios', NEW.id, 'INSERT');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_usuarios_update AFTER UPDATE ON usuarios FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('usuarios', NEW.id, 'UPDATE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_usuarios_delete AFTER DELETE ON usuarios FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('usuarios', OLD.id, 'DELETE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `vales` (
  `idvales` int(11) NOT NULL,
  `idCliente` int(11) DEFAULT NULL,
  `cantidad` double(10,2) DEFAULT NULL,
  `estado` tinyint(1) DEFAULT NULL,
  `fechaEmision` date DEFAULT NULL,
  `fechaUso` date DEFAULT NULL,
  `id_tienda_origen` int(11) DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`idvales`),
  KEY `updated_at` (`updated_at`),
  KEY `id_tienda_origen` (`id_tienda_origen`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_vales_insert AFTER INSERT ON vales FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('vales', NEW.idvales, 'INSERT');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_vales_update AFTER UPDATE ON vales FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('vales', NEW.idvales, 'UPDATE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_uca1400_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER sync_vales_delete AFTER DELETE ON vales FOR EACH ROW BEGIN   IF @skip_sync IS NULL OR @skip_sync = 0 THEN     INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('vales', OLD.idvales, 'DELETE');   END IF; END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `valesPendientesMarcar` (
  `id` int(11) NOT NULL,
  `idVale` int(11) DEFAULT NULL,
  `tienda` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `ventasClienteTMP` (
  `id` int(11) NOT NULL,
  `idCliente` int(11) DEFAULT NULL,
  `cantidad` double DEFAULT NULL,
  PRIMARY KEY (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `verifactu_logs` (
  `id` int(10) unsigned NOT NULL,
  `id_factura` int(10) unsigned NOT NULL,
  `fecha_hora` timestamp NULL DEFAULT current_timestamp(),
  `hash_actual` varchar(64) NOT NULL,
  `hash_anterior` varchar(64) NOT NULL,
  `cadena_firmada` text NOT NULL,
  `usuario` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `verifactu_logs_tickets_FK` (`id_factura`),
  CONSTRAINT `verifactu_logs_tickets_FK` FOREIGN KEY (`id_factura`) REFERENCES `tickets` (`ticket`)
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
/*!50013 DEFINER=`ladis`@`%` SQL SECURITY DEFINER */
/*!50001 VIEW `Tickets` AS select 1 AS `ticket`,1 AS `usuario`,1 AS `cliente`,1 AS `fecha`,1 AS `hora`,1 AS `base`,1 AS `iva`,1 AS `descuento`,1 AS `total`,1 AS `fpago`,1 AS `entrega`,1 AS `cambio`,1 AS `descripcion`,1 AS `cantidad`,1 AS `precio`,1 AS `descuentoLinea`,1 AS `totallinea` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*M!100616 SET NOTE_VERBOSITY=@OLD_NOTE_VERBOSITY */;

