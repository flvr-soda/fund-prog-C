-- Ejercicio 6: Personal Obrero que cotizó a SOTUC TODO el año 2025
SELECT 
    t.cedula AS "CEDULA", 
    t.nombre AS "APELLIDO Y NOMBRE",
    CASE 
        WHEN EXISTS (SELECT 1 FROM nomina.asided_obr act WHERE act.cedula = t.cedula) 
        THEN 'SI COBRA ACTUALMENTE'
        ELSE 'NO COBRA ACTUALMENTE'
    END AS "ESTATUS EN NOMINA ORDINARIA"
FROM nomina.trabajador t
WHERE t.cedula IN (
    SELECT h.cedula 
    FROM nomina.hasidedobr h 
    JOIN nomina.tper_conc c ON h.cod_concepto = c.cod_concepto
    WHERE c.descripcion LIKE '%SOTUC%'
      AND EXTRACT(YEAR FROM h.fecha_nomina) = 2025
    GROUP BY h.cedula
    HAVING COUNT(DISTINCT EXTRACT(MONTH FROM h.fecha_nomina)) = 12
);
