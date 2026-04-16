#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Script para propagar la descripción de un artículo por todas las tablas
relacionadas en la base de datos de la tienda.

Uso:
  python3 propagar_descripcion.py "CODIGO_ARTICULO" "NUEVA DESCRIPCION"
  
También puedes configurar la conexión:
  python3 propagar_descripcion.py "CODIGO" "DESC" --host 127.0.0.1 --user root --password mypass --db tiendaNueva
"""

import argparse
# Intentamos importar pymysql (compatible con interactuar en MariaDB/MySQL)
try:
    import pymysql
except ImportError:
    print("[!] Error: La librería 'pymysql' no está instalada.")
    print("    Puedes instalarla usando: pip install pymysql")
    exit(1)

def propagar_descripcion(host, user, password, database, port, cod, descripcion):
    # Lista de dependencias extraída de baseDatos::propagarCambioCodigoArticulo
    dependencias = [
        # (Nombre de tabla, Nombre columna código, Nombre columna descripción)
        ("articulos", "cod", "descripcion"),
        ("lineasticket", "cod", "descripcion"),
        ("lineaspedido", "cod", "descripcion"),
        ("lineaspedido_tmp", "cod", "descripcion"),
        ("entradaGenero_tmp", "cod", "descripcion"),
        ("salidaGenero_tmp", "cod", "descripcion"),
        ("lineasticket_tmp", "cod", "descripcion"),
        ("salidaGenero", "cod", "descripcion")
    ]

    try:
        # Nos conectamos a la base de datos
        conexion = pymysql.connect(
            host=host,
            user=user,
            password=password,
            database=database,
            port=port,
            autocommit=False # Controlamos la transacción manualmente
        )
        
        with conexion.cursor() as cursor:
            # Desactivamos comprobaciones de claves foráneas por si acaso
            cursor.execute("SET FOREIGN_KEY_CHECKS = 0;")
            
            print(f"[*] Iniciando propagación para el artículo: '{cod}'")
            print(f"[*] Nueva descripción a aplicar: '{descripcion}'")
            print("-" * 50)
            
            total_actualizadas = 0
            
            for tabla, col_cod, col_desc in dependencias:
                sql = f"UPDATE {tabla} SET {col_desc} = %s WHERE {col_cod} = %s"
                
                try:
                    cursor.execute(sql, (descripcion, cod))
                    filas = cursor.rowcount
                    print(f"  [+] Tabla '{tabla}': {filas} registros actualizados.")
                    total_actualizadas += filas
                except pymysql.MySQLError as e:
                    print(f"  [!] Error al actualizar la tabla '{tabla}': {e}")
                    print("  [!] Revirtiendo cambios (Rollback)...")
                    conexion.rollback()
                    cursor.execute("SET FOREIGN_KEY_CHECKS = 1;")
                    return False
            
            # Volvemos a activar comprobaciones y aplicamos transacion
            cursor.execute("SET FOREIGN_KEY_CHECKS = 1;")
            conexion.commit()
            
            print("-" * 50)
            print(f"[*] Propagación completada. Total de registros modificados: {total_actualizadas}")
            return True

    except pymysql.MySQLError as e:
        print(f"[!] Error de conexión a la base de datos: {e}")
        return False
    finally:
        if 'conexion' in locals() and conexion.open:
            conexion.close()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Propagar nueva descripción de artículo en base de datos Tienda.')
    parser.add_argument('cod', type=str, help='Código del artículo (ej. EAN)')
    parser.add_argument('descripcion', type=str, help='Nueva descripción del artículo')
    
    # Parámetros opcionales para la base de datos (por defecto los de conexion.h locales)
    parser.add_argument('--host', type=str, default='localhost', help='Host de la base de datos (defecto: localhost)')
    parser.add_argument('--user', type=str, default='root', help='Usuario de la base de datos (defecto: root)')
    parser.add_argument('--password', type=str, default='meganizado', help='Contraseña de la base de datos (defecto: meganizado)')
    parser.add_argument('--db', type=str, default='tiendaNueva', help='Nombre de la base de datos (defecto: tiendaNueva)')
    parser.add_argument('--port', type=int, default=3306, help='Puerto de la base de datos (defecto: 3306)')
    
    args = parser.parse_args()
    
    propagar_descripcion(
        host=args.host,
        user=args.user,
        password=args.password,
        database=args.db,
        port=args.port,
        cod=args.cod,
        descripcion=args.descripcion
    )
