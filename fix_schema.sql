-- Fix Tickets table
ALTER TABLE tickets MODIFY COLUMN descuento double DEFAULT 0;
ALTER TABLE tickets MODIFY COLUMN base double DEFAULT 0;
ALTER TABLE tickets MODIFY COLUMN iva double DEFAULT 0;
ALTER TABLE tickets MODIFY COLUMN total decimal(10,2) DEFAULT 0;

-- Fix Ticketss (histórico) table
ALTER TABLE ticketss MODIFY COLUMN descuento double DEFAULT 0;
ALTER TABLE ticketss MODIFY COLUMN base double DEFAULT 0;
ALTER TABLE ticketss MODIFY COLUMN iva double DEFAULT 0;
ALTER TABLE ticketss MODIFY COLUMN total decimal(10,2) DEFAULT 0;
