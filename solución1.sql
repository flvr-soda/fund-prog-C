-- solucion1.sql
-- Nota: Al ejecutar en SQL Developer, pedirá los valores para mes (MM) y año (YYYY)

SELECT 
    [span_0](start_span)'EN' AS TIPO_DOC, --[span_0](end_span)
    'EN' || '&mes' || [span_1](start_span)'&anio' AS NRO_DOCUMENTO, --[span_1](end_span)
    [span_2](start_span)'2' AS COMPROBANTE, --[span_2](end_span)
    [span_3](start_span)SYSDATE AS FECHA_REGISTRO, --[span_3](end_span)
    [span_4](start_span)h.fecha_nomina AS FECHA_TRAMITE, --[span_4](end_span)
    [span_5](start_span)'&anio' AS PERIODO, --[span_5](end_span)
    [span_6](start_span)SUBSTR(p.sub_accion_esp, 1, INSTR(p.sub_accion_esp, '.') - 1) AS ESTRUCTURA, --[span_6](end_span)
    [span_7](start_span)c.codigo_cta AS CUENTA_CONTABLE, -- Relación con Contabilidad.cuenta[span_7](end_span)
    [span_8](start_span)SUM(h.monto) AS MONTO_DOCUMENTO --[span_8](end_span)
FROM 
    [span_9](start_span)Nomina.hasidedadm h --[span_9](end_span)
INNER JOIN 
    Presupuesto.nomina_ppto p ON h.tipo_personal = p.tipo_personal 
    [span_10](start_span)AND h.id_concepto = p.id_concepto --[span_10](end_span)
LEFT JOIN 
    [span_11](start_span)Contabilidad.cuenta c ON p.id_cuenta = c.id_cuenta -- Enlace sugerido[span_11](end_span)
WHERE 
    TO_CHAR(h.fecha_nomina, 'MM') = '&mes' 
    AND TO_CHAR(h.fecha_nomina, 'YYYY') = '&anio'
GROUP BY 
    h.fecha_nomina, 
    p.sub_accion_esp,
    c.codigo_cta;


-- solucion2.sql
-- Procesa todos los tipos de nómina de un periodo específico

SELECT 
    [span_12](start_span)v.TIPO_DOC_FINAL, --[span_12](end_span)
    v.TIPO_DOC_FINAL || [span_13](start_span)TO_CHAR(v.fecha_nomina, 'MMYYYY') AS NRO_DOCUMENTO, --[span_13](end_span)
    [span_14](start_span)'2' AS COMPROBANTE, --[span_14](end_span)
    [span_15](start_span)SYSDATE AS FECHA_REGISTRO, --[span_15](end_span)
    [span_16](start_span)v.fecha_nomina AS FECHA_ELABORACION, --[span_16](end_span)
    [span_17](start_span)TO_CHAR(v.fecha_nomina, 'YYYY') AS PERIODO, --[span_17](end_span)
    [span_18](start_span)SUBSTR(ppto.sub_accion_esp, 1, INSTR(ppto.sub_accion_esp, '.') - 1) AS ACCION_ESP, --[span_18](end_span)
    [span_19](start_span)SUM(v.monto) AS MONTO_TOTAL --[span_19](end_span)
FROM (
    -- Unificación de todas las tablas de asignaciones mencionadas
    [span_20](start_span)SELECT monto, fecha_nomina, tipo_personal, id_concepto, 'EN' as TIPO_DOC_FINAL FROM Nomina.hasidedadm --[span_20](end_span)
    UNION ALL
    [span_21](start_span)[span_22](start_span)SELECT monto, fecha_nomina, tipo_personal, id_concepto, 'EO' as TIPO_DOC_FINAL FROM Nomina.hasidedobr --[span_21](end_span)[span_22](end_span)
    UNION ALL
    [span_23](start_span)SELECT monto, fecha_nomina, tipo_personal, id_concepto, 'EN' as TIPO_DOC_FINAL FROM Nomina.hasideddoc --[span_23](end_span)
    UNION ALL
    [span_24](start_span)SELECT monto, fecha_nomina, tipo_personal, id_concepto, 'EN' as TIPO_DOC_FINAL FROM Nomina.hasidedvda --[span_24](end_span)
) v
INNER JOIN 
    Presupuesto.nomina_ppto ppto ON v.tipo_personal = ppto.tipo_personal 
    [span_25](start_span)AND v.id_concepto = ppto.id_concepto --[span_25](end_span)
WHERE 
    v.fecha_nomina BETWEEN TO_DATE('01/&mes/&anio', 'DD/MM/YYYY') 
    AND LAST_DAY(TO_DATE('01/&mes/&anio', 'DD/MM/YYYY'))
GROUP BY 
    v.TIPO_DOC_FINAL, 
    v.fecha_nomina, 
    ppto.sub_accion_esp;
