# Proyecto-Final : 🌿 Las Cicatrices del Valle Iridiano
Esto es el proyecto final del clase Programacion Orientada En Objectos 2025-2, el objectivo es crear una simulacion de una ecosistema donde los criaturas viven dentro de un entorno constantemente cambiando, tienen que sobrivir , reproducir y superar los condiciones Entornos extremos.

## Objectivos

- Modelar un ecosistema dinámico con criaturas de diferentes tipos.
- Implementar herencia múltiple, clases y métodos virtuales.
- Implementar Los tecnicas de asociaciones dentro del proyecto.
- Mostrar conceptos reales de diseño de software aplicados a un mundo simulado.
- Ofrecer una simulación visual basada en consola.
- Permitir ejecutar múltiples ciclos donde las criaturas interactúan y cambian.

---


##  Manual del Usuario

###  ¿Qué hace el programa?
Al ejecutar el programa se genera:
- Un **mapa 8x8** de ambientes (llanura, desierto, agua).
- Una población inicial de criaturas:
  - **RaizLenta** (planta)
  - **Centella** (animal)
  - **Albo** (animal volador regenerador)

Cada ciclo de simulación ejecuta:
1. `actuar()` — movimiento, envejecimiento, efectos internos  
2. `interactuar()` — interacciones con criaturas cercanas  
3. `reproducirse()` — posibilidad de generar una nueva criatura  
4. Eliminación de criaturas muertas  
5. Visualización del mapa  
6. Estadísticas de población  

La simulación dura **14 ciclos** o hasta que no queden criaturas vivas.

###  Ejemplo de Comportamientos
- **RaizLenta** absorbe energía y comparte con otras raíces cercanas.
- **Centella** roba energía a plantas cercanas.
- **Albo** puede *volar* y *regenerarse*, además de ayudar a criaturas cercanas.

---
#  **Ejemplo de Salida (Mapa ASCII)**
=== El Mundo de El Valle Iridiano ===

--- Ciclo 1 ---
-Texto de eventos durante el ciclo.
```

=== MAPA ===
. . . C . . . .
. R . . A . . .
. . C . . . . .
. . . R . . . .
. . . . . . . .
. . . . . . . .
. . . . . . . .
. . . . . . . .

```
```

Poblacion: 5 (Albo:1 Centella:2 RaizLenta:2)

````
---

##  Instrucciones de Compilación y Ejecución
## Prerrequisitos
- Git instalado en el sistema
- CLion instalado
- Acceso a Internet para clonar el repositorio

##  Clonar el Proyecto

### Paso 1: Preparar el entorno
1. Abrir **Git Bash**
2. Navegar a la carpeta destino:
   ```bash
   cd "ruta/de/su/carpeta/preferida"

### Paso 2: Clonar el repositorio
    ```bash
    git clone https://github.com/ThomasGomez970/Proyecto-Final.git
## Paso 3: Abir y compilar
   Abrir CLion

   Seleccionar "Nuevo proyecto"

   Buscar y seleccionar la carpeta que contiene el repositorio clonado

   Seleccionar "Crear a partir de fuentes existentes"

   Asegurarse de que el archivo principal(main.cpp) que contiene el código esté seleccionado.
   
   Ahora puedes selecionar para compilar. 




# **Estructura del Proyecto

```
├── main.cpp
├──UML.md
├──Evaluacion.md.
└── README.md
```

# Conceptos de Programacion orientada en objectos ultilizados
### 1. **Herencia**
El código usa herencia para crear jerarquías de criaturas.
Ejemplo:
- `Planta` hereda de `Criatura`.
- `Animal` hereda de `Criatura`.
- `RaizLenta` hereda de `Planta`.
- `Centella` hereda de `Animal`.

```cpp
class Planta : public Criatura { ... };
class Animal : public Criatura { ... };
class RaizLenta : public Planta { ... };
````

Esto permite reutilizar atributos como vida, posición, edad y métodos como mover(), envejecer(), etc.

---

###  2. **Polimorfismo**

Se logra con métodos virtuales puros en `Criatura` y un vector que almacena distintos tipos de criaturas.

```cpp
vector<shared_ptr<Criatura>> mundo;
```

Cada criatura implementa:

```cpp
virtual void actuar(...) = 0;
virtual void interactuar(...) = 0;
virtual shared_ptr<Criatura> reproducirse() = 0;
```

Al recorrer `mundo`, aunque los objetos son de distintos tipos (Albo, RaizLenta, Centella), se llama al método correcto de cada uno.

---

###  3. **Herencia Múltiple**

Ejemplo directo: la clase **Albo**.

```cpp
class Albo : public Animal, public Volador, public Regenerador { ... };
```

`Volador` y `Regenerador` son interfaces (clases abstractas) y Albo hereda de ambas además de Animal.

Además se usa `using` para resolver ambigüedades:

```cpp
using Animal::curar;
using Animal::daniar;
```

Esto demuestra un caso correcto y controlado de herencia múltiple.

---

###  4. **Composición**

La clase Mapa contiene internamente el grid del ambiente.

```cpp
vector<vector<Ambiente>> grid;
```

El mapa existe *dentro* de la clase Mapa, no afuera, lo que es un ejemplo claro de composición.
Si el objeto Mapa se destruye, también lo hace su grid.

---

###  5. **Asociación**

Las criaturas interactúan con el `mundo` pero no lo poseen.

```cpp
void actuar(vector<shared_ptr<Criatura>>& mundo)
```

Esto significa:

* La criatura usa el vector mundo
* No es dueña de él
* No controla su ciclo de vida

Eso es una **asociación**

---

###  6. **Clases abstractas e interfaces**

La clase `Criatura` define un conjunto de métodos puros:

```cpp
virtual void actuar(...) = 0;
virtual bool puedeSobrevivirEn(...) const = 0;
```

Esto obliga a que todas las criaturas implementen su propio comportamiento.

Aqui estan los subclasses que que una de las criaturas hereda. 

* `Volador`
* `Regenerador`

Ejemplo:

```cpp
class Volador {
public:
    virtual void volar() = 0;
};
```

---

###  7. **Smart pointers (shared_ptr)**

El vector del mundo utiliza punteros inteligentes:

```cpp
vector<shared_ptr<Criatura>> mundo;
```


## Créditos 
   Thomas David Gomez Tamayo


---
