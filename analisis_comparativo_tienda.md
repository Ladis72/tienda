# Análisis Comparativo: Tienda vs. Mercado
## Módulos actuales · Brechas · Módulos sugeridos

---

## 1. Inventario de módulos actuales

A partir del código fuente analizado, el programa cuenta con los siguientes módulos:

### 🟢 TPV / Punto de Venta
| Módulo | Archivo(s) | Estado |
|---|---|---|
| Terminal Punto de Venta | `tpv.cpp` / `tpv.ui` | ✅ Completo |
| Totalización / cobro | `totalizar.cpp` | ✅ Completo |
| Tickets pendientes | `tpv.cpp` | ✅ Completo |
| Impresión de ticket | `imprimirticket.cpp` | ✅ Completo |
| Búsqueda de producto en TPV | `buscarproducto.cpp` | ✅ Completo |
| Vales descuento | `generarvales.cpp` | ✅ Completo |
| Encargos de cliente | `encargosdialog.cpp`, `gestorencargosdialog.cpp` | ✅ Completo |
| VeriFACTU (hash cadena) | `verifactuclass.cpp`, `hashutil.h` | ✅ Implementado |

### 🟢 Gestión de Artículos / Stock
| Módulo | Archivo(s) | Estado |
|---|---|---|
| Maestro de artículos | `articulos.cpp` | ✅ Completo |
| Familias y fabricantes | `familias.cpp`, `fabricantes.cpp` | ✅ Completo |
| Formatos de venta | `formatos.cpp` | ✅ Completo |
| Control de stock | `stock.cpp` | ✅ Completo |
| Entrada de mercancía | `entradamercancia.cpp` | ✅ Completo |
| Caducidades | `caducidades.cpp`, `listadocaducados.cpp` | ✅ Completo |
| Historial de precios | `historialprecios.cpp` | ✅ Completo |
| Etiquetas | `etiquetas.cpp` | ✅ Completo |
| Lotes (funciones en `base_datos`) | `base_datos.h` | ✅ En BD, UI parcial |
| Comparar artículos entre tiendas | `dialogcomparararticulos.cpp` | ✅ Completo |

### 🟢 Proveedores y Compras
| Módulo | Archivo(s) | Estado |
|---|---|---|
| Maestro de proveedores | `proveedores.cpp` | ✅ Completo |
| Pedidos a proveedor | `pedidos.cpp`, `gestionpedidos.cpp` | ✅ Completo |
| Aceptar pedido / albarán | `aceptarpedido.cpp` | ✅ Completo |
| Facturar albaranes | `facturaralbaranes.cpp` | ✅ Completo |
| Facturas de proveedor | `imprimirfacturaproveedor.cpp` | ✅ Completo |
| Unificar proveedores | `unificarproveedores.cpp` | ✅ Completo |

### 🟢 Clientes y Facturación
| Módulo | Archivo(s) | Estado |
|---|---|---|
| Maestro de clientes | `clientes.cpp` | ✅ Completo |
| Facturas de cliente | `verfacturas.cpp`, `imprimirfactura.cpp` | ✅ Completo |
| Búsqueda de cliente | `buscarcliente.cpp` | ✅ Completo |
| Formas de pago | `formaspago.cpp` | ✅ Completo |
| Impuestos (IVA configurable) | `impuestos.cpp` | ✅ Completo |

### 🟢 Caja y Tesorería
| Módulo | Archivo(s) | Estado |
|---|---|---|
| Arqueo de caja | `cajas.cpp` | ✅ Completo |
| Entradas/Salidas de efectivo | `entradasalida.cpp`, `salidas.cpp` | ✅ Completo |
| Tipos de movimientos | `tiposentradassalidas.cpp` | ✅ Completo |
| Listado arqueos | `listadoarqueos.cpp` | ✅ Completo |
| Listado movimientos efectivo | `listadosalidas.cpp` | ✅ Completo |

### 🟢 Estadísticas e Informes
| Módulo | Archivo(s) | Estado |
|---|---|---|
| KPIs (tickets, ventas, clientes) | `estadisticas.cpp` | ✅ Completo |
| Gráfico de ventas por período | `graficoventaswidget.cpp` | ✅ Completo |
| Top productos vendidos/rentables | `estadisticas.cpp` | ✅ Completo |
| Ventas por vendedor / forma pago | `estadisticas.cpp` | ✅ Completo |
| Ventas por familia | `estadisticas.cpp` | ✅ Completo |
| Histórico de tickets | `historicotickets.cpp` | ✅ Completo |
| Listado de ventas | `listadoventas.cpp` | ✅ Completo |
| Listado venta por artículo | `listadoventaarticulos.cpp` | ✅ Completo |
| Informes 1 | `informes1.cpp` | 🟡 Básico |

