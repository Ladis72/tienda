import subprocess
import json

def run_query(db, query):
    cmd = ["mariadb", "-u", "root", "-pmeganizado", "-D", db, "-e", query, "--batch", "--column-names"]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        return f"Error: {result.stderr}"
    return result.stdout.strip()

print("--- tiendaNueva.tiendas ---")
print(run_query("tiendaNueva", "SELECT id, nombre, ip, local, master, baseDatos FROM tiendas;"))

print("\n--- tiendaNube.tiendas ---")
print(run_query("tiendaNube", "SELECT id, nombre, ip, baseDatos FROM tiendas;"))

print("\n--- configuracion (tiendaNueva) ---")
print(run_query("tiendaNueva", "SELECT * FROM configuracion LIMIT 1;"))
