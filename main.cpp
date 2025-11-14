#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <random>
#include <chrono>
#include <map>
#include <cmath>
using namespace std;

// GENERADOR DE NÚMEROS ALEATORIOS
// Semilla del generador basada en el tiempo actual para asegurar aleatoriedad
static mt19937_64 rng((unsigned)chrono::system_clock::now().time_since_epoch().count());
// Función para generar enteros aleatorios en un rango [a, b]
int randint(int a, int b) {
    return uniform_int_distribution<int>(a, b)(rng);
}

// Función para generar números decimales aleatorios entre 0.0 y 1.0
double rand01() {
    return uniform_real_distribution<double>(0.0, 1.0)(rng);
}

//Ultilize el enumeracion para represetar los 3 tipos de ambientes en el ecosistema.
enum class Ambiente {
    DESIERTO, AGUA,LLANURA
};
// Aqui estan los subclassses para realizar la herecia multiple.


// Interfaz que define la capacidad de vuelo.
class Volador {
public:
    virtual void volar() = 0;
    // Metodo para que deben impremetar a las clases voladoras.
    virtual ~Volador() = default;
    //Implementar un destructor para evitar problemas de memoria
};
// Interfaz que define la capacidad de regeneracion.
class Regenerador {
public:
    virtual void regenerarse() = 0;
    // Métodopuro para regeneración de vida


    virtual ~Regenerador() = default;
    //Implementar un destructor para evitar problemas de memoria
};


class Criatura {
protected:
    string nombre; // nombre de la criatura.
    int x, y; // Posicion en el mapa (las cordenadas).
    int vida;  //Puntos de vida.
    int edad; // Edad de la criatura.

public:
    // Implementar Constructor inicializa una criatura con nombre, posición y vida
    Criatura(string n, int px, int py, int v = 5)
        : nombre(n), x(px), y(py), vida(v), edad(0) {}

    virtual ~Criatura() = default;
    //El destructor virtual para polimorfismo



    // Métodos virtuales puros para polimorfismo

    virtual void actuar(vector<shared_ptr<Criatura>>& mundo) = 0;
    // Define el comportamiento principal de la criatura en cada ciclo

    virtual void interactuar(vector<shared_ptr<Criatura>>& mundo) = 0;
    // Define cómo interactúa la criatura con otras criaturas

    virtual bool puedeSobrevivirEn(Ambiente ambiente) const = 0;

    // Determina si la criatura puede sobrevivir en un ambiente específico

    virtual shared_ptr<Criatura> reproducirse() = 0;
    // Lógica de reproducción de la criatura

    virtual string getTipo() const = 0;
    // Retorna el tipo específico de criatura

    bool estaViva() const {     // Verifica si la criatura está viva (vida > 0)
        return vida > 0;
    }

    void daniar(int d) {
        vida = max(0, vida - d);  // Reduce la vida de la criatura, asegurando que no sea menor a 0
    }

    void curar(int c) {
        vida = min(20, vida + c);   // Aumenta la vida de la criatura, con un máximo de 20
    }

    // Movimiento aleatorio dentro de los límites del mapa

    virtual void mover(int maxX, int maxY) {
        // Los ciraturas tiene 70% de probabilidad de morverse en cada ciclo
        if (rand01() < 0.7) {
            // Calcula nueva posición con movimiento aleatorio (-1, 0, +1)
            x = max(0, min(maxX-1, x + randint(-1, 1)));
            y = max(0, min(maxY-1, y + randint(-1, 1)));
        }
    }


    // Incrementa la edad y aplica efectos del envejecimiento

    void envejecer() {
        edad++;
        if (edad > 10 && rand01() < 0.1) daniar(1);
    }
    // Después de 10 ciclos, tiene 10% de probabilidad de perder vida por veje

    // Aqui estan los getters  y los metodos de ultilidad.

    int getX() const {
        return x;
    }
    int getY() const {
        return y;
    }
    int getVida() const {
        return vida;
    }
    // Condiciones para poder reproducirse: vida suficiente y edad mínima
    bool puedeReproducir() const {
        return vida > 3 && edad >= 2;
    }

    // Se calcula la distancia euclidiana a otra criatura, ultilize el Teorema de pitagoras.

    double distanciaA(const shared_ptr<Criatura>& otra) const {
        int dx = x - otra->getX();
        int dy = y - otra->getY();
        return sqrt(dx*dx + dy*dy);
    }
};

// Clase base para todas las criaturas de tipo planta
class Planta : public Criatura {
public:
    Planta(string n, int x, int y, int v = 5) : Criatura(n, x, y, v) {}

    void mover(int maxX, int maxY) override {
        if (rand01() < 0.3) { // Las plantas se mueven menos
            Criatura::mover(maxX, maxY); // Llama al movimiento base
        }
    }
};

// Clase base para todas las criaturas de tipo animal
class Animal : public Criatura {
public:
    Animal(string n, int x, int y, int v = 5) : Criatura(n, x, y, v) {}

