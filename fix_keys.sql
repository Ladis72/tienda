SET SESSION group_concat_max_len = 100000;
SELECT CONCAT('ALTER TABLE usuarios ', GROUP_CONCAT(CONCAT('DROP INDEX ', index_name) SEPARATOR ', '), ';') 
FROM information_schema.statistics 
WHERE table_name = 'usuarios' AND table_schema = 'nubeCervantes' AND index_name LIKE 'updated_at%';
