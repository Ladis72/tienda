-- SCRIPT PARA CAMBIAR EL ID DE USUARIO 1 POR EL 4 EN TODAS LAS TABLAS
-- Esto corrige el problema de sincronización donde las operaciones de Silvia (id=4)
-- se registraron como Ladislao/jefe (id=1).

SET FOREIGN_KEY_CHECKS = 0;

-- 1. Actualizar tickets (ventas)
UPDATE tickets SET usuario = 4 WHERE usuario = 1;

-- 2. Actualizar tickets históricos (ticketss)
UPDATE ticketss SET usuario = 4 WHERE usuario = 1;

-- 3. Actualizar tickets temporales (vendedor es el usuario en ticket_tmp)
UPDATE ticket_tmp SET vendedor = 4 WHERE vendedor = 1;

-- 4. Actualizar logs de VeriFactu (el usuario se guarda como texto)
UPDATE verifactu_logs SET usuario = '4' WHERE usuario = '1';

SET FOREIGN_KEY_CHECKS = 1;
