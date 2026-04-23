#!/bin/bash
mariadb-dump -u root -pmeganizado --no-data --skip-opt --skip-comments tiendaNube > tiendaNube_schema.sql
mariadb-dump -u root -pmeganizado --no-data --skip-opt --skip-comments tiendaNueva > tiendaNueva_schema.sql
