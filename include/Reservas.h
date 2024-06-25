#ifndef RESERVAS_H
#define RESERVAS_H

#include <string>
#include <iostream>
#include <map>

using namespace std;

template <typename T>
class Reservas {
public:
    class Datos {
    public:
        Datos() : destino(""), aerolinea(""), reservados(0) {} // Constructor predeterminado

        Datos(const string& destino, const string& aerolinea, int reservados)
            : destino(destino), aerolinea(aerolinea), reservados(reservados) {}
        string devolver_destino() const;
        string devolver_aerolinea() const;
        int devolver_reservados() const;

    private:
        string destino;
        string aerolinea;
        int reservados;
    };

    Reservas();

    void agregarReserva(const string& origen, const string& destino, const string& aerolinea, int reservados);
    void imprimirReservasEnArchivo() const;
    int obtenerReservados(const string& origen, const string& destino, const string& aerolinea) const;

    ~Reservas();

private:
    multimap<string, Datos> datos_reservas;
};

template <typename T>
Reservas<T>::Reservas() {

}
template <typename T> Reservas<T>::~Reservas()
{

}
//

template <typename T> string Reservas<T>::Datos::devolver_destino() const
{
    return destino;
}

template <typename T> string Reservas<T>::Datos::devolver_aerolinea() const
{
    return aerolinea;
}

template <typename T> int Reservas<T>::Datos::devolver_reservados() const
{
    return reservados;
}


template <typename T>
void Reservas<T>::agregarReserva(const string& origen, const string& destino, const string& aerolinea, int reservados)
{
    Datos nueva_reserva(destino, aerolinea, reservados);
    datos_reservas.insert(make_pair(origen, nueva_reserva));
}

template <typename T> void Reservas<T>::imprimirReservasEnArchivo() const {
    //
    string nombreArchivo;
    cout<<"Ingrese el nombre que quiere para el archivo de texto, sin la extension .txt" <<endl;
    cin>> nombreArchivo;
    //
    string archivoTxt = nombreArchivo + ".txt";
    ofstream archivo(archivoTxt);

    if (archivo.is_open()) {
        for (const auto& par : datos_reservas) {
            cout << par.first << ", " << par.second.devolver_destino() << ", " << par.second.devolver_aerolinea() << ", " << par.second.devolver_reservados() << endl;
            archivo << "Origen: " << par.first << ", Destino: " << par.second.devolver_destino() << ", Aerolínea: " << par.second.devolver_aerolinea() << ", Reservados: " << par.second.devolver_reservados() << endl;
        }
        archivo.close();
        cout << "Reservas guardadas en el archivo: " << archivoTxt << endl;
    } else {
        cerr << "Error al abrir el archivo: " << archivoTxt << endl;
    }
}

template <typename T> int Reservas<T>::obtenerReservados(const string& origen, const string& destino, const string& aerolinea) const {
    int totalReservados = 0;

    //  iterador para buscar las reservas que coincidan con el origen, destino y aerolínea
    auto range = datos_reservas.equal_range(origen);

    for (auto it = range.first; it != range.second; ++it) {
        if (it->second.devolver_destino() == destino && it->second.devolver_aerolinea() == aerolinea) {
            totalReservados += it->second.devolver_reservados();
        }
    }

    return totalReservados;
}


#endif