### 🟢 Multi-tienda y Sincronización
| Módulo | Archivo(s) | Estado |
|---|---|---|
| Gestor de sucursales | `tiendas.cpp` | ✅ Completo |
| Conexiones remotas | `conexionesremotas.cpp` | ✅ Completo |
| Sincronización bidireccional nube | `syncmanager.cpp` | ✅ Completo |
| Compras/ventas remoto | `comprasventasremoto.cpp` | ✅ Completo |

### 🟢 Administración y Seguridad
| Módulo | Archivo(s) | Estado |
|---|---|---|
| Login con hash bcrypt | `login.cpp`, `hashutil.h` | ✅ Completo |
| Gestión de roles y permisos | `gestorpermisos.cpp`, `editorpermisos.cpp` | ✅ Completo |
| Usuarios | `base_datos.cpp` | ✅ Completo |
| Log de actividad | `base_datos.h` (insertarLog) | ✅ En BD |
| Copia de seguridad | `base_datos.cpp` | ✅ Completo |

### 🟢 Herramientas / Utilidades
| Módulo | Archivo(s) | Estado |
|---|---|---|
| Notas internas con prioridad | `notaswidget.cpp` | ✅ Completo |
| Saneador global de BD | `saneadorglobal.cpp` | ✅ Completo |
| Unificar maestros | `unificarmaestros.cpp` | ✅ Completo |
| Configuración de ticket impreso | `configticket.cpp` | ✅ Completo |
| Configuración DB / local / otras | `configbase.cpp`, `configlocal.cpp` | ✅ Completo |
| Directorios | `directorios.cpp` | ✅ Completo |
| Formateo numérico | `formateo.cpp` | ✅ Completo |
| Impresión de pedido | `imprimirpedido.cpp` | ✅ Completo |

---

## 2. Comparativa con software del mercado

### Competidores analizados
| Software | Tipo | Precio aprox. | Plataforma |
|---|---|---|---|
| **Holded** | ERP/TPV cloud | 50-200 €/mes | Web |
| **Factusol** | TPV+facturación | Gratuito / 300 € | Windows |
| **TPV Manager** | TPV retail | 30-100 €/mes | Windows |
| **Odoo Community** | ERP completo | Gratuito (hosting) | Web/Linux |
| **FacturaScripts** | Facturación TPV | Gratuito | Web/Linux |

### Tabla comparativa por áreas

| Área funcional | **Tienda** | Holded | Factusol | Odoo |
|---|---|---|---|---|
| TPV táctil | ✅ | ✅ | ✅ | ✅ |
| Multi-tienda real | ✅ (sync nube) | ✅ | ❌ | ✅ |
| Pedidos a proveedor | ✅ | ✅ | ✅ | ✅ |
| VeriFACTU | ✅ | ✅ | ✅ | ❌ (plugins) |
| Encargos de cliente | ✅ | ❌ | ❌ | 🟡 parcial |
| Vales descuento | ✅ | ✅ | ❌ | ✅ |
| Control de lotes | 🟡 (BD, sin UI completa) | ✅ | ✅ | ✅ |
| Caducidades | ✅ | ❌ | 🟡 | ✅ |
| **CRM** | ❌ | ✅ | ❌ | ✅ |
| **E-commerce / web** | ❌ | ✅ | ❌ | ✅ |
| **TPV offline móvil** | ❌ | ❌ | ❌ | 🟡 |
| **Gestión de citas** | ❌ | ❌ | ❌ | ✅ |
| **Fidelización / puntos** | ❌ | ✅ | ❌ | ✅ |
| **Análisis predictivo** | ❌ | 🟡 | ❌ | ❌ |
| **Envío de email** | ❌ | ✅ | 🟡 | ✅ |
| **API REST / integraciones** | ❌ | ✅ | ❌ | ✅ |
| **Dashboard en tiempo real** | 🟡 KPIs básicos | ✅ | ❌ | ✅ |
| **Gestión de reparaciones** | ❌ | ❌ | ❌ | 🟡 |
| **Trazabilidad SSCC/EAN128** | ❌ | ❌ | 🟡 | ✅ |
| Estadísticas avanzadas | ✅ | ✅ | 🟡 | ✅ |
| Permisos granulares | ✅ | 🟡 | ❌ | ✅ |
| Etiquetas de artículos | ✅ | ❌ | ✅ | ✅ |
| Instalación local (offline) | ✅ | ❌ | ✅ | ✅ |
| Open source | ✅ | ❌ | ❌ | ✅ |

