-- Ejercicio 5: Personal Administrativo que cotizó a AEUC TODO el año 2025
SELECT 
    t.cedula AS "CEDULA", 
    t.nombre AS "APELLIDO Y NOMBRE",
    CASE 
        WHEN EXISTS (SELECT 1 FROM nomina.asided_adm act WHERE act.cedula = t.cedula) 
        THEN 'SI COBRA ACTUALMENTE'
        ELSE 'NO COBRA ACTUALMENTE'
    END AS "ESTATUS EN NOMINA ORDINARIA"
FROM nomina.trabajador t
WHERE t.cedula IN (
    -- Subconsulta: Solo los que tienen descuentos en 12 meses distintos de 2025
    SELECT h.cedula 
    FROM nomina.hasidedadm h 
    JOIN nomina.tper_conc c ON h.cod_concepto = c.cod_concepto
    WHERE c.descripcion LIKE '%AEUC%'
      AND EXTRACT(YEAR FROM h.fecha_nomina) = 2025
    GROUP BY h.cedula
    HAVING COUNT(DISTINCT EXTRACT(MONTH FROM h.fecha_nomina)) = 12
);