    void mover(int maxX, int maxY) override {
        if (rand01() < 0.8) { // Los animales se mueven más
            Criatura::mover(maxX, maxY); // Llama al movimiento base
        }
    }
};

// Criatura de tipo planta: RaizLenta
class RaizLenta : public Planta {
public:
    RaizLenta(int x, int y) : Planta("RaizLenta", x, y, 8) {}

    // Comportamiento principal en cada ciclo

    void actuar(vector<shared_ptr<Criatura>>& mundo) override {
        envejecer();  // Aplica envejecimiento
        mover(8, 8); // Se mueve en mapa 8x8
        // Probabilidades de ganar o perder vida naturalmente
        if (rand01() < 0.4) curar(1);
        if (rand01() < 0.2) daniar(1);

        cout << "RaizLenta en (" << x << "," << y << ") absorbe energia\n";
    }
    // Interacción con otras criaturas
    void interactuar(vector<shared_ptr<Criatura>>& mundo) override {
        for (auto& otra : mundo) {
            // Se verifica no es ella misma, está viva, es del mismo tipo y está cerca
            if (otra.get() != this && otra->estaViva() &&
                otra->getTipo() == "RaizLenta" && distanciaA(otra) <= 2.0) {
                if (rand01() < 0.3) { // 30% de probabilidad de compartir energía
                    curar(1);
                    cout << "RaicesLentas comparten energia\n";
                }
            }
        }
    }

    // Las RaicesLentas no pueden sobrevivir en desiertos

    bool puedeSobrevivirEn(Ambiente ambiente) const override {
        return ambiente != Ambiente::DESIERTO;
    }

    // Lógica de reproducción

    shared_ptr<Criatura> reproducirse() override {
        if (puedeReproducir() && rand01() < 0.3) {
            auto hijo = make_shared<RaizLenta>(x, y);
            cout << "RaizLenta se reprodujo!\n";
            daniar(1);
            return hijo;  // La reproducción cuesta energía
        }
        return nullptr; // No se reprodujo
    }

    string getTipo() const override {return "RaizLenta";
    }
};


// Criatura de tipo animal: Centella:
class Centella : public Animal {
public:
    Centella(int x, int y) : Animal("Centella", x, y, 5) {}

    void actuar(vector<shared_ptr<Criatura>>& mundo) override {
        envejecer();
        mover(8, 8);
        // Las Centellas consumen mucha energía al moverse
        if (rand01() < 0.6) daniar(1);

        cout << "Centella en (" << x << "," << y << ") se mueve rapidamente\n";
    }

    void interactuar(vector<shared_ptr<Criatura>>& mundo) override {
        for (auto& otra : mundo) {
            if (otra.get() != this && otra->estaViva() &&
                otra->getTipo() == "RaizLenta" && distanciaA(otra) <= 2.0) {
                if (rand01() < 0.4) {
                    curar(1);
                    otra->daniar(1);
                    cout << "Centella robo energia de RaizLenta!\n";
                }
            }
        }
    }

    // Las Centellas no pueden sobrevivir en agua
    bool puedeSobrevivirEn(Ambiente ambiente) const override {
        return ambiente != Ambiente::AGUA;
    }

    shared_ptr<Criatura> reproducirse() override {
        if (puedeReproducir() && rand01() < 0.4) {
            auto hijo = make_shared<Centella>(x, y);
            cout << "Centella se reprodujo!\n";
            daniar(2);
            return hijo;
        }
        return nullptr;
    }

    string getTipo() const override {
        return "Centella";
    }
};

// Albo hereda de Animal y también implementa las interfaces Volador y Regenerador
class Albo : public Animal, public Volador, public Regenerador {
public:
    Albo(int x, int y) : Animal("Albo", x, y, 6) {}

    void actuar(vector<shared_ptr<Criatura>>& mundo) override {
        envejecer();
        mover(8, 8);

        // Usa sus habilidades especiales con cierta probabilidad

        if (rand01() < 0.5) volar();
        if (rand01() < 0.3) regenerarse();

        cout << "Albo en (" << getX() << "," << getY() << ") actua\n";
    }

    void volar() override {
        cout << "Albo vuela elegantemente\n";
        if (rand01() < 0.4) curar(1);
    }

    void regenerarse() override {
        curar(2);
        cout << "Albo se regenera\n";
    }

    void interactuar(vector<shared_ptr<Criatura>>& mundo) override {
        for (auto& otra : mundo) {
            // Albo ayuda a todas las criaturas cercanas
            if (otra.get() != this && otra->estaViva() && distanciaA(otra) <= 2.0) {
                if (rand01() < 0.3) {
                    curar(1);
                    cout << "Albo ayuda a " << otra->getTipo() << "\n";
                }
            }
        }
    }

    bool puedeSobrevivirEn(Ambiente ambiente) const override {
        return true; // Albo puede sobrevivir en cualquier ambiente
    }

