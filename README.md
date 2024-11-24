# Análisis Numérico de Datos de Casos Confirmados de Covid-19

> [!WARNING]  
> Este código fue creado por: Geordy Harley Pardo Velosa. Si desea usarlo en alguna investigación o trabajo académico, se debe dar crédito de la siguiente forma:  
> G. H. Pardo Velosa, "Cpp-Datos-Numericos", GitHub, 2024. [Enlace]. Accedido: Día-Mes-Año.  
> https://github.com/NYoyiC/C-Datos-Numericos


> [!NOTE]
>Esta codigo sirve tambien para cualquier tipo de .CSV que contenga el formato mencionado más adelante.

<p>
Este programa en C++ permite procesar archivos CSV que contienen datos de fechas y casos confirmados. Realiza análisis numéricos utilizando diferencias finitas para calcular derivadas de primer y segundo orden, e identifica los puntos de mayor aceleración y desaceleración.
</p>

## Caracteristicas del código

1. **Lectura de Archivos CSV**  
- El programa puede leer archivos `.csv` que contengan datos organizados en formato:

| fecha   | casos / datos |
| ------------- | ------------- |
| 2024-11-01 | 100  |
| 2024-11-02  | 200  |

> [!NOTE]
> El archivo debe estar en la misma carpeta que el programa.

2. **Cálculo de Diferencias Finitas**  
- Derivadas de **primer orden** (centradas, adelantadas, atrasadas).
- Derivadas de **segundo orden** (centradas, adelantadas, atrasadas).
3. **Resultados Analíticos**  
- Identifica las fechas de mayor aceleración y desaceleración según las derivadas calculadas.
- Presenta los resultados de forma tabular con soporte para colores en la consola.

## Requisitos

- **Sistema Operativo:** Windows (por el uso de la biblioteca `windows.h` para cambiar colores).
- **Compilador de C++** compatible con el estándar C++11 o superior (como MinGW en Windows).
- Archivo CSV en el mismo directorio del programa.

## Instrucciones de Uso

### 1. Compilación
Utiliza un compilador de C++ para compilar el código. Por ejemplo:
```bash
g++ datos.cpp -o AnalisisDatos
```
### 2. Ejecución
Ejecuta el archivo .exe desde la terminal
```bash
AnalisisDatos
```

### 3. Selección del Archivo
El programa buscará automáticamente archivos `.csv` en la carpeta actual y mostrará una lista de los disponibles. Ingresa el nombre del archivo (sin la extensión  `.csv`) para procesarlo.

### 4. Resultados
El programa calcula las derivadas de primer y segundo orden y muestra los resultados en una tabla, indicando:
- Mayor aceleración y desaceleración para cada método de cálculo (centrado, adelantado, atrasado).

### Ejemplo de salida

```bash
Archivos .csv encontrados en: .
- datos_ejemplo

Ingrese el nombre del archivo CSV Ejemplo (datos_ejemplo): datos_ejemplo
+-------------------------------------------------+--------------+------------+
|                  Resultados                    |    Fecha     |    Valor   |
+-------------------------------------------------+--------------+------------+
Mayor aceleracion (centrada 1er orden)           | 2024-11-02   |    50
Mayor desaceleracion (centrada 1er orden)        | 2024-11-03   |   -30
```


## Funcionalidades tecnicas

1. Lectura de Archivos CSV

- Se usa `std::ifstream` para abrir y procesar los datos.
- Soporte para delimitadores ;.

2. Cálculo Numérico
Implementación de fórmulas de diferencias finitas para derivadas:
- Primer Orden:
   - Centradas: (f(x+h) - f(x-h) / 2h
   - Adelantadas: (f(x+h) - f(x) / h
   - Atrasadas: (f(x) - f(x-h) / 2h
 
- Segundo Orden:
   - Centradas: (f(x+h) - 2f(x) + f(x-h) / h^2
   - Adelantadas: (-3f(x) + 4f(x+h) - f(x+2h)) / 2h
   - Atrasadas: (3f(x) - 4f(x-h) + f(x-2h)) / 2h

3. Colores en la Consola

- Cambia el color de los resultados usando la biblioteca  `Windows.h`:
- Verde para resultados centrados.
- Azul para resultados adelantados.
- Púrpura para resultados atrasados.

4. Manejo de Errores

- Validación de apertura de archivos.
- Verificación de valores numéricos en los datos.

## Limitaciones
> [!NOTE]
> Este primera limitación no perjudica en total el codigo final solamente no sera visto en consola el cambio de color.
1. El programa está diseñado para Windows debido al uso de la biblioteca `windows.h` para cambiar colores.
2. No soporta archivos CSV con delimitadores diferentes a `;`.
3. Requiere que los datos sean estrictamente numéricos y organizados en columnas `fecha;casos`.

> [!IMPORTANT]
> En varias oportunidades se ha probado con datos distintos pero en todos se encuentra un serie de valores en erroneos en las diferencias adelantadas y atrasadas de segundo orden. No se ha podido encontrar el origen del error, porfavor tener en cuenta.

