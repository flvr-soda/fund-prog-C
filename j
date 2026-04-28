-- CABECERA: Un solo registro con los totales
SELECT 
    'ONTNOMG200000414' || 
    LPAD(COUNT(*), 7, '0') || 
    LPAD(CAST(SUM(monto) * 100 AS INTEGER), 15, '0') || 
    'VES' || 
    TO_CHAR(MAX(fecha_nomina), 'YYYYMMDD') AS linea_archivo
FROM hcheques_doc
WHERE fecha_nomina = (
    SELECT MAX(fecha_nomina) 
    FROM hcheques_doc 
    WHERE fecha_nomina BETWEEN '2025-01-01' AND '2025-01-31'
)

UNION ALL

-- DETALLE: Todos los trabajadores
SELECT 
    SUBSTR(h.cedula, 1, 1) ||                -- Nacionalidad (V o E)
    LPAD(SUBSTR(h.cedula, 2), 8, '0') ||     -- Cédula (los números después de la letra)
    RPAD(h.cuenta, 20, ' ') ||               -- Cuenta (20 caracteres)
    LPAD(CAST(h.monto * 100 AS INTEGER), 11, '0') || -- Monto sin decimales
    RPAD(UPPER(t.nombre), 40, ' ')           -- Nombre en mayúsculas
FROM hcheques_doc h
JOIN trabajador t ON h.cedula = t.cedula
WHERE h.fecha_nomina = (
    SELECT MAX(fecha_nomina) 
    FROM hcheques_doc 
    WHERE fecha_nomina BETWEEN '2025-01-01' AND '2025-01-31'
);