-- Ejercicio 3: Listado de conceptos de asignaciones pagados (Diciembre 2025)
SELECT 
    c.cod_concepto AS "COD_CONCEPTO", 
    c.descripcion AS "DESCRIPCION", 
    SUM(h.monto) AS "MONTO TOTAL CANCELADO"
FROM nomina.hasidedadm h
JOIN nomina.tper_conc c ON h.cod_concepto = c.cod_concepto
WHERE EXTRACT(MONTH FROM h.fecha_nomina) = 12
  AND EXTRACT(YEAR FROM h.fecha_nomina) = 2025
  -- Asumimos que TIPO_OP = 1 identifica las asignaciones en tu maestro de conceptos
  AND c.tipo_op = 1 
GROUP BY c.cod_concepto, c.descripcion
ORDER BY c.cod_concepto;