> **Ventaja diferencial de Tienda**: instalación local robusta, sincronización multi-tienda propia, VeriFACTU nativo, sistema de encargos único, y control de caducidades — funcionalidades ausentes en muchos competidores.

---

## 3. Módulos sugeridos (priorizados)

### 🔴 ALTA PRIORIDAD — Brechas críticas

#### M01 · UI completa de Lotes / Trazabilidad
**Justificación**: La lógica de lotes ya existe en `base_datos` (crearLote, aumentarLote, etc.) pero carece de interfaz gráfica completa. Es fundamental para sectores como alimentación, cosmética y farmacia.  
**Archivos a crear**: `lotes.cpp/.h/.ui`  
**Esfuerzo estimado**: 2-3 semanas

---

#### M02 · Sistema de Fidelización de Clientes (Puntos)
**Justificación**: Holded y la mayoría de TPV modernos lo incluyen. Aumenta la retención de clientes y el ticket medio. Los vales actuales son unidireccionales; los puntos acumulables son un modelo más potente.  
**Diseño**:
- Tabla `puntos_cliente` (id_cliente, puntos_acumulados, puntos_usados)
- Configuración: ratio €→puntos, puntos→descuento
- Integración en TPV al cobrar
- Consulta de saldo en ficha de cliente

**Archivos a crear**: `fidelizacion.cpp/.h/.ui`, modificación en `tpv.cpp` y `clientes.cpp`  
**Esfuerzo estimado**: 2-3 semanas

---

#### M03 · Envío de Documentos por Email (SMTP)
**Justificación**: Factusol, Holded y prácticamente todo el software del mercado envía facturas y pedidos por email desde la propia aplicación. Actualmente hay que imprimir en PDF y enviar manualmente.  
**Diseño**:
- Uso de `QSslSocket` + `QNetworkAccessManager` o librería SMTP externa
- Configuración SMTP en `configuracionotros`
- Envío desde `imprimirfactura` e `imprimirpedido`
- Plantillas HTML para email de factura/pedido

**Archivos a crear**: `emailmanager.cpp/.h`, modificación en `configbase.cpp`  
**Esfuerzo estimado**: 1-2 semanas

---

#### M04 · Visor de Log de Actividad
**Justificación**: La función `insertarLog` y `consultarLog` ya existen en `base_datos`, pero no hay ninguna ventana que muestre el log al administrador. Es una brecha de seguridad/auditoría.  
**Diseño**:
- Tabla con filtros por usuario, fecha, categoría
- Exportación a CSV
- Acceso restringido por permiso `log.ver`

**Archivos a crear**: `visorlog.cpp/.h/.ui`  
**Esfuerzo estimado**: 1 semana

---

### 🟡 MEDIA PRIORIDAD — Mejoras importantes

#### M05 · CRM Básico (Historial de Comunicaciones)
**Justificación**: Los TPV orientados a retail avanzado (Holded, Odoo) permiten registrar llamadas, incidencias y observaciones por cliente. Actualmente solo hay un campo de notas en la ficha.  
**Diseño**:
- Tabla `comunicaciones_cliente` (fecha, tipo, usuario, descripción)
- Pestaña "Historial" dentro del diálogo de clientes
- Tipos: llamada, visita, reclamación, incidencia, otro

**Archivos a crear**: `crmcliente.cpp/.h/.ui`  
**Esfuerzo estimado**: 1-2 semanas

---

#### M06 · Dashboard en Tiempo Real (Pantalla de inicio)
**Justificación**: La pantalla de inicio actual muestra solo botones. Un panel con KPIs del día (ventas del día, artículos bajo stock, tickets, arqueo pendiente) mejoraría enormemente la experiencia.  
**Diseño**:
- Widget incrustado en `tienda.ui` como panel lateral/inferior
- Datos: ventas hoy, nº tickets, caja actual, artículos bajo mínimo, notas pendientes
- Actualización automática cada X minutos (reutilizar mecanismo del SyncManager)

**Archivos a crear**: `dashboardwidget.cpp/.h`  
**Esfuerzo estimado**: 1-2 semanas

---

#### M07 · Gestión de Reparaciones / Servicios Técnicos
**Justificación**: Muchas tiendas de electrónica, informática o electrodomésticos gestionan también reparaciones. Ningún competidor directo en el segmento "local" lo incluye bien; sería un diferenciador importante.  
**Diseño**:
- Tabla `reparaciones` (id, cliente, descripción, estado, técnico, fecha_entrada, fecha_salida, precio)
- Estados: Pendiente → En reparación → Listo → Entregado
- Impresión de albarán de entrega
- Enlazado con TPV para cobro

