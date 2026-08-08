# Tienda

TPV (punto de venta) para gestión de comercios desarrollado en **Qt 6 / C++** con
**MariaDB/MySQL**. Incluye venta en caja (TPV), gestión de artículos, clientes,
proveedores, pedidos, stock por lotes con control de caducidades, históricos,
impresión de tickets/facturas, cumplimiento **VeriFactu** y sincronización
multi-tienda con la nube.

## Características principales

- **TPV**: venta por código o búsqueda multitérmino, devoluciones, encargos,
  múltiples formas de pago y vales.
- **Stock**: control por lotes, fechas de caducidad, monitor de caducados y
  recomendaciones de compra.
- **Fiscalidad**: cadena de hash y QR conforme a la normativa VeriFactu de la
  AEAT.
- **Multi-tienda**: sincronización de maestros, tickets, pedidos y stock entre
  la tienda local y la nube con resolución de conflictos por `updated_at`.
- **Permisos**: acceso a la interfaz por rol de usuario (gestor de permisos).
- **Seguridad**: autenticación con SHA-256 + salt y bloqueo anti-fuerza-bruta,
  consultas SQL con *prepared statements* y verificación de integridad de la
  base de datos.

## Requisitos

- Qt 6 (módulos: `sql`, `charts`, `webenginewidgets`, `concurrent`, `network`,
  `core5compat`, `widgets`)
- MariaDB o MySQL (driver Qt `QMYSQL`)
- Librería `libqrencode` (generación de códigos QR)
- Compilador compatible con C++17

## Compilación

```bash
qmake6 tienda.pro
make -j$(nproc)
```

El binario se genera en el directorio de compilación (`tienda`).

## Instalación y configuración

Ver [`Instalación.txt`](Instalación.txt). En resumen:

1. Crear la base de datos (las tablas se crean automáticamente al arrancar).
2. Copiar `tienda.ini.example` como `tienda.ini` y rellenar las credenciales
   de conexión local.
3. Configurar las tiendas y la conexión a la nube desde la aplicación.

> **Seguridad**: `tienda.ini` contiene credenciales y **no** debe versionarse.

## Estructura del proyecto

- `tienda.cpp/.h` — ventana principal y creación inicial de tablas.
- `base_datos.cpp/.h` — capa de acceso a datos (consultas, stock, VeriFactu).
- `syncmanager.cpp/.h` — sincronización multi-tienda con la nube.
- `tpv.cpp/.h` — módulo de caja.
- `verificadorbasedatos.cpp/.h` — comprobación de integridad de la base de datos.
- Los diálogos y formularios usan archivos `.ui` de Qt Designer.
