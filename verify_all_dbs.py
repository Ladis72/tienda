import subprocess

def get_schema(host, user, password, db):
    # Columns
    cmd_cols = ["mariadb", "-h", host, "-u", user, f"-p{password}", "-D", db, "-e", 
           "SELECT TABLE_NAME, COLUMN_NAME, COLUMN_TYPE FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA='" + db + "'", 
           "--batch", "--column-names"]
    res_cols = subprocess.run(cmd_cols, capture_output=True, text=True)
    
    # Triggers
    cmd_trig = ["mariadb", "-h", host, "-u", user, f"-p{password}", "-D", db, "-e", 
           "SELECT TRIGGER_NAME, EVENT_MANIPULATION, EVENT_OBJECT_TABLE FROM INFORMATION_SCHEMA.TRIGGERS WHERE TRIGGER_SCHEMA='" + db + "'", 
           "--batch", "--column-names"]
    res_trig = subprocess.run(cmd_trig, capture_output=True, text=True)

    if res_cols.returncode != 0:
        return None
        
    schema = {'tables': {}, 'triggers': set()}
    
    # Parse columns
    lines = res_cols.stdout.strip().split('\n')
    if len(lines) > 1:
        headers = lines[0].split('\t')
        for line in lines[1:]:
            parts = line.split('\t')
            if len(parts) == 3:
                t, c, type_ = parts
                if t not in schema['tables']:
                    schema['tables'][t] = {}
                schema['tables'][t][c] = type_
                
    # Parse triggers
    if res_trig.returncode == 0:
        lines = res_trig.stdout.strip().split('\n')
        if len(lines) > 1:
            for line in lines[1:]:
                parts = line.split('\t')
                if len(parts) == 3:
                    schema['triggers'].add(parts[0]) # just trigger name is enough for comparison usually
                    
    return schema

dbs = {
    "Local": ("localhost", "root", "meganizado", "tiendaNueva"),
    "Casablanca": ("casablanca.ddns.net", "ladis", "meganizado72", "tiendaNueva"),
    "Cervantes": ("cervantes19.ddns.net", "ladis", "meganizado72", "tiendaNueva")
}

schemas = {}
for name, (h, u, p, d) in dbs.items():
    print(f"Fetching {name} schema...")
    sch = get_schema(h, u, p, d)
    if not sch:
        print(f"FAILED to fetch {name}")
    schemas[name] = sch

# Compare
def compare_dbs(name1, name2):
    print(f"\n--- Comparando {name1} vs {name2} ---")
    s1 = schemas.get(name1)
    s2 = schemas.get(name2)
    if not s1 or not s2:
        return

    # Tables
    t1 = set(s1['tables'].keys())
    t2 = set(s2['tables'].keys())
    
    if t1 != t2:
        only1 = t1 - t2
        only2 = t2 - t1
        if only1: print(f"Tablas solo en {name1}: {only1}")
        if only2: print(f"Tablas solo en {name2}: {only2}")
    
    common = t1 & t2
    diff_cols = False
    for t in sorted(common):
        c1 = set(s1['tables'][t].keys())
        c2 = set(s2['tables'][t].keys())
        if c1 != c2:
            print(f"Tabla {t}: Diferencia en columnas")
            only_c1 = c1 - c2
            only_c2 = c2 - c1
            if only_c1: print(f"  Solo en {name1}: {only_c1}")
            if only_c2: print(f"  Solo en {name2}: {only_c2}")
            diff_cols = True
        else:
            for c in c1:
                type1 = s1['tables'][t][c]
                type2 = s2['tables'][t][c]
                if type1 != type2:
                    print(f"Tabla {t}.{c}: {name1}={type1}, {name2}={type2}")
                    diff_cols = True
                    
    # Triggers
    tr1 = s1['triggers']
    tr2 = s2['triggers']
    if tr1 != tr2:
        print(f"\nDiferencias en Triggers:")
        only_tr1 = tr1 - tr2
        only_tr2 = tr2 - tr1
        if only_tr1: print(f"  Solo en {name1}: {only_tr1}")
        if only_tr2: print(f"  Solo en {name2}: {only_tr2}")
        
    if t1 == t2 and not diff_cols and tr1 == tr2:
        print(f"¡{name1} y {name2} son EXACTAMENTE IGUALES!")

compare_dbs("Local", "Casablanca")
compare_dbs("Local", "Cervantes")
compare_dbs("Casablanca", "Cervantes")
