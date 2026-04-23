# Comparativa de Estructura de BD: `tiendaNueva` Local vs `tiendaNueva` (Cervantes)

## Diferencias en Tablas Comunes
### Tabla `arqueos`
- Columnas solo en Local: `usuario`
- Columnas solo en Cervantes: `ususario`

### Tabla `configuracion`
- Columnas solo en Cervantes: `nube_comunes`

### Tabla `sync_control`
- Columnas solo en Cervantes: `pendientes`

### Tabla `tickets`
- Columna `descuento` difiere en tipo: Local -> `float`, Cervantes -> `double`

### Tabla `ticketss`
- Columna `base` difiere en tipo: Local -> `float(10,4)`, Cervantes -> `double`
- Columna `descuento` difiere en tipo: Local -> `float`, Cervantes -> `double`
- Columna `iva` difiere en tipo: Local -> `float(10,4)`, Cervantes -> `double`
- Columna `total` difiere en tipo: Local -> `float(10,2)`, Cervantes -> `double`

### Tabla `verifactu_logs`
- Columna `id` difiere en tipo: Local -> `int(10) unsigned`, Cervantes -> `int(11)`
- Columna `id_factura` difiere en tipo: Local -> `int(10) unsigned`, Cervantes -> `int(11)`

