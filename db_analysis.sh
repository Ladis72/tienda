#!/bin/bash
DB="tiendaNueva"
USER="root"
PASS="meganizado"
MYSQL="mariadb -u $USER -p$PASS -D $DB -t -c"

echo "=== DATABASE ANALYSIS ==="
echo ""
echo "1. Checking for missing indexes on foreign keys"
$MYSQL -e "SELECT TABLE_NAME, COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA='$DB' AND COLUMN_NAME LIKE 'id_%' AND COLUMN_NAME NOT IN (SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.STATISTICS WHERE TABLE_SCHEMA='$DB');"

echo ""
echo "2. Checking for orphaned ticket lines (lineasticket without ticket)"
$MYSQL -e "SELECT COUNT(*) AS OrphanedLineasTicket FROM lineasticket LEFT JOIN tickets ON lineasticket.idTicket = tickets.id WHERE tickets.id IS NULL AND lineasticket.idTicket IS NOT NULL;"

echo ""
echo "3. Checking for orphaned lotes (lotes without articulo)"
$MYSQL -e "SELECT COUNT(*) AS OrphanedLotes FROM lotes LEFT JOIN articulos ON lotes.ean = articulos.cod WHERE articulos.cod IS NULL;"

echo ""
echo "4. Checking for orphaned precios_tienda"
$MYSQL -e "SELECT COUNT(*) AS OrphanedPreciosTienda FROM precios_tienda LEFT JOIN articulos ON precios_tienda.cod_articulo = articulos.cod WHERE articulos.cod IS NULL;"

echo ""
echo "5. Checking for negative stock in articulos"
$MYSQL -e "SELECT COUNT(*) AS NegativeStock FROM articulos WHERE stock < 0;"

echo ""
echo "6. Checking for empty or null critical fields in articulos"
$MYSQL -e "SELECT COUNT(*) AS EmptyDescriptions FROM articulos WHERE descripcion IS NULL OR descripcion = '';"

echo ""
echo "7. Checking for duplicate nif in clientes"
$MYSQL -e "SELECT nif, COUNT(*) as c FROM clientes WHERE nif IS NOT NULL AND nif != '' GROUP BY nif HAVING c > 1;"

echo ""
echo "8. Checking for duplicate nif in proveedores"
$MYSQL -e "SELECT nif, COUNT(*) as c FROM proveedores WHERE nif IS NOT NULL AND nif != '' GROUP BY nif HAVING c > 1;"

