-- Ejercicio 2: Cálculo para el 8.5% - Personal Administrativo
SELECT 
    t.cedula AS "CEDULA", 
    t.nombre AS "APELLIDO Y NOMBRE", 
    h.fecha_nomina AS "FECHA_NOMINA",
    SUM(CASE WHEN h.cod_concepto = '20100' THEN h.monto ELSE 0 END) AS "SUELDO BASICO",
    SUM(CASE WHEN h.cod_concepto = '20120' THEN h.monto ELSE 0 END) AS "PRIMA DE TITULAR",
    SUM(CASE WHEN h.cod_concepto = '20136' THEN h.monto ELSE 0 END) AS "PRIMA DE DIRECTOR",
    SUM(CASE WHEN h.cod_concepto = '20142' THEN h.monto ELSE 0 END) AS "PRIMA POR ANTIGUEDAD",
    SUM(CASE WHEN h.cod_concepto = '20147' THEN h.monto ELSE 0 END) AS "PRIMA DE RESPONSABILIDAD",
    SUM(CASE WHEN h.cod_concepto = '20166' THEN h.monto ELSE 0 END) AS "COMPLEMENTO PRIMA RESPONSABILI",
    SUM(CASE WHEN h.cod_concepto = '20170' THEN h.monto ELSE 0 END) AS "PRIMA ALTO NIVEL",
    SUM(CASE WHEN h.cod_concepto = '20172' THEN h.monto ELSE 0 END) AS "PRIMA ACTIVIDAD UNIVERSIT",
    SUM(CASE WHEN h.cod_concepto = '20174' THEN h.monto ELSE 0 END) AS "PRIMA RESPONSAB C.C.U",
    SUM(CASE WHEN h.cod_concepto = '20176' THEN h.monto ELSE 0 END) AS "PRIMA JERARQUIA C.C.U",
    SUM(CASE WHEN h.cod_concepto = '20511' THEN h.monto ELSE 0 END) AS "PRIMA JERARQUIA",
    SUM(CASE WHEN h.cod_concepto = '20514' THEN h.monto ELSE 0 END) AS "PARTICIPACION PROYEC ESPECIAL",
    SUM(CASE WHEN h.cod_concepto = '21600' THEN h.monto ELSE 0 END) AS "PRIMA POR HIJOS",
    SUM(CASE WHEN h.cod_concepto = '25100' THEN h.monto ELSE 0 END) AS "TABULADOR PROFESIONALES Y TECN",
    SUM(CASE WHEN h.cod_concepto = '27100' THEN h.monto ELSE 0 END) AS "PRIMA FAMILIAR"
FROM nomina.hasidedadm h
JOIN nomina.trabajador t ON h.cedula = t.cedula
JOIN nomina.administrativo a ON h.cedula = a.cedula
WHERE h.fecha_nomina = TO_DATE('31/12/2025', 'DD/MM/YYYY')
  -- Filtro: Ingreso antes del 01/01/2024
  AND a.f_ingreso_cargo < TO_DATE('01/01/2024', 'DD/MM/YYYY')
  -- Filtro para jubilados: Fecha de jubilación mayor a 30/09/2021
  AND (a.f_jubilacion IS NULL OR a.f_jubilacion > TO_DATE('30/09/2021', 'DD/MM/YYYY'))
GROUP BY t.cedula, t.nombre, h.fecha_nomina;
