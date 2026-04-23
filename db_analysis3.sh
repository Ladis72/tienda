#!/bin/bash
DB="tiendaNueva"
USER="root"
PASS="meganizado"
MYSQL="mariadb -u $USER -p$PASS -D $DB -t -c"

echo "=== DATABASE ANALYSIS 3 ==="

echo ""
echo "1. Structure of Clientes"
$MYSQL -e "DESCRIBE clientes;"

echo ""
echo "2. Valuar orphaned encargos and vales con idCliente correcto"
$MYSQL -e "SELECT COUNT(*) FROM vales LEFT JOIN clientes ON vales.idCliente = clientes.id WHERE clientes.id IS NULL AND vales.idCliente != 0;"
$MYSQL -e "SELECT COUNT(*) FROM encargos LEFT JOIN clientes ON encargos.id_cliente = clientes.id WHERE clientes.id IS NULL AND encargos.id_cliente != 0;"

echo ""
echo "3. Tickets where base and iva are 0 but total > 0"
$MYSQL -e "SELECT COUNT(*) as TicketsWithoutBaseIva FROM tickets WHERE base = 0 AND iva = 0 AND total > 0;"

echo ""
echo "4. How many total tickets?"
$MYSQL -e "SELECT COUNT(*) as TotalTickets FROM tickets;"

echo ""
echo "5. Let's look at tickets with base and iva = 0"
$MYSQL -e "SELECT ticket, base, iva, total, fecha FROM tickets WHERE base = 0 AND iva = 0 AND total > 0 LIMIT 5;"

