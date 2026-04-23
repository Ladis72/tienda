import sys

tables = [
    ("articulos", "cod"),
    ("clientes", "idCliente"),
    ("codaux", "id"),
    ("fabricantes", "id"),
    ("familias", "id"),
    ("formatos", "idformato"),
    ("fpago", "id"),
    ("impuestos", "tipoIva"),
    ("motivosEntrada", "idtiposEntrada"),
    ("permisos", "id"),
    ("proveedores", "idProveedor"),
    ("usuarios", "id"),
    ("vales", "idvales"),
    ("verifactu_logs", "id")
]

with open("equalize_casablanca.sql", "a") as f:
    f.write("\n\n-- TRIGGERS DE SINCRONIZACIÓN (Generados automáticamente)\n")
    f.write("DELIMITER $$\n\n")
    
    for table, id_col in tables:
        f.write(f"DROP TRIGGER IF EXISTS sync_{table}_insert$$\n")
        f.write(f"CREATE TRIGGER sync_{table}_insert AFTER INSERT ON {table} FOR EACH ROW BEGIN IF @skip_sync IS NULL OR @skip_sync = 0 THEN INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('{table}', NEW.{id_col}, 'INSERT'); END IF; END$$\n\n")
        
        f.write(f"DROP TRIGGER IF EXISTS sync_{table}_update$$\n")
        f.write(f"CREATE TRIGGER sync_{table}_update AFTER UPDATE ON {table} FOR EACH ROW BEGIN IF @skip_sync IS NULL OR @skip_sync = 0 THEN INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('{table}', NEW.{id_col}, 'UPDATE'); END IF; END$$\n\n")
        
        f.write(f"DROP TRIGGER IF EXISTS sync_{table}_delete$$\n")
        f.write(f"CREATE TRIGGER sync_{table}_delete AFTER DELETE ON {table} FOR EACH ROW BEGIN IF @skip_sync IS NULL OR @skip_sync = 0 THEN INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('{table}', OLD.{id_col}, 'DELETE'); END IF; END$$\n\n")
        
    f.write("DELIMITER ;\n")

print("Triggers appended to equalize_casablanca.sql")
