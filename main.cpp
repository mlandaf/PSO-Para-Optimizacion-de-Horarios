#include <iostream>
#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>

using namespace std;

const int HORAS_POR_DIA = 10;
const int DIAS_POR_SEMANA = 5;

class Profesor {
private:
    int id;
    vector<vector<bool>> disponibilidad;

public:
    Profesor(int id) : id(id), disponibilidad(DIAS_POR_SEMANA, vector<bool>(HORAS_POR_DIA, true)) {}

    void generarDisponibilidadProfesor() {
        for (int i = 0; i < DIAS_POR_SEMANA; ++i) {
            for (int j = 0; j < HORAS_POR_DIA; ++j) {
                disponibilidad[i][j] = true;
            }
        }
    }

    void print() const {
        cout << "Profesor " << id << " disponibilidad:\n";
        for (int i = 0; i < DIAS_POR_SEMANA; ++i) {
            cout << "Dia " << i + 1 << ": ";
            for (int j = 0; j < HORAS_POR_DIA; ++j) {
                cout << disponibilidad[i][j] << " ";
            }
            cout << endl;
        }
    }

    int get_id() const {return id;}
    void set_id(int x) {id = x;}
    const vector<vector<bool>>& get_disponibilidad() const {return disponibilidad;}
};

class Aula {
private:
    int id;
    int capacidad;
    vector<vector<bool>> disponibilidad;

public:
    Aula(int id) : id(id), capacidad(20 + rand() % 30), disponibilidad(DIAS_POR_SEMANA, vector<bool>(HORAS_POR_DIA)) {
        generarDisponibilidadAula();
    }

    void generarDisponibilidadAula() {
        for (int i = 0; i < DIAS_POR_SEMANA; ++i) {
            for (int j = 0; j < HORAS_POR_DIA; ++j) {
                disponibilidad[i][j] = rand() % 2;
            }
        }
    }

    void print() const {
        cout << "Aula " << id << " capacidad: " << capacidad << " disponibilidad:\n";
        for (int i = 0; i < DIAS_POR_SEMANA; ++i) {
            cout << "Dia " << i + 1 << ": ";
            for (int j = 0; j < HORAS_POR_DIA; ++j) {
                cout << disponibilidad[i][j] << " ";
            }
            cout << endl;
        }
    }

    int get_id() const {return id;}
    int get_capacidad() const {return capacidad;}
    void set_id(int x) {id = x;}
    void set_capacidad(int y) {capacidad = y;}
    const vector<vector<bool>>& get_disponibilidad() const {return disponibilidad;}
};

class Clase {
private:
    int id;
    int tamanio;
    int duracion;

public:
    Clase(int id) : id(id) {
        tamanio = 15 + rand() % 35;
        duracion = 1;
    }

    void print() const {
        cout << "Clase " << id << " tamanio: " << tamanio << " duracion: " << duracion << " hora\n";
    }

    int get_id() const {return id;}
    int get_tamanio() const {return tamanio;}
    int get_duracion() const {return duracion;}
    void set_id(int x) {id = x;}
    void set_tamanio(int y) {tamanio = y;}
    void set_duracion(int z) {duracion = z;}
};

class Horario {
private:
    int profesor_id;
    int aula_id;
    int clase_id;
    int dia;
    int hora_inicio;

public:
    Horario(int profesor_id, int aula_id, int clase_id, int dia, int hora_inicio)
        : profesor_id(profesor_id), aula_id(aula_id), clase_id(clase_id), dia(dia), hora_inicio(hora_inicio) {}

    int get_profesor_id() const {return profesor_id;}
    int get_aula_id() const {return aula_id;}
    int get_clase_id() const {return clase_id;}
    int get_dia() const {return dia;}
    int get_hora_inicio() const {return hora_inicio;}
    void set_profesor_id(int x) {profesor_id = x;}
    void set_aula_id(int y) {aula_id = y;}
    void set_clase_id(int z) {clase_id = z;}
    void set_dia(int w) {dia = w;}
    void set_hora_inicio(int r) {hora_inicio = r;}
};

class Particula {
private:
    vector<Horario> posicion;
    vector<Horario> mejor_posicion;
    int mejor_fitness;

public:
    Particula() : mejor_fitness(numeric_limits<int>::max()) {}
    const vector<Horario>& get_posicion() const{return posicion;}
    const vector<Horario>& get_mejor_posicion() const{return mejor_posicion;}
    int get_mejor_fitness() const {return mejor_fitness;}
    void set_posicion(const vector<Horario>& nueva_posicion){posicion = nueva_posicion;}
    void set_mejor_posicion(const vector<Horario>& nueva_mejor_posicion) {mejor_posicion = nueva_mejor_posicion;}
    void set_mejor_fitness(int nuevo_mejor_fitness){mejor_fitness = nuevo_mejor_fitness;}
};


