-- Ejercicio 1: Archivo texto plano - Nómina Enero 2025 (Corregido)
-- Extrayendo nacionalidad desde la cédula y usando la tabla de cheques (Docentes)

SELECT linea_txt FROM (
    -- 1. BLOQUE DE CABECERA
    SELECT 
        1 AS orden,
        -- Constante + Nro Personas (7) + Monto Nomina (15) + VES + Fecha (8)
        'ONTNOMG200000414' || 
        LPAD(COUNT(DISTINCT hc.cedula), 7, '0') || 
        LPAD(REPLACE(TO_CHAR(SUM(hc.monto), '999999999999.99'), '.', ''), 15, '0') || 
        'VES' || 
        '20250131' AS linea_txt
    FROM nomina.hcheques_doc hc
    -- Se asume el uso de fecha_pago en la tabla de cheques, ajusta si es fecha_nomina
    WHERE hc.fecha_pago = TO_DATE('31/01/2025', 'DD/MM/YYYY')
    
    UNION ALL
    
    -- 2. BLOQUE DE DETALLE
    SELECT 
        2 AS orden,
        -- Nacionalidad (1): Primer carácter de la cadena
        -- Cédula (8): Del segundo carácter en adelante, con ceros a la izquierda
        -- Cuenta (20): Rellenada con espacios a la derecha
        -- Monto (11): Suma total sin puntos decimales, ceros a la izquierda
        -- Nombre (40): Rellenado con espacios a la derecha
        SUBSTR(t.cedula, 1, 1) || 
        LPAD(SUBSTR(t.cedula, 2), 8, '0') || 
        RPAD(NVL(d.nro_cta_banco_nomina, ' '), 20, ' ') || 
        LPAD(REPLACE(TO_CHAR(SUM(hc.monto), '99999999.99'), '.', ''), 11, '0') || 
        RPAD(t.nombre, 40, ' ') AS linea_txt
    FROM nomina.hcheques_doc hc
    JOIN nomina.trabajador t ON hc.cedula = t.cedula
    JOIN nomina.docente d ON hc.cedula = d.cedula
    WHERE hc.fecha_pago = TO_DATE('31/01/2025', 'DD/MM/YYYY')
    GROUP BY t.cedula, d.nro_cta_banco_nomina, t.nombre
) 
ORDER BY orden, linea_txt;