    shared_ptr<Criatura> reproducirse() override {
        if (puedeReproducir() && rand01() < 0.3) {
            auto hijo = make_shared<Albo>(getX(), getY());
            cout << "Albo se reprodujo!\n";
            daniar(2);
            return hijo;
        }
        return nullptr;
    }

    string getTipo() const override { return "Albo"; }

    // Resolver problemas con  herencia múltiple
    // La palabra clave 'using' indica explícitamente qué versión del métodousar
    // cuando hay ambigüedad debido a la herencia múltiple
    using Animal::getX;
    using Animal::getY;
    using Animal::getVida;
    using Animal::curar;
    using Animal::daniar;
    using Animal::distanciaA;
    using Animal::envejecer;
    using Animal::puedeReproducir;
};


class Mapa {
    vector<vector<Ambiente>> grid; // Matriz 2D que representa los ambientes
    //Ultilize el composicion aqui

public:
    // Constructor inicializa el mapa con ambientes aleatorios
    Mapa(int ancho, int alto) {
        grid.resize(alto, vector<Ambiente>(ancho, Ambiente::LLANURA));
        for (int i = 0; i < alto; i++) {
            for (int j = 0; j < ancho; j++) {
                if (rand01() < 0.3) grid[i][j] = Ambiente::DESIERTO;
                else if (rand01() < 0.1) grid[i][j] = Ambiente::AGUA;
            }
        }
    }

    // Muestra el estado actual del mapa en formato ASCII
    void mostrar(const vector<shared_ptr<Criatura>>& criaturas) {
        cout << "\n=== MAPA ===\n";
        vector<vector<char>> gridMostrar(8, vector<char>(8, '.'));


        // Coloca cada criatura viva en su posición correspondiente
        for (auto& criatura : criaturas) {
            if (criatura->estaViva()) {
                int x = criatura->getX();
                int y = criatura->getY();
                // Verifica que la posición esté dentro de los límites
                if (x >= 0 && x < 8 && y >= 0 && y < 8) {
                    // Usa la primera letra del tipo como representación
                    gridMostrar[y][x] = criatura->getTipo()[0];
                }
            }
        }


        // Imprime el grid completo

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                cout << gridMostrar[i][j] << " ";
            }
            cout << "\n";
        }
    }
};


int main() {
    cout << "=== El Mundo de El Valle Iridiano ===\n";

    // Por aca se inicializa el simulacion.

    Mapa mapa(8, 8); // Se crea una mapa de 8x8 con el ambiente aleatorios
    vector<shared_ptr<Criatura>> mundo;  // Contenedor de todas las criaturas

    // Población inicial
    mundo.push_back(make_shared<RaizLenta>(1, 1));
    mundo.push_back(make_shared<RaizLenta>(3, 3));
    mundo.push_back(make_shared<Centella>(2, 2));
    mundo.push_back(make_shared<Centella>(4, 4));
    mundo.push_back(make_shared<Albo>(3, 1));

    for (int ciclo = 1; ciclo <= 14; ciclo++) {
        cout << "\n--- Ciclo " << ciclo << " ---\n";

        // Acciones de las criaturas
        for (auto& criatura : mundo) {
            if (criatura->estaViva()) {
                criatura->actuar(mundo);
            }
        }

        // Interacciones

        for (auto& criatura : mundo) {
            if (criatura->estaViva()) {
                criatura->interactuar(mundo);
            }
        }

        // Reproducción
        vector<shared_ptr<Criatura>> nuevasCriaturas;
        for (auto& criatura : mundo) {
            if (criatura->estaViva()) {
                auto hijo = criatura->reproducirse();
                if (hijo) nuevasCriaturas.push_back(hijo); // Añade nuevo hijo
            }
        }
        mundo.insert(mundo.end(), nuevasCriaturas.begin(), nuevasCriaturas.end());

        // Eliminar criaturas muertas
        vector<shared_ptr<Criatura>> vivas;
        for (auto& criatura : mundo) {
            if (criatura->estaViva()) {
                vivas.push_back(criatura);
            } else {
                cout << criatura->getTipo() << " ha muerto.\n"; // Anuncia muerte
            }
        }
        mundo = vivas;

        mapa.mostrar(mundo);

        // Estadísticas de población
        map<string, int> conteos;
        for (auto& criatura : mundo) {
            conteos[criatura->getTipo()]++;
        }

        cout << "Poblacion: " << mundo.size() << " (";
        // Itera sobre el mapa de conteos para mostrar estadísticas
        for (auto it = conteos.begin(); it != conteos.end(); ++it) {
            cout << it->first << ":" << it->second << " ";
        }
        cout << ")\n";

        // Condición de término: si no quedan criaturas vivas
        if (mundo.empty()) {
            cout << "Todas las criaturas han muerto.\n";
            break;
        }
    }

    cout << "\nSimulacion terminada\n";
    return 0;
}