-- ============================================================
-- Migración: Mejora del sistema de arqueo de caja (Opción B)
-- Ejecutar sobre la base de datos tiendaNueva
-- ============================================================

-- 1. Añadir columna efectivoContado (lo que realmente se contó en caja)
ALTER TABLE arqueos
  ADD COLUMN efectivoContado DOUBLE DEFAULT 0 AFTER descuadre;

-- 2. Añadir columna usuario (quién realizó el arqueo)
ALTER TABLE arqueos
  ADD COLUMN usuario VARCHAR(50) DEFAULT '' AFTER efectivoContado;

-- 3. Nueva tabla para desglose de denominaciones
CREATE TABLE IF NOT EXISTS arqueos_detalle (
  id INT PRIMARY KEY AUTO_INCREMENT,
  idArqueo INT NOT NULL,
  denominacion DECIMAL(5,2) NOT NULL,
  cantidad INT NOT NULL DEFAULT 0,
  FOREIGN KEY (idArqueo) REFERENCES arqueos(id) ON DELETE CASCADE
);
