#!/bin/bash
# =============================================================================
# migrar_vale_uuid.sh
# Añade la columna vale_uuid a la tabla vales en todas las tiendas,
# rellena los existentes con UUID y añade UNIQUE INDEX.
# También elimina duplicados de idvales en la nube si los hubiera.
#
# Tiendas:
#   localhost    → local (nene)
#   emeicjac.ddns.net  → Emeicjac  (maestra)
#   casablanca.ddns.net → Casablanca
#   cervantes19.ddns.net → Cervantes
#   cervantes19.ddns.net / nubeCervantes → NUBE
# =============================================================================

USER="ladis"
PASS="meganizado72"
BD="tiendaNueva"

SQL_MIGRACION='
-- 1. Añadir columna si no existe
ALTER TABLE `vales` ADD COLUMN IF NOT EXISTS
    vale_uuid CHAR(36) NOT NULL DEFAULT "";

-- 2. Rellenar con UUID() los vales que no lo tengan
UPDATE vales SET vale_uuid = UUID()
WHERE vale_uuid = "" OR vale_uuid IS NULL;

-- 3. Añadir índice único solo si no existe (evita error en segundas ejecuciones)
SET @idx := (
    SELECT COUNT(*)
    FROM information_schema.statistics
    WHERE table_schema = DATABASE()
      AND table_name   = "vales"
      AND index_name   = "uq_vale_uuid"
);
SET @sql := IF(@idx = 0,
    "ALTER TABLE vales ADD UNIQUE INDEX uq_vale_uuid (vale_uuid)",
    "SELECT \"Indice uq_vale_uuid ya existe, omitiendo.\"");
PREPARE stmt FROM @sql;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

SELECT CONCAT("OK: ", COUNT(*), " vales con uuid asignado") AS resultado
FROM vales WHERE vale_uuid != "";
'

# Script adicional SOLO para la NUBE: eliminar filas con idvales duplicado
# (guarda la más reciente de cada idvales, borra el resto)
SQL_DEDUP_NUBE='
-- Mostrar duplicados antes de borrar
SELECT idvales, COUNT(*) AS n
FROM vales
GROUP BY idvales
HAVING n > 1;

-- Eliminar duplicados manteniendo el registro de mayor idvales_rowid
-- (el que se insertó más tarde), conservando además el vale_uuid
DELETE v1 FROM vales v1
INNER JOIN vales v2
    ON  v1.idvales = v2.idvales
    AND v1.idCliente = v2.idCliente
    AND v1.fechaEmision = v2.fechaEmision
WHERE v1.vale_uuid < v2.vale_uuid;

SELECT CONCAT("Tras dedup: ", COUNT(*), " vales en la nube") AS resultado
FROM vales;
'

run_sql() {
    local desc="$1"
    local host="$2"
    local bd="$3"
    local sql="$4"
    echo ""
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    echo "  ▶  $desc  ($host / $bd)"
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    mysql -h "$host" -u "$USER" -p"$PASS" "$bd" \
          --connect-timeout=5 \
          -e "$sql" 2>&1
    if [ $? -eq 0 ]; then
        echo "  ✔  $desc completado"
    else
        echo "  ✘  Error al conectar con $desc — comprueba que está encendida"
    fi
}

echo "============================================================"
echo "  Migración vale_uuid — $(date '+%Y-%m-%d %H:%M:%S')"
echo "============================================================"

# 1. LOCAL (esta máquina)
run_sql "LOCAL (nene/localhost)" "127.0.0.1" "$BD" "$SQL_MIGRACION"

# 2. EMEICJAC (maestra)
run_sql "Emeicjac" "emeicjac.ddns.net" "$BD" "$SQL_MIGRACION"

# 3. CASABLANCA
run_sql "Casablanca" "casablanca.ddns.net" "$BD" "$SQL_MIGRACION"

# 4. CERVANTES
run_sql "Cervantes" "cervantes19.ddns.net" "$BD" "$SQL_MIGRACION"

# 5. NUBE (cervantes19.ddns.net / nubeCervantes) — primero dedup, luego migración
run_sql "NUBE — Deduplicar idvales" "cervantes19.ddns.net" "nubeCervantes" "$SQL_DEDUP_NUBE"
run_sql "NUBE — Añadir vale_uuid"   "cervantes19.ddns.net" "nubeCervantes" "$SQL_MIGRACION"

echo ""
echo "============================================================"
echo "  Migración completada — $(date '+%Y-%m-%d %H:%M:%S')"
echo "============================================================"
