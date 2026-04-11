-- Script de migración de Caducados a Historico_Stock
-- Este script mueve los registros de la tabla antigua 'caducados' a la nueva estructura de auditoría.

INSERT INTO historico_stock (
    ean, 
    lote, 
    fecha_caducidad_ant, 
    fecha_caducidad_new, 
    stock_ant, 
    stock_new, 
    motivo, 
    usuario, 
    fecha_hora
)
SELECT 
    cod,                          -- ean
    '',              -- lote (no existía en la tabla antigua)
    NULL,                         -- fecha_caducidad_ant
    fechaCaducidad ,                     -- fecha_caducidad_new
    0,                            -- stock_ant
    -cantidad,                    -- stock_new (cantidad negativa indica salida)
    'Cadicidad',  -- motivo
    'ADMIN',                      -- usuario
    fecha                         -- fecha de la operación original
FROM caducados;

-- Una vez verificado, puedes eliminar la tabla antigua:
-- DROP TABLE caducados;