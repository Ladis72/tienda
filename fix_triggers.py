import re

with open("equalize_casablanca.sql", "r") as f:
    content = f.read()

# Remove the DELIMITER $$ and DELIMITER ; lines
content = re.sub(r'DELIMITER \$\$\n+', '', content)
content = re.sub(r'DELIMITER ;\n*', '', content)

# Replace DROP TRIGGER IF EXISTS sync_xyz_insert$$ with ;
content = re.sub(r'(DROP TRIGGER IF EXISTS [a-zA-Z0-9_]+)\$\$', r'\1;', content)

# Replace the CREATE TRIGGER body
def replace_trigger(match):
    prefix = match.group(1) # CREATE TRIGGER ... AFTER ... ON ... FOR EACH ROW
    table = match.group(2)
    id_col = match.group(3)
    action = match.group(4)
    
    new_body = f"INSERT INTO sync_cola (tabla, id_registro, accion) SELECT '{table}', {id_col}, '{action}' FROM DUAL WHERE @skip_sync IS NULL OR @skip_sync = 0;"
    return f"{prefix}\n{new_body}"

# Pattern to match the trigger creation:
# CREATE TRIGGER sync_articulos_insert AFTER INSERT ON articulos FOR EACH ROW BEGIN IF @skip_sync IS NULL OR @skip_sync = 0 THEN INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('articulos', NEW.cod, 'INSERT'); END IF; END$$
pattern = r'(CREATE TRIGGER [a-zA-Z0-9_]+ AFTER (?:INSERT|UPDATE|DELETE) ON [a-zA-Z0-9_]+ FOR EACH ROW) BEGIN IF @skip_sync IS NULL OR @skip_sync = 0 THEN INSERT INTO sync_cola \(tabla, id_registro, accion\) VALUES \(\'([a-zA-Z0-9_]+)\', ([A-Z]+\.[a-zA-Z0-9_]+), \'([A-Z]+)\'\); END IF; END\$\$'

content = re.sub(pattern, replace_trigger, content)

# Replace the actStock triggers ending with $$
content = re.sub(r'(CREATE TRIGGER actStock[a-zA-Z]+ AFTER (?:INSERT|UPDATE) ON lotes\nFOR EACH ROW UPDATE articulos SET stock= \(SELECT sum\(cantidad\) FROM lotes WHERE ean = [A-Z]+\.ean\) where articulos\.cod = [A-Z]+\.ean)\$\$', r'\1;', content)

with open("equalize_casablanca.sql", "w") as f:
    f.write(content)

print("Triggers rewritten")
