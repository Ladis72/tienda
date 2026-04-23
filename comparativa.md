# Comparativa de Base de Datos: `tienda` (Local) vs `tiendaNueva` (Emeicjac)

## Tablas solo en `tienda`
- `artitien0000`
- `caducados`
- `novadiet`

## Tablas solo en `tiendaNueva`
- `Tickets`
- `arqueos_detalle`
- `config_nube`
- `encargos`
- `historico_stock`
- `impuestos`
- `logs`
- `notas`
- `permisos`
- `precios_tienda`
- `sync_cola`
- `sync_control`
- `sync_unificaciones`
- `verifactu_logs`

## Diferencias en Tablas Comunes
### Tabla `albaranes`
- Columnas solo en `tiendaNueva`: `idFactura`

### Tabla `arqueos`
- Columnas solo en `tiendaNueva`: `efectivoContado`, `usuario`

### Tabla `articulos`
- Columnas solo en `tiendaNueva`: `id_tienda_origen`, `updated_at`
- Columna `descripcion` difiere en tipo: `tienda` -> `char(50)`, `tiendaNueva` -> `char(90)`

### Tabla `clientes`
- Columnas solo en `tiendaNueva`: `id_tienda_origen`, `updated_at`

### Tabla `codaux`
- Columnas solo en `tiendaNueva`: `id_tienda_origen`, `updated_at`

### Tabla `configTicket`
- Columnas solo en `tiendaNueva`: `imagenPromo`

### Tabla `configuracion`
- Columnas solo en `tiendaNueva`: `precios_locales`

### Tabla `entradasSalidas`
- Columnas solo en `tiendaNueva`: `usuario`

### Tabla `fabricantes`
- Columnas solo en `tiendaNueva`: `id_tienda_origen`, `updated_at`

### Tabla `facturas`
- Columna `nFactura` difiere en tipo: `tienda` -> `char(15)`, `tiendaNueva` -> `text`

### Tabla `familias`
- Columnas solo en `tiendaNueva`: `id_tienda_origen`, `updated_at`

### Tabla `formatos`
- Columnas solo en `tiendaNueva`: `id_tienda_origen`, `updated_at`

### Tabla `fpago`
- Columnas solo en `tiendaNueva`: `id_tienda_origen`, `updated_at`

### Tabla `lineaspedido`
- Columna `tipoIva` difiere en tipo: `tienda` -> `int(11)`, `tiendaNueva` -> `double`

### Tabla `lineaspedido_tmp`
- Columna `costo` difiere en tipo: `tienda` -> `decimal(10,3)`, `tiendaNueva` -> `double`
- Columna `pvp` difiere en tipo: `tienda` -> `decimal(10,2)`, `tiendaNueva` -> `double`
- Columna `tipoIva` difiere en tipo: `tienda` -> `int(11)`, `tiendaNueva` -> `double`

### Tabla `lineasticket`
- Columna `descuento` difiere en tipo: `tienda` -> `float`, `tiendaNueva` -> `double`
- Columna `iva` difiere en tipo: `tienda` -> `int(11)`, `tiendaNueva` -> `double`
- Columna `precio` difiere en tipo: `tienda` -> `float`, `tiendaNueva` -> `double`
- Columna `totallinea` difiere en tipo: `tienda` -> `float`, `tiendaNueva` -> `double`

### Tabla `lineasticket_tmp`
- Columna `descuento` difiere en tipo: `tienda` -> `float`, `tiendaNueva` -> `double`
- Columna `iva` difiere en tipo: `tienda` -> `int(11)`, `tiendaNueva` -> `double`
- Columna `precio` difiere en tipo: `tienda` -> `float`, `tiendaNueva` -> `double`
- Columna `totallinea` difiere en tipo: `tienda` -> `float`, `tiendaNueva` -> `double`

### Tabla `motivosEntrada`
- Columnas solo en `tiendaNueva`: `id_tienda_origen`, `updated_at`

### Tabla `pedidos`
- Columnas solo en `tienda`: `base1`, `base2`, `base3`, `iva1`, `iva2`, `iva3`, `re1`, `re2`, `re3`

### Tabla `proveedores`
- Columnas solo en `tiendaNueva`: `id_tienda_origen`, `updated_at`

### Tabla `tickets`
- Columnas solo en `tienda`: `base1`, `base2`, `base3`, `iva1`, `iva2`, `iva3`
- Columnas solo en `tiendaNueva`: `base`, `cambio`, `entrega`, `iva`

### Tabla `ticketss`
- Columnas solo en `tienda`: `base1`, `base2`, `base3`, `iva1`, `iva2`, `iva3`
- Columnas solo en `tiendaNueva`: `base`, `cambio`, `entrega`, `iva`

### Tabla `tiendas`
- Columnas solo en `tiendaNueva`: `baseDatos`, `local`, `master`, `puerto`, `ssl_ca`
- Columna `ip` difiere en tipo: `tienda` -> `varchar(45)`, `tiendaNueva` -> `varchar(100)`
- Columna `password` difiere en tipo: `tienda` -> `varchar(15)`, `tiendaNueva` -> `varchar(100)`
- Columna `usuario` difiere en tipo: `tienda` -> `varchar(15)`, `tiendaNueva` -> `varchar(100)`

### Tabla `usuarios`
- Columnas solo en `tiendaNueva`: `id_tienda_origen`, `updated_at`

### Tabla `vales`
- Columnas solo en `tiendaNueva`: `id_tienda_origen`, `updated_at`

