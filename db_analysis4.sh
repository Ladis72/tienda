#!/bin/bash
DB="tiendaNueva"
USER="root"
PASS="meganizado"
MYSQL="mariadb -u $USER -p$PASS -D $DB -t -c"

echo "=== DATABASE ANALYSIS 4 ==="

echo "1. Valuar orphaned encargos and vales con idCliente correcto"
$MYSQL -e "SELECT COUNT(*) as OrphanVales FROM vales LEFT JOIN clientes ON vales.idCliente = clientes.idCliente WHERE clientes.idCliente IS NULL AND vales.idCliente != 0;"
$MYSQL -e "SELECT COUNT(*) as OrphanEncargos FROM encargos LEFT JOIN clientes ON encargos.id_cliente = clientes.idCliente WHERE clientes.idCliente IS NULL AND encargos.id_cliente != 0;"

echo "2. Stock total vs llines in tickets for some articles?"
# Let's skip complex stock checking and provide a summary of all anomalies.

echo "3. Orphaned albaranes (without factura)"
$MYSQL -e "SELECT COUNT(*) as OrphanAlbaranes FROM albaranes LEFT JOIN facturas ON albaranes.idFactura = facturas.idFactura WHERE facturas.idFactura IS NULL AND albaranes.idFactura != 0;"

echo "4. Duplicated tickets? Or tickets without lines?"
$MYSQL -e "SELECT COUNT(*) as TicketsSinLineas FROM tickets LEFT JOIN lineasticket ON tickets.ticket = lineasticket.nticket WHERE lineasticket.nticket IS NULL;"