**Archivos a crear**: `reparaciones.cpp/.h/.ui`, `imprimirreparacion.cpp/.h`  
**Esfuerzo estimado**: 3-4 semanas

---

#### M08 · Control de Stock Mínimo con Alertas y Sugerencia de Pedido
**Justificación**: La tabla de artículos probablemente ya tiene campos `stockMinimo` o `stockAlerta`. Se necesita un módulo que: detecte artículos bajo mínimo, sugiera pedidos automáticos y genere un borrador de pedido al proveedor habitual.  
**Diseño**:
- Listado de artículos bajo mínimo agrupado por proveedor
- Botón "Generar pedido sugerido" que crea borradores en `pedidos`
- Alerta visual en dashboard (M06) y en menú principal

**Archivos a crear**: `alertasstock.cpp/.h/.ui` o integrar en `stock.cpp`  
**Esfuerzo estimado**: 1-2 semanas

---

#### M09 · Exportación / Importación de Datos (CSV / Excel)
**Justificación**: Factusol y la mayoría permiten importar artículos y clientes desde Excel/CSV. Actualmente no hay forma de hacer importaciones masivas ni exportar datos para análisis externo.  
**Diseño**:
- Exportación CSV de artículos, clientes, ventas
- Importación masiva de artículos/clientes con validación y preview
- Uso de `QTextStream` (sin dependencias externas)

**Archivos a crear**: `importadorcsv.cpp/.h/.ui`, `exportadorcsv.cpp/.h`  
**Esfuerzo estimado**: 2 semanas

---

### 🟢 BAJA PRIORIDAD — Valor añadido futuro

#### M10 · Integración con Lectores de Peso / Balanza
**Justificación**: Para tiendas de alimentación o ferretería. Conexión RS-232/USB a balanza para captura de peso en TPV.  
**Esfuerzo estimado**: 2 semanas (depende del hardware)

---

#### M11 · TPV Web / Aplicación Móvil (Qt para Android)
**Justificación**: El proyecto ya se llama `AndroidStudioProjects/tienda` — podría aprovecharse una versión simplificada del TPV para tablet como segunda pantalla o TPV auxiliar.  
**Esfuerzo estimado**: Alto (4-8 semanas)

---

#### M12 · Módulo de Presupuestos
**Justificación**: Presupuestos a clientes antes de convertirse en factura, funcionalidad estándar en Holded/Odoo pero ausente aquí.  
**Esfuerzo estimado**: 2-3 semanas

---

#### M13 · Contabilidad Básica / Integración con SII
**Justificación**: Holded tiene contabilidad integrada. Para el mercado español, la integración con el SII (Suministro Inmediato de Información) es cada vez más requerida para empresas medianas.  
**Esfuerzo estimado**: Alto (depende de la complejidad fiscal)

---

## 4. Roadmap sugerido

```
Q3 2026  ──────────────────────────────────────────────────────────
  M04  Visor de Log de Actividad               [1 semana]
  M01  UI completa de Lotes / Trazabilidad     [2-3 semanas]
  M08  Alertas de stock mínimo                 [1-2 semanas]

Q4 2026  ──────────────────────────────────────────────────────────
  M03  Envío por email (SMTP)                  [1-2 semanas]
  M06  Dashboard en tiempo real                [1-2 semanas]
  M09  Exportación/Importación CSV             [2 semanas]

Q1 2027  ──────────────────────────────────────────────────────────
  M02  Fidelización / Puntos                   [2-3 semanas]
  M05  CRM Básico                              [1-2 semanas]
  M07  Gestión de Reparaciones                 [3-4 semanas]

Futuro   ──────────────────────────────────────────────────────────
  M10  Integración balanza
  M11  TPV móvil (Android/tablet)
  M12  Presupuestos
  M13  Contabilidad / SII
```

---

## 5. Resumen ejecutivo

**Fortalezas del programa actuales:**
- TPV robusto con encargos, vales, tickets pendientes y VeriFACTU nativo
- Sistema multi-tienda con sincronización bidireccional propia (algo excepcional en software local)
- Permisos granulares muy completos (80 permisos por rol)
- Gestión de caducidades por lotes (rara en la competencia local)
- 100% offline-first, sin dependencia de cloud externo

**Principales brechas vs. mercado:**
1. **No hay UI de lotes** (la lógica existe en BD pero no hay ventana)
2. **No hay fidelización de puntos** (solo vales manuales)
3. **No se pueden enviar documentos por email**
4. **El log existe en BD pero no tiene visor**
5. **No hay dashboard al abrir la aplicación**
6. **No hay importación/exportación masiva de datos**
