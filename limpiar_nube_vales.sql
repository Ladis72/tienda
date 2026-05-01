-- ============================================================
-- SCRIPT: limpiar_nube_vales.sql
-- Ejecutar en la BD de la NUBE (nubeCervantes)
-- 1. Borra los 63 índices duplicados de updated_at
-- 2. Añade vale_uuid y lo rellena
-- 3. Elimina filas con idvales duplicado
-- ============================================================

-- PASO 1: Borrar índices duplicados de updated_at en la tabla vales
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_2;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_3;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_4;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_5;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_6;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_7;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_8;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_9;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_10;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_11;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_12;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_13;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_14;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_15;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_16;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_17;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_18;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_19;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_20;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_21;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_22;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_23;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_24;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_25;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_26;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_27;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_28;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_29;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_30;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_31;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_32;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_33;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_34;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_35;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_36;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_37;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_38;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_39;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_40;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_41;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_42;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_43;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_44;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_45;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_46;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_47;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_48;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_49;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_50;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_51;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_52;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_53;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_54;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_55;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_56;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_57;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_58;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_59;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_60;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_61;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_62;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_63;
ALTER TABLE vales DROP INDEX IF EXISTS updated_at_64;

SELECT 'PASO 1 OK: índices duplicados de updated_at eliminados' AS resultado;

-- PASO 2: Añadir vale_uuid y rellenar
ALTER TABLE vales ADD COLUMN IF NOT EXISTS vale_uuid CHAR(36) NOT NULL DEFAULT '';
UPDATE vales SET vale_uuid = UUID() WHERE vale_uuid = '' OR vale_uuid IS NULL;
ALTER TABLE vales ADD UNIQUE INDEX uq_vale_uuid (vale_uuid);

SELECT CONCAT('PASO 2 OK: vale_uuid asignado a ', COUNT(*), ' vales') AS resultado
FROM vales WHERE vale_uuid != '';

-- PASO 3: Eliminar duplicados de idvales
-- (conserva el registro con mayor vale_uuid lexicográfico)
DELETE v1 FROM vales v1
INNER JOIN vales v2
  ON  v1.idvales      = v2.idvales
  AND v1.fechaEmision  = v2.fechaEmision
  AND v1.idCliente    = v2.idCliente
WHERE v1.vale_uuid < v2.vale_uuid;

SELECT CONCAT('PASO 3 OK: quedan ', COUNT(*), ' vales en la nube tras eliminar duplicados') AS resultado
FROM vales;
