#ifndef VUELOS_H
#define VUELOS_H

#include <iostream>
#include <list>
#include <map>

class Vuelo {
public:
    Vuelo(int id, int origen, int destino, double distancia, bool cabotaje)
        : id(id), origen(origen), destino(destino), distancia(distancia), cabotaje(cabotaje) {}

    int getId() const {
        return id;
    }

    int getOrigen() const {
        return origen;
    }

    int getDestino() const {
        return destino;
    }

    double getDistancia() const {
        return distancia;
    }

    bool esCabotaje() const {
        return cabotaje;
    }

private:
    int id;
    int origen;
    int destino;
    double distancia;
    bool cabotaje;
};

#endif // VUELOS_H
