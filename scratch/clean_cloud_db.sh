#!/bin/bash

# Cloud database credentials
HOST="cervantes19.ddns.net"
USER="ladis"
PASS="meganizado72"
DB="nubeCervantes"

MYSQL="mysql -h $HOST -P 3306 -u $USER -p$PASS $DB -sN -e"

echo "Fixing fpago duplicate..."
$MYSQL "DELETE FROM fpago WHERE id = 4 AND efectivo = 1 AND id_tienda_origen IS NULL"

TABLES=$($MYSQL "SHOW TABLES")

for TABLE in $TABLES; do
    echo "Processing table: $TABLE"
    
    # Check if Primary Key exists
    PK_EXISTS=$($MYSQL "SHOW KEYS FROM \`$TABLE\` WHERE Key_name = 'PRIMARY'")
    if [ -z "$PK_EXISTS" ]; then
        PK_COL=""
        case $TABLE in
            articulos) PK_COL="cod" ;;
            clientes) PK_COL="idCliente" ;;
            familias|fabricantes|usuarios|permisos|directorios) PK_COL="id" ;;
            proveedores) PK_COL="idProveedor" ;;
            codaux|fpago) PK_COL="id" ;;
            impuestos) PK_COL="tipoIva" ;;
            formatos) PK_COL="idformato" ;;
            motivosEntrada) PK_COL="idtiposEntrada" ;;
            vales) PK_COL="vale_uuid" ;;
        esac
        
        if [ -n "$PK_COL" ]; then
            echo "  Adding PRIMARY KEY ($PK_COL) to $TABLE"
            $MYSQL "ALTER TABLE \`$TABLE\` ADD PRIMARY KEY (\`$PK_COL\`)"
        fi
    fi

    # Clean redundant updated_at indexes
    INDEXES=$($MYSQL "SELECT index_name FROM information_schema.statistics WHERE table_schema = '$DB' AND table_name = '$TABLE' AND column_name = 'updated_at' AND index_name != 'PRIMARY'")
    
    COUNT=$(echo "$INDEXES" | wc -w)
    if [ "$COUNT" -gt 1 ]; then
        KEEP="updated_at"
        # Check if "updated_at" is one of them
        if ! echo "$INDEXES" | grep -q "^updated_at$"; then
            KEEP=$(echo "$INDEXES" | head -n 1)
        fi
        
        for IDX in $INDEXES; do
            if [ "$IDX" != "$KEEP" ]; then
                echo "  Dropping redundant index $IDX from $TABLE"
                $MYSQL "ALTER TABLE \`$TABLE\` DROP INDEX \`$IDX\`"
            fi
        done
    fi
done

echo "Cleanup complete."
