# Tensor++ 

> **Tarea #1** · Programación III (CS2013) · UTEC · Periodo 2026-1

---

## Descripción General

**Tensor++** es una librería matemática desarrollada en C++ diseñada para manejar tensores de hasta 3 dimensiones, inspirada en bibliotecas científicas como NumPy y PyTorch. Implementa la gestión dinámica manual de memoria y operaciones algebraicas avanzadas necesarias para construir aplicaciones de procesamiento de datos, como una red neuronal.

---

## Justificación Técnica y Decisiones de Diseño

Para cumplir con los requerimientos de la tarea y asegurar un rendimiento óptimo en las operaciones matemáticas, esta librería se construyó aplicando prácticas de C++ moderno:

### Gestión Manual de Memoria — Regla de los 5
Dado que se exigía manejar la memoria dinámicamente de forma manual (mediante un arreglo contiguo `double*`), se implementó estrictamente la **Regla de los 5**: Destructor, Constructor de copia, Constructor de movimiento, Asignador de copia y Asignador de movimiento. Esto garantiza que no existan fugas de memoria (*memory leaks*) durante la creación y destrucción de tensores en la red neuronal.

### Move Semantics — Semántica de Movimiento
En métodos como `view` o `concat`, donde el enunciado exige explícitamente *"no copiar los datos"*, se utilizó la transferencia de propiedad de punteros mediante referencias r-value (`&&`) y `std::move`. Esto evita copiar grandes bloques de memoria, trasladando los datos al nuevo tensor y dejando al original en un estado válido pero seguro (nulo).

### Seguridad — Clang-Tidy
Se incorporaron directivas como `[[nodiscard]]` para asegurar que el usuario no ignore el retorno de operaciones matemáticas, y se marcaron los constructores de un solo parámetro como `explicit` para evitar conversiones de tipos implícitas y errores silenciosos.

---

## Estructura del Repositorio

```
Tensor++_Jasyr VP/
├── Tensor.h      # Declaración e implementación de la clase Tensor y la interfaz TensorTransform
├── main.cpp      # Flujo principal de ejecución que construye y prueba la red neuronal
└── README.md     # Este archivo
```

| Archivo | Descripción |
|---|---|
| `Tensor.h` | Clase `Tensor` (gestión de memoria, operadores matemáticos) e interfaz `TensorTransform` con sus clases derivadas (`ReLU`, `Sigmoid`) |
| `main.cpp` | Código fuente con el flujo principal de ejecución que construye y prueba la red neuronal solicitada |
| `README.md` | Instrucciones de compilación, ejecución y justificación técnica |

---

## Instrucciones de Compilación y Ejecución

### Paso 1 — Compilación

Ubícate en el directorio raíz del proyecto y ejecuta el siguiente comando:

```bash
g++ main.cpp -o tensor_app -std=c++17 -O3
```

> **Nota:** La bandera `-O3` optimiza el rendimiento de las multiplicaciones matriciales y `-std=c++17` asegura la compatibilidad con el estándar requerido.

### Paso 2 — Ejecución

Una vez generado el ejecutable, córrelo según tu sistema operativo:

**Linux / macOS**
```bash
./tensor_app
```

**Windows (CMD / PowerShell)**
```powershell
.\tensor_app.exe
```

---

## Salida Esperada

Al ejecutar el programa, este simulará el flujo completo de datos de la aplicación real (Pasos 1 al 8). La consola mostrará el progreso de cada etapa hasta confirmar que las dimensiones de la activación final son las correctas (`{1000, 10}`):

```
--- Iniciando Flujo de Red Neuronal ---
1. Input creado con éxito.
2. View aplicado.
3. Matmul W1 completado.
4. Suma Bias b1 completada.
5. ReLU aplicado.
6. Matmul W2 completado.
7. Suma Bias b2 completada.
8. Sigmoid aplicado. Dimensiones finales de salida: {1000, 10}
```