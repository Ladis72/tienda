import subprocess
import json
import sys

def get_schema(db):
    cmd = ["mariadb", "-u", "root", "-pmeganizado", "-D", db, "-e", 
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

db1 = "tienda"
db2 = "tiendaNueva"

s1 = get_schema(db1)
s2 = get_schema(db2)

with open("comparativa.md", "w") as f:
    f.write(f"# Comparativa de Base de Datos: `{db1}` (Local) vs `{db2}` (Emeicjac)\n\n")
    
    tables1 = set(s1.keys())
    tables2 = set(s2.keys())
    
    only_in_1 = tables1 - tables2
    only_in_2 = tables2 - tables1
    common = tables1 & tables2
    
    if only_in_1:
        f.write(f"## Tablas solo en `{db1}`\n")
        for t in sorted(only_in_1):
            f.write(f"- `{t}`\n")
        f.write("\n")
        
    if only_in_2:
        f.write(f"## Tablas solo en `{db2}`\n")
        for t in sorted(only_in_2):
            f.write(f"- `{t}`\n")
        f.write("\n")
        
    f.write("## Diferencias en Tablas Comunes\n")
    diffs_found = False
    for t in sorted(common):
        cols1 = set(s1[t].keys())
        cols2 = set(s2[t].keys())
        
        only_cols1 = cols1 - cols2
        only_cols2 = cols2 - cols1
        common_cols = cols1 & cols2
        
        table_diff = []
        if only_cols1:
            table_diff.append(f"Columnas solo en `{db1}`: " + ", ".join(f"`{c}`" for c in sorted(only_cols1)))
        if only_cols2:
            table_diff.append(f"Columnas solo en `{db2}`: " + ", ".join(f"`{c}`" for c in sorted(only_cols2)))
            
        for c in sorted(common_cols):
            type1 = s1[t][c]['COLUMN_TYPE']
            type2 = s2[t][c]['COLUMN_TYPE']
            if type1 != type2:
                table_diff.append(f"Columna `{c}` difiere en tipo: `{db1}` -> `{type1}`, `{db2}` -> `{type2}`")
                
        if table_diff:
            diffs_found = True
            f.write(f"### Tabla `{t}`\n")
            for d in table_diff:
                f.write(f"- {d}\n")
            f.write("\n")
            
    if not diffs_found:
        f.write("No hay diferencias en la estructura de las tablas comunes.\n")

print("Comparativa generada en comparativa.md")
