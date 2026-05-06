# Preparando el archivo solucion3.sql basado en el requerimiento DCRRHH-025-2026
sql_content = """-- solucion3.sql
-- REPORTE: Trabajadoras con hijos para Bono "Día de las Madres"
-- ORIGEN: Memorando DCRRHH-025-2026

SELECT DISTINCT
    t.cedula AS "Cédula de Identidad",
    t.nombre AS "Apellidos y Nombres",
    laboral.nro_puesto AS "Número de puesto",
    laboral.tipo AS "Tipo de Personal",
    t.sexo AS "Género"
FROM 
    nomina.trabajador t
INNER JOIN (
    -- Se consolidan las tablas laborales para obtener el número de puesto
    SELECT cedula, nro_puesto, 'ADMINISTRATIVO' AS tipo FROM nomina.administrativo
    UNION ALL
    SELECT cedula, nro_puesto, 'OBRERO' AS tipo FROM nomina.obrero
    UNION ALL
    SELECT cedula, nro_puesto, 'DOCENTE' AS tipo FROM nomina.docente
) laboral ON t.cedula = laboral.cedula
WHERE 
    t.sexo = 'F' -- Filtro exclusivo para trabajadoras (Mujeres)
    AND EXISTS (
        -- Verificación de existencia de al menos un hijo registrado
        -- Nota: En la tabla BENEFICIARIO el campo 'PARENTESCO' identifica el nexo.
        -- Comúnmente se usa 'H' para Hijos en este sistema.
        SELECT 1 
        FROM nomina.beneficiario b 
        WHERE b.ced_falle = t.cedula 
        AND b.parentesco = 'H' 
    )
ORDER BY 
    t.nombre;

