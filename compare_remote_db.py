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
remote_schema = get_schema("emeicjac.ddns.net", "ladis", "meganizado72", "tiendaNueva")

with open("comparativa_emeicjac.md", "w") as f:
    f.write("# Comparativa de Estructura de BD: `tiendaNueva` Local vs `tiendaNueva` (Emeicjac)\n\n")
    
    if not local_schema:
        f.write("Error al conectar con la base de datos local.\n")
        exit()
    if not remote_schema:
        f.write("Error al conectar con la base de datos remota (Emeicjac).\n")
        exit()
        
    tables_local = set(local_schema.keys())
    tables_remote = set(remote_schema.keys())
    
    only_local = tables_local - tables_remote
    only_remote = tables_remote - tables_local
    common = tables_local & tables_remote
    
    if only_local:
        f.write("## Tablas solo en Local\n")
        for t in sorted(only_local):
            f.write(f"- `{t}`\n")
        f.write("\n")
        
    if only_remote:
        f.write("## Tablas solo en Emeicjac (Remoto)\n")
        for t in sorted(only_remote):
            f.write(f"- `{t}`\n")
        f.write("\n")
        
    f.write("## Diferencias en Tablas Comunes\n")
    diffs_found = False
    for t in sorted(common):
        cols_local = set(local_schema[t].keys())
        cols_remote = set(remote_schema[t].keys())
        
        only_local_cols = cols_local - cols_remote
        only_remote_cols = cols_remote - cols_local
        common_cols = cols_local & cols_remote
        
        table_diff = []
        if only_local_cols:
            table_diff.append(f"Columnas solo en Local: " + ", ".join(f"`{c}`" for c in sorted(only_local_cols)))
        if only_remote_cols:
            table_diff.append(f"Columnas solo en Emeicjac: " + ", ".join(f"`{c}`" for c in sorted(only_remote_cols)))
            
        for c in sorted(common_cols):
            type_local = local_schema[t][c]['COLUMN_TYPE']
            type_remote = remote_schema[t][c]['COLUMN_TYPE']
            if type_local != type_remote:
                table_diff.append(f"Columna `{c}` difiere en tipo: Local -> `{type_local}`, Emeicjac -> `{type_remote}`")
                
        if table_diff:
            diffs_found = True
            f.write(f"### Tabla `{t}`\n")
            for d in table_diff:
                f.write(f"- {d}\n")
            f.write("\n")
            
    if not diffs_found:
        f.write("No hay diferencias en la estructura de las tablas comunes.\n")

print("Comparativa generada en comparativa_emeicjac.md")
