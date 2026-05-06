-- Ejercicio 4: Personal Docente que cotizó a APUC en 2025 y su estatus actual
SELECT 
    t.cedula AS "CEDULA", 
    t.nombre AS "APELLIDO Y NOMBRE",
    CASE 
        WHEN EXISTS (SELECT 1 FROM nomina.asided_doc act WHERE act.cedula = t.cedula) 
        THEN 'SI COBRA ACTUALMENTE'
        ELSE 'NO COBRA ACTUALMENTE'
    END AS "ESTATUS EN NOMINA ORDINARIA"
FROM nomina.trabajador t
WHERE t.cedula IN (
    -- Subconsulta para ubicar a quienes se les descontó APUC en 2025
    SELECT h.cedula 
    FROM nomina.hasideddoc h 
    JOIN nomina.tper_conc c ON h.cod_concepto = c.cod_concepto
    WHERE c.descripcion LIKE '%APUC%'
      AND EXTRACT(YEAR FROM h.fecha_nomina) = 2025
);
