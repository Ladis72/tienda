-- Fix arqueos typo (rename column if exists)
SET @exist = (SELECT COUNT(*) FROM information_schema.columns WHERE table_schema = 'tiendaNueva' AND table_name = 'arqueos' AND column_name = 'ususario');
SET @s = IF(@exist > 0, 'ALTER TABLE arqueos RENAME COLUMN ususario TO usuario;', 'SELECT 1;');
PREPARE stmt FROM @s;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

-- Drop nube_comunes if exists from configuracion
SET @exist = (SELECT COUNT(*) FROM information_schema.columns WHERE table_schema = 'tiendaNueva' AND table_name = 'configuracion' AND column_name = 'nube_comunes');
SET @s = IF(@exist > 0, 'ALTER TABLE configuracion DROP COLUMN nube_comunes;', 'SELECT 1;');
PREPARE stmt FROM @s;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

-- Drop pendientes if exists from sync_control
SET @exist = (SELECT COUNT(*) FROM information_schema.columns WHERE table_schema = 'tiendaNueva' AND table_name = 'sync_control' AND column_name = 'pendientes');
SET @s = IF(@exist > 0, 'ALTER TABLE sync_control DROP COLUMN pendientes;', 'SELECT 1;');
PREPARE stmt FROM @s;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

-- Fix verifactu_logs
ALTER TABLE verifactu_logs ADD PRIMARY KEY (id);
ALTER TABLE verifactu_logs MODIFY COLUMN id int(10) unsigned NOT NULL AUTO_INCREMENT;
ALTER TABLE verifactu_logs MODIFY COLUMN id_factura int(10) unsigned NOT NULL;