class PSO {
private:
    int num_particulas;
    vector<Profesor> profesores;
    vector<Aula> aulas;
    vector<Clase> clases;
    vector<Particula> enjambre;

public:
    PSO(int num_particulas, const vector<Profesor>& profesores, const vector<Aula>& aulas, const vector<Clase>& clases)
        : num_particulas(num_particulas), profesores(profesores), aulas(aulas), clases(clases) {
        inicializarEnjambre();
    }

    void optimizar(int iteraciones) {
        for (int iter = 0; iter < iteraciones; ++iter) {
            for (auto& particula : enjambre) {
                int fitness = evaluarFitness(particula.get_posicion());
                if (fitness < particula.get_mejor_fitness()) {
                    particula.set_mejor_fitness(fitness);
                    particula.set_mejor_posicion(particula.get_posicion());
                }
            }
        }
    }

    void inicializarEnjambre() {
        srand(static_cast<unsigned int>(time(nullptr)));
        for (int i = 0; i < num_particulas; ++i) {
            Particula particula;
            vector<Horario> posicion;
            for (const auto& clase : clases) {
                Horario horario(rand() % profesores.size(), rand() % aulas.size(), clase.get_id(), rand() % DIAS_POR_SEMANA, rand() % HORAS_POR_DIA);
                posicion.push_back(horario);
            }
            particula.set_posicion(posicion);
            particula.set_mejor_posicion(posicion);
            particula.set_mejor_fitness(evaluarFitness(posicion));
            enjambre.push_back(particula);
        }
    }

    int evaluarFitness(const vector<Horario>& horarios) const {
        int penalizacion = 0;
        for (const auto& horario : horarios) {
            const auto& profesor = profesores[horario.get_profesor_id()];
            const auto& aula = aulas[horario.get_aula_id()];

            if (!profesor.get_disponibilidad()[horario.get_dia()][horario.get_hora_inicio()]) {
                penalizacion += 10;
            }

            if (!aula.get_disponibilidad()[horario.get_dia()][horario.get_hora_inicio()]) {
                penalizacion += 10;
            }

            if (aula.get_capacidad() < clases[horario.get_clase_id()].get_tamanio()) {
                penalizacion += 5;
            }
        }
        return penalizacion;
    }

    void printHorarios() const {
        for (const auto& particula : enjambre) {
            cout << "Mejor Fitness: " << particula.get_mejor_fitness() << endl;
            for (const auto& horario : particula.get_mejor_posicion()) {
                cout << "Profesor: " << horario.get_profesor_id() << " | Aula: " << horario.get_aula_id() + 1 << " | Clase: " << horario.get_clase_id()
                     << " | Dia: " << horario.get_dia() + 1 << " | Hora Inicio: " << horario.get_hora_inicio() + 7
                     << " | Hora Fin: " << horario.get_hora_inicio() + 8 << endl;
            }
            cout << endl;
        }
    }

    void printProfesores() const {
        for (const auto& profesor : profesores) {
            profesor.print();
            cout << endl;
        }
    }

    void printAulas() const {
        for (const auto& aula : aulas) {
            aula.print();
            cout << endl;
        }
    }

    void printClases() const {
        for (const auto& clase : clases) {
            clase.print();
            cout << endl;
        }
    }
};

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    const int NUM_PROFESORES = 30;
    const int NUM_AULAS = 40;
    const int NUM_CLASES = 60;
    const int NUM_PARTICULAS = 30;
    const int ITERACIONES = 1000;

    vector<Profesor> profesores;
    for (int i = 0; i < NUM_PROFESORES; ++i) {
        profesores.push_back(Profesor(i));
    }

    vector<Aula> aulas;
    for (int i = 0; i < NUM_AULAS; ++i) {
        aulas.push_back(Aula(i));
    }

    vector<Clase> clases;
    for (int i = 0; i < NUM_CLASES; ++i) {
        clases.push_back(Clase(i));
    }

    PSO pso(NUM_PARTICULAS, profesores, aulas, clases);

    cout << "Profesores:\n";
    pso.printProfesores();
    cout << "\nAulas:\n";
    pso.printAulas();
    cout << "\nClases:\n";
    pso.printClases();

    pso.optimizar(ITERACIONES);
    pso.printHorarios();

    return 0;
}
