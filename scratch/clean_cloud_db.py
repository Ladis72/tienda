
import mysql.connector

# Cloud database credentials
config = {
    'user': 'ladis',
    'password': 'meganizado72',
    'host': 'cervantes19.ddns.net',
    'database': 'nubeCervantes',
    'port': 3306
}

try:
    conn = mysql.connector.connect(**config)
    cursor = conn.cursor()

    # 1. Fix fpago duplicate
    print("Fixing fpago duplicate...")
    cursor.execute("DELETE FROM fpago WHERE id = 4 AND efectivo = 1 AND id_tienda_origen IS NULL")
    print(f"Removed {cursor.rowcount} duplicate records from fpago.")

    # 2. Get all tables
    cursor.execute("SHOW TABLES")
    tables = [row[0] for row in cursor.fetchall()]

    for table in tables:
        print(f"Cleaning table: {table}")
        
        # Add Primary Key if missing and if we know what it should be
        # Based on SyncManager::getPkTabla
        pk_map = {
            "articulos": "cod",
            "clientes": "idCliente",
            "familias": "id",
            "fabricantes": "id",
            "proveedores": "idProveedor",
            "codaux": "id",
            "fpago": "id",
            "impuestos": "tipoIva",
            "formatos": "idformato",
            "motivosEntrada": "idtiposEntrada",
            "usuarios": "id",
            "permisos": "id",
            "vales": "vale_uuid",
            "directorios": "id"
        }
        
        if table in pk_map:
            pk_col = pk_map[table]
            # Check if PK exists
            cursor.execute(f"SHOW KEYS FROM `{table}` WHERE Key_name = 'PRIMARY'")
            if not cursor.fetchone():
                print(f"  Adding PRIMARY KEY ({pk_col}) to {table}")
                try:
                    # For vales, vale_uuid is CHAR(36), so it's fine.
                    # For others, check if there are nulls or duplicates first? 
                    # We'll just try to add it.
                    cursor.execute(f"ALTER TABLE `{table}` ADD PRIMARY KEY (`{pk_col}`)")
                except Exception as e:
                    print(f"  Error adding PK to {table}: {e}")

        # Clean up redundant updated_at indexes
        cursor.execute(f"""
            SELECT index_name 
            FROM information_schema.statistics 
            WHERE table_schema = '{config['database']}' 
              AND table_name = '{table}' 
              AND column_name = 'updated_at'
              AND index_name != 'PRIMARY'
        """)
        indexes = [row[0] for row in cursor.fetchall()]
        
        if len(indexes) > 1:
            # Keep only the one named 'updated_at' if it exists, or the first one
            to_keep = 'updated_at' if 'updated_at' in indexes else indexes[0]
            for idx in indexes:
                if idx != to_keep:
                    print(f"  Dropping redundant index {idx} from {table}")
                    cursor.execute(f"ALTER TABLE `{table}` DROP INDEX `{idx}`")

    conn.commit()
    cursor.close()
    conn.close()
    print("Cleanup complete.")

except Exception as e:
    print(f"Error: {e}")
