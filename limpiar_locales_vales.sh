#!/bin/bash
# =============================================================================
# limpiar_locales_vales.sh
# Elimina los vales duplicados en las bases de datos locales.
# Mantiene el registro con el idvales más alto.
# =============================================================================

USER="ladis"
PASS="meganizado72"
BD="tiendaNueva"

SQL_DEDUP='
-- Mostrar cantidad de vales antes de limpiar
SELECT CONCAT("Antes de limpiar: ", COUNT(*), " vales") AS estado FROM vales;

-- Eliminar duplicados, manteniendo el que tenga el idvales más alto
DELETE v1 FROM vales v1
INNER JOIN vales v2
    ON  v1.idCliente = v2.idCliente
    AND v1.fechaEmision = v2.fechaEmision
    AND v1.idvales < v2.idvales;

-- Mostrar cantidad de vales después de limpiar
SELECT CONCAT("Después de limpiar: ", COUNT(*), " vales") AS estado FROM vales;
'

run_sql() {
    local desc="$1"
    local host="$2"
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    echo "  ▶  Limpiando $desc ($host)"
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    mysql -h "$host" -u "$USER" -p"$PASS" "$BD" \
          --connect-timeout=5 \
          -e "$SQL_DEDUP" 2>&1
}

echo "============================================================"
echo "  Limpieza de duplicados locales — $(date '+%Y-%m-%d %H:%M:%S')"
echo "============================================================"

# 1. LOCAL (esta máquina)
run_sql "Local (nene/localhost)" "127.0.0.1"

# 2. EMEICJAC
run_sql "Emeicjac" "emeicjac.ddns.net"

# 3. CASABLANCA
run_sql "Casablanca" "casablanca.ddns.net"

# 4. CERVANTES
run_sql "Cervantes" "cervantes19.ddns.net"

echo ""
echo "Limpieza finalizada."
