import subprocess
import json

def run_query(query):
    cmd = ["mariadb", "-u", "root", "-pmeganizado", "-D", "tiendaNueva", "-e", query, "--batch", "--column-names"]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Error running query: {result.stderr}")
        return []
    lines = result.stdout.strip().split('\n')
    if not lines:
        return []
    headers = lines[0].split('\t')
    data = []
    for line in lines[1:]:
        data.append(dict(zip(headers, line.split('\t'))))
    return data

# Get all indexes
query = """
SELECT 
    TABLE_NAME, 
    INDEX_NAME, 
    NON_UNIQUE,
    GROUP_CONCAT(COLUMN_NAME ORDER BY SEQ_IN_INDEX) AS COLUMNS
FROM 
    INFORMATION_SCHEMA.STATISTICS 
WHERE 
    TABLE_SCHEMA = 'tiendaNueva'
GROUP BY 
    TABLE_NAME, INDEX_NAME;
"""

indexes = run_query(query)

# Group by table and columns
duplicates = {}
for idx in indexes:
    table = idx['TABLE_NAME']
    cols = idx['COLUMNS']
    key = (table, cols)
    if key not in duplicates:
        duplicates[key] = []
    duplicates[key].append(idx)

drop_queries = []
for (table, cols), idx_list in duplicates.items():
    if len(idx_list) > 1:
        # We have duplicates. Decide which one to keep.
        # Primary key first, then shortest name or name without numbers
        idx_list.sort(key=lambda x: (x['INDEX_NAME'] != 'PRIMARY', len(x['INDEX_NAME']), x['INDEX_NAME']))
        
        keep = idx_list[0]
        to_drop = idx_list[1:]
        
        for d in to_drop:
            # Only drop if it's not the PRIMARY key (just in case)
            if d['INDEX_NAME'] != 'PRIMARY':
                drop_queries.append(f"ALTER TABLE `{table}` DROP INDEX `{d['INDEX_NAME']}`;")

if not drop_queries:
    print("No duplicate indexes found.")
else:
    with open("drop_duplicates.sql", "w") as f:
        f.write("\n".join(drop_queries))
    print(f"Found {len(drop_queries)} duplicate indexes. Executing drop_duplicates.sql...")
    subprocess.run(["mariadb", "-u", "root", "-pmeganizado", "-D", "tiendaNueva"], input="\n".join(drop_queries), text=True)
    print("Cleanup complete.")

