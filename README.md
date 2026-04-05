# Tensor++

> Librería matemática de tensores en C++ para aplicaciones de Deep Learning

![Curso](https://img.shields.io/badge/Curso-Programación%20III%20CS2013-blue)
![UTEC](https://img.shields.io/badge/Universidad-UTEC-red)
![Período](https://img.shields.io/badge/Período-2026--1-green)
![C++](https://img.shields.io/badge/C++-17-00599C?logo=c%2B%2B)

---

## 📌 Descripción General

**Tensor++** es una librería matemática desarrollada en **C++**, diseñada para manejar tensores de hasta 3 dimensiones. Está inspirada en bibliotecas científicas como [NumPy](https://numpy.org/) y [PyTorch](https://pytorch.org/), e implementa gestión dinámica manual de memoria y operaciones algebraicas avanzadas, necesarias para construir aplicaciones de procesamiento de datos como una red neuronal.

---

## 📁 Estructura del Repositorio

```
Tensor++_Jasyr VP/
├── Tensor.h       # Clase Tensor y la interfaz TensorTransform (ReLU, Sigmoid)
├── main.cpp       # Flujo principal: construcción y prueba de la red neuronal
└── README.md      # Este archivo
```

| Archivo | Descripción |
|---|---|
| `Tensor.h` | Declaración e implementación de la clase `Tensor` (gestión de memoria, operadores matemáticos) y de la interfaz `TensorTransform` con sus clases derivadas (`ReLU`, `Sigmoid`). |
| `main.cpp` | Código fuente con el flujo principal de ejecución que construye y prueba la red neuronal solicitada. |
| `README.md` | Este archivo, con instrucciones claras de compilación y ejecución. |

---

## ⚙️ Instrucciones de Compilación y Ejecución

### Paso 1 — Compilación

Ubícate en el **directorio raíz** del proyecto y ejecuta el siguiente comando desde tu terminal:

```bash
g++ main.cpp -o tensor_app -std=c++17 -O3
```

> **Nota:** Se utiliza `-O3` para optimizar el rendimiento de las multiplicaciones matriciales y `-std=c++17` para asegurar la compatibilidad del estándar.

---

### Paso 2 — Ejecución

Una vez generado el ejecutable, córrelo con el comando correspondiente a tu sistema operativo:

**Linux / macOS**
```bash
./tensor_app
```

**Windows (CMD / PowerShell)**
```powershell
.\tensor_app.exe
```

---

## 🖥️ Salida Esperada

Al ejecutar el programa, se simulará el flujo completo de datos de la aplicación (Pasos 1 al 8). En la consola deberías ver el progreso de cada etapa, finalizando con la confirmación de las dimensiones correctas de la activación final (`{1000, 10}`):

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

---

<p align="center">
  Tarea #1 · Programación III (CS2013) · UTEC · 2026-1
</p>