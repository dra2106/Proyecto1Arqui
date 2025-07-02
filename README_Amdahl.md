# Análisis de la Ley de Amdahl - ProyectoPhoenix

## Introducción

Este proyecto implementa un sistema de análisis de rendimiento basado en la **Ley de Amdahl** para identificar científicamente qué módulos del juego deben ser priorizados para optimización y paralelización.

## ¿Qué es la Ley de Amdahl?

La Ley de Amdahl es una fórmula utilizada para encontrar la mejora máxima esperada de un sistema cuando solo se mejora una parte del mismo. La fórmula es:

```
Speedup = 1 / (f + (1-f)/n)
```

Donde:
- `f` = fracción del programa que debe ejecutarse secuencialmente
- `n` = número de procesadores
- `1-f` = fracción del programa que puede paralelizarse

## Instrumentación Implementada

### Módulos Medidos

El sistema mide el tiempo de ejecución de los siguientes módulos:

1. **Rendering** - Renderizado de gráficos (ncurses)
2. **Input** - Procesamiento de entrada del usuario
3. **Game Logic** - Lógica general del juego
4. **Collision** - **MÓDULO CRÍTICO** - Sistema de detección de colisiones

### Métricas Generadas

- **Tiempo por frame** en cada módulo
- **Porcentaje del tiempo total** que consume cada módulo
- **Fracción secuencial vs paralelizable**
- **Speedup teórico** para 2, 4, 8, y 16 núcleos
- **Identificación del cuello de botella** principal

## Justificación Científica

### ¿Por qué el módulo de colisiones?

1. **Complejidad Algorítmica**: O(n²) en el peor caso para n entidades
2. **Paralelización Natural**: Cada entidad puede procesarse independientemente
3. **Mayor Impacto**: Típicamente consume 40-60% del tiempo de cómputo
4. **Escalabilidad**: El problema se agrava con más enemigos/balas

### Aplicación de Amdahl

Si el sistema de colisiones representa el 50% del tiempo de ejecución:
- Con 2 núcleos: Speedup máximo = 1.33x
- Con 4 núcleos: Speedup máximo = 1.60x
- Con 8 núcleos: Speedup máximo = 1.78x

## Cómo Usar el Sistema

### 1. Compilar el Proyecto
```bash
make
```

### 2. Ejecutar el Juego
```bash
./ProyectoPhoenix
```

### 3. Generar Métricas
- Juega por al menos 30-60 segundos para obtener datos representativos
- Las métricas se muestran en tiempo real en la ventana lateral
- Al perder todas las vidas, se genera automáticamente un reporte científico

### 4. Revisar el Reporte
El reporte se guarda en: `Files/Amdahl_Analysis_Report.txt`

## Interpretación de Resultados

### Métricas Clave

1. **Módulo Crítico**: El que consume más tiempo
2. **Impacto (%)**: Porcentaje del tiempo total
3. **Speedup Teórico**: Mejora máxima esperada con paralelización

### Ejemplo de Interpretación

```
MÓDULO CRÍTICO IDENTIFICADO: collision
Tiempo total consumido: 245.6 ms
Impacto en rendimiento: 52.3%
```

**Interpretación**: El sistema de colisiones consume más de la mitad del tiempo de cómputo, siendo el candidato ideal para paralelización.

### Recomendaciones de Optimización

1. **Prioridad Alta**: Módulos con >30% de impacto
2. **Prioridad Media**: Módulos con 15-30% de impacto  
3. **Prioridad Baja**: Módulos con <15% de impacto

## Estrategias de Paralelización para Colisiones

### 1. Particionado Espacial
- Dividir el espacio de juego en regiones
- Asignar cada región a un hilo diferente
- Procesar colisiones en paralelo por región

### 2. Paralelización por Tipo de Entidad
- Hilo 1: Colisiones jugador-enemigos
- Hilo 2: Colisiones balas-enemigos
- Hilo 3: Colisiones entre enemigos

### 3. Pipeline de Detección
- Etapa 1: Broad phase (detección aproximada)
- Etapa 2: Narrow phase (detección precisa)
- Cada etapa en hilos separados

## Limitaciones y Consideraciones

### Overhead de Sincronización
- La paralelización tiene costos de sincronización
- Para conjuntos pequeños de entidades, puede no ser beneficiosa

### Arquitectura Hardware
- Los resultados dependen del número real de núcleos
- La memoria compartida puede crear cuellos de botella

### Escalabilidad
- Los beneficios disminuyen según la Ley de Amdahl
- Factores como la caché y el ancho de banda de memoria importan

## Archivos Relevantes

- `Profiler.h` - Sistema de medición y análisis
- `GameAdmin.h` - Instrumentación del bucle principal
- `CollisionController.h` - Módulo objetivo (NO MODIFICAR)
- `Files/Amdahl_Analysis_Report.txt` - Reporte generado

## Conclusión

Este sistema proporciona evidencia científica objetiva para justificar la optimización del módulo de colisiones, basándose en mediciones reales y aplicación rigurosa de la Ley de Amdahl. Los datos generados pueden usarse para:

1. Justificar decisiones de optimización en documentos técnicos
2. Priorizar esfuerzos de desarrollo
3. Predecir beneficios de paralelización
4. Comparar diferentes estrategias de optimización

---

**Nota**: El sistema de colisiones se trata como una "caja negra" durante la medición, respetando su implementación actual mientras se obtienen métricas precisas de su impacto en el rendimiento.
