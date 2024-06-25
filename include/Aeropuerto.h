#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>

using namespace std;

// Definición de la clase Aeropuerto
// En el archivo Aeropuertos.h
class Aeropuerto {
public:
    Aeropuerto(int id, const string& nombre, const string& pais, const string& ciudad)
        : id(id), nombre(nombre), pais(pais), ciudad(ciudad) {}

    int getId() const {
        return id;
    }

    const string& getNombre() const {
        return nombre;
    }

    const string& getPais() const {
        return pais;
    }

    const string& getCiudad() const {
        return ciudad;
    }

private:
    int id;
    string nombre;
    string pais;
    string ciudad;

};
