import subprocess
import json

def get_schema(host, user, password, db):
    cmd = ["mariadb", "-h", host, "-u", user, f"-p{password}", "-D", db, "-e", 
           "SELECT TABLE_NAME, COLUMN_NAME, COLUMN_TYPE, COLUMN_KEY, EXTRA FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA='" + db + "'", 
           "--batch", "--column-names"]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        return None
    lines = result.stdout.strip().split('\n')
    if not lines or len(lines) == 1:
        return {}
    
    schema = {}
    headers = lines[0].split('\t')
    for line in lines[1:]:
        parts = line.split('\t')
        row = dict(zip(headers, parts))
        t = row['TABLE_NAME']
        if t not in schema:
            schema[t] = {}
        schema[t][row['COLUMN_NAME']] = row
    return schema

local_schema = get_schema("localhost", "root", "meganizado", "tiendaNueva")
remote_schema = get_schema("casablanca.ddns.net", "ladis", "meganizado72", "tiendaNueva")

if not local_schema:
    print("Error connecting to local DB")
    exit(1)
if not remote_schema:
    print("Error connecting to Casablanca DB")
    exit(1)

# Exclusions managed by tienda.cpp
excluded_tables = {'encargos', 'historico_stock', 'proveedores', 'precios_tienda'}
excluded_columns = {('configuracion', 'precios_locales')}

tables_local = set(local_schema.keys())
tables_remote = set(remote_schema.keys())

only_local = tables_local - tables_remote
only_remote = tables_remote - tables_local
common = tables_local & tables_remote

sql_commands = []

sql_commands.append("-- SCRIPT DE MIGRACIÓN: Casablanca a Local")
sql_commands.append("-- Excluyendo tablas/columnas autogeneradas por el código C++ en tienda.cpp\n")

for t in sorted(only_local):
    if t not in excluded_tables:
        sql_commands.append(f"-- ATENCIÓN: La tabla '{t}' no existe en Casablanca.")
        sql_commands.append(f"-- Generando estructura básica para '{t}' (Ajustar si es necesario)")
        cmd = ["mariadb-dump", "-u", "root", "-pmeganizado", "--no-data", "--skip-opt", "--skip-comments", "tiendaNueva", t]
        res = subprocess.run(cmd, capture_output=True, text=True)
        if res.returncode == 0:
            for line in res.stdout.split('\n'):
                if line.startswith("CREATE TABLE") or line.startswith("  `") or line.startswith(") ENGINE"):
                    sql_commands.append(line)
            sql_commands.append(";")
        sql_commands.append("")

for t in sorted(only_remote):
    sql_commands.append(f"-- ATENCIÓN: La tabla '{t}' existe en Casablanca pero NO en local.")
    sql_commands.append(f"-- DROP TABLE IF EXISTS `{t}`; \n")

for t in sorted(common):
    cols_local = set(local_schema[t].keys())
    cols_remote = set(remote_schema[t].keys())
    
    only_local_cols = cols_local - cols_remote
    only_remote_cols = cols_remote - cols_local
    common_cols = cols_local & cols_remote
    
    for c in sorted(only_local_cols):
        if (t, c) not in excluded_columns:
            col_def = local_schema[t][c]
            sql_commands.append(f"ALTER TABLE `{t}` ADD COLUMN `{c}` {col_def['COLUMN_TYPE']};")
            
    for c in sorted(only_remote_cols):
        sql_commands.append(f"ALTER TABLE `{t}` DROP COLUMN `{c}`;")
        
    for c in sorted(common_cols):
        type_local = local_schema[t][c]['COLUMN_TYPE']
        type_remote = remote_schema[t][c]['COLUMN_TYPE']
        if type_local != type_remote:
            sql_commands.append(f"ALTER TABLE `{t}` MODIFY COLUMN `{c}` {type_local};")

with open("equalize_casablanca.sql", "w") as f:
    f.write("\n".join(sql_commands))

print("Script generated at equalize_casablanca.sql")
