DELETE t1 FROM usuarios t1
INNER JOIN usuarios t2 
WHERE t1.id = t2.id AND t1.updated_at < t2.updated_at;

-- If there are exact duplicates (same id, same updated_at) but different hidden rows:
-- We can add an auto_increment temporary column, or just recreate the table.
