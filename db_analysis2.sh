#!/bin/bash
DB="tiendaNueva"
USER="root"
PASS="meganizado"
MYSQL="mariadb -u $USER -p$PASS -D $DB -t -c"

echo "=== DATABASE ANALYSIS 2 ==="
echo ""
echo "1. Checking for orphaned ticket lines (lineasticket without ticket)"
$MYSQL -e "SELECT COUNT(*) AS OrphanedLineasTicket FROM lineasticket LEFT JOIN tickets ON lineasticket.nticket = tickets.ticket WHERE tickets.ticket IS NULL AND lineasticket.nticket IS NOT NULL;"

echo ""
echo "2. Checking for duplicate EANs in articulos"
$MYSQL -e "SELECT cod, COUNT(*) as c FROM articulos GROUP BY cod HAVING c > 1;"

echo ""
echo "3. Comparing stock in articulos vs lotes"
$MYSQL -e "SELECT a.cod, a.stock as stock_articulos, SUM(l.cantidad) as stock_lotes FROM articulos a JOIN lotes l ON a.cod = l.ean GROUP BY a.cod, a.stock HAVING a.stock != SUM(l.cantidad) LIMIT 10;"

echo ""
echo "4. Checking vales sin cliente"
$MYSQL -e "SELECT COUNT(*) FROM vales LEFT JOIN clientes ON vales.idCliente = clientes.id_cliente WHERE clientes.id_cliente IS NULL AND vales.idCliente != 0;"

echo ""
echo "5. Checking for orphaned encargos"
$MYSQL -e "SELECT COUNT(*) FROM encargos LEFT JOIN clientes ON encargos.id_cliente = clientes.id_cliente WHERE clientes.id_cliente IS NULL AND encargos.id_cliente != 0;"

echo ""
echo "6. Checking data anomalies: prices less than 0"
$MYSQL -e "SELECT cod, pvp FROM articulos WHERE pvp < 0 LIMIT 5;"

echo ""
echo "7. Checking for ticket anomalies: total != base + iva"
$MYSQL -e "SELECT ticket, base, iva, total FROM tickets WHERE ABS(total - (base + iva)) > 0.1 LIMIT 5;"

