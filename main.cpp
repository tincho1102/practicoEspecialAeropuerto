#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include "Grafo.h"

using namespace std;

//Funciones para cargar datos
void cargar_aeropuertos(string path, Grafo<int>& servicio);
void cargar_reservas(string path, Reservas<int>& reservaciones);
void cargar_aeropuertos_rutas(string path, map<string, int>& nueva_aerolinea, string origen, string destino, Reservas<int>& reservaciones);
void cargar_rutas(string path, Grafo<int>& servicio, Reservas<int>& reservaciones);
//Funcion para Listar los aeropuertos
void listarClaves(Grafo<int>& servicio);
// Funcion para el servicio 1
void servicio1(Grafo<int>& servicio);
//Funciones para resolver el servicio 2
template <typename C>
bool verificaAerolinea(const Grafo<C>& servicio, const list<string>& camino, string& aerolineaCamino);

template <typename C>
bool DFS(const Grafo<C>& servicio, const string& origen, const string& destino, list<string>& camino, map<string, bool>& marcados, list<list<string>>& caminosPosibles, string& aerolineaCamino);

template <typename C>
double sumar_distancias(const Grafo<C>& servicio, const list<string>& vertices);

template <typename C>
void servicio2(const Grafo<C>& servicio, const string& origen, const string& destino);

//Funciones para resolver el servicio 3
template <typename C>
bool todosVisitados(const Grafo<C>& servicio, const map<string, bool>& visitados) ;

template <typename C>
void servicio3(const Grafo<C>& servicio, const string& origen);

template <typename C>
bool existeCicloHamiltoniano(const Grafo<C>& servicio, const string& inicio, const string& vertice, map<string, bool>& visitado, list<string>& verticesEncontrados);



int main() {
    // Definicion del grafo y estructura Reservas
    Grafo<int> servicio;
    Reservas<int> reservaciones;
    // dataset, cargar todos los datos
    cargar_reservas("datasets/ReservasServ3.txt", reservaciones);
    cargar_aeropuertos("datasets/AeropuertosServ3.txt", servicio);
    cargar_rutas("datasets/RutasServ3.txt", servicio, reservaciones);
    //
    //presentacion del menú
    int opcion;

    while(true){
        cout << "MENU" << endl;
        cout << "Presione 1 para listar todos los AEROPUERTOS" << endl;
        cout << "Presione 2 para Listar todas las reservas" << endl;
        cout << "Presione 3 para Utilizar el servicio 1: Verificar vuelo directo." << endl;
        cout << "Presione 4 para utilizar el servicio 2: Obtener vuelos de la misma aerolínea." << endl;
        cout << "Presione 5 para utilizar el Servicio 3: Circuito de aeropuertos." << endl;

        cin >> opcion;

        switch (opcion) {
            case 1:
                listarClaves(servicio);
                break;
            case 2:
                reservaciones.imprimirReservasEnArchivo();
                break;
            case 3:{
                servicio1(servicio);
                break;
                }
            case 4:{
                string origen, destino, aerolinea;
                cin.ignore();

                cout << "Origen: ";
                getline(cin, origen);

                cout << "Destino: ";
                getline(cin, destino);

                servicio2(servicio, origen, destino);
                break;
                }
            case 5:{
                string origen;

                cin.ignore();

                cout << "Ingrese un Aeropuerto de origen, y la aplicacion buscara un circuito para pasar una vez por cada aeropuerto:"<<endl;
                cout << "Origen: ";
                getline(cin, origen);

                servicio3(servicio, origen);
            break;
            }
            case 0: {
                cout << "Saliendo del programa." << endl;
                return 0;
                break;
            }
            default:
                cout << "Opcion no valida. Por favor, elija una opcion valida." << endl;
                break;
        }
    }
    return 0;
}


void cargar_reservas(string path, Reservas<int>& reservaciones)
{
    ifstream origen(path.c_str());
    if (!origen.is_open())
        cout << "No se pudo abrir el archivo: " << path << endl;
    else {
        //cout << "RESERVAS " << endl;
        //cout << "------------------------" << endl;
        while (!origen.eof()) {
            string linea;
            getline(origen,linea);
            size_t primera_pos = linea.find(',');
            string nombre_aeropuerto_origen = linea.substr(0, primera_pos);

            size_t segunda_pos = linea.find(',', primera_pos + 1);
            string nombre_aeropuerto_destino = linea.substr(primera_pos + 1, segunda_pos - primera_pos - 1);

            size_t tercera_pos = linea.find(',', segunda_pos + 1);
            string aerolinea = linea.substr(segunda_pos + 1, tercera_pos - segunda_pos - 1);

            string asientos_reservados_texto = linea.substr(tercera_pos + 1);
            int asientos_reservados = atoi(asientos_reservados_texto.c_str());

            reservaciones.agregarReserva(nombre_aeropuerto_origen, nombre_aeropuerto_destino, aerolinea, asientos_reservados);

            //cout <<nombre_aeropuerto_origen << " - " << nombre_aeropuerto_destino << " - " << aerolinea
             //   << " - " << asientos_reservados << endl;
        }
    }
}

void cargar_aeropuertos(string path, Grafo<int>& servicio)
{
    ifstream origen(path.c_str());
    if (!origen.is_open()){
        cout << "No se pudo abrir el archivo: " << path << endl;
    }
    else {
        //cout << "AEROPUERTOS " << endl;
        //cout << "------------------------" << endl;
        while (!origen.eof()) {
            string linea;
            getline(origen,linea);
            size_t primera_pos = linea.find(',');
            string nombre_aeropuerto = linea.substr(0, primera_pos);
            servicio.agregar_vertice(nombre_aeropuerto);
            size_t segunda_pos = linea.find(',', primera_pos + 1);
            string nombre_ciudad = linea.substr(primera_pos + 1, segunda_pos - primera_pos - 1);
            string pais = linea.substr(segunda_pos + 1);

            //cout <<nombre_aeropuerto << " - " << nombre_ciudad << " - " << pais << endl;
        }
    }
}

void cargar_aerolineas_rutas(string aerolineas, map<string, int>& nueva_aerolinea, string origen, string destino, Reservas<int>& reservaciones)
{
    //Se elimina la llave inicial
    aerolineas = aerolineas.substr(1);
    size_t inicial = 0;
    size_t pos = 0;
    size_t pos_final = aerolineas.find('}');
    while ((pos != std::string::npos) && (pos + 1 < pos_final)) {
        pos = aerolineas.find(',', inicial);
        string dato_aerolinea = aerolineas.substr(inicial, pos - inicial);
        size_t pos_separador = dato_aerolinea.find('-');
        string aerolinea = dato_aerolinea.substr(0, pos_separador);
        string asientos_texto = dato_aerolinea.substr(pos_separador + 1);
        int asientos = atoi(asientos_texto.c_str());

        //cout <<"[ " << aerolinea << " - " << asientos << " ]" << endl;
        int asientosReservados = reservaciones.obtenerReservados(origen, destino, aerolinea);
        nueva_aerolinea[aerolinea] = asientos - asientosReservados ;
        inicial = pos + 1;
    }
}

void cargar_rutas(string path, Grafo<int>& servicio, Reservas<int>& reservaciones)
{
    ifstream origen(path.c_str());
    if (!origen.is_open())
        cout << "No se pudo abrir el archivo: " << path << endl;
    else {
        //cout << "RUTAS " << endl;
        //cout << "------------------------" << endl;
        while (!origen.eof()) {
            string linea;
            getline(origen, linea);
            size_t primera_pos = linea.find(',');
            string nombre_aeropuerto_origen = linea.substr(0, primera_pos);

            size_t segunda_pos = linea.find(',', primera_pos + 1);
            string nombre_aeropuerto_destino = linea.substr(primera_pos + 1, segunda_pos - primera_pos - 1);

            size_t tercera_pos = linea.find(',', segunda_pos + 1);
            string distancia_texto = linea.substr(segunda_pos + 1, tercera_pos - segunda_pos - 1);
            double distancia = atof(distancia_texto.c_str());

            size_t cuarta_pos = linea.find(',', tercera_pos + 1);
            string cabotaje_texto = linea.substr(tercera_pos + 1, cuarta_pos - tercera_pos - 1);
            bool cabotaje = false;
            if (cabotaje_texto == "1"){
                cabotaje = true;
            }

            string aerolineas = linea.substr(cuarta_pos + 1);

            map<string, int> nueva_aerolinea;
            cargar_aerolineas_rutas(aerolineas, nueva_aerolinea, nombre_aeropuerto_origen, nombre_aeropuerto_destino, reservaciones);

            servicio.agregar_arco(nombre_aeropuerto_origen, nombre_aeropuerto_destino, distancia, cabotaje, nueva_aerolinea);

        }
    }
}

//listar aeropuertos
void listarClaves(Grafo<int>& servicio){
    string nombre; //este va a ser el nombre del archivo donde se va a guardar
    cout<<"Ingrese el nombre que quiere para el archivo de texto, sin la extension .txt" <<endl;
    cin>> nombre;
    // Agrega la extensión .txt al nombre del archivo
    string nombreArchivoConExtension = nombre + ".txt";
    ofstream archivo(nombreArchivoConExtension);
    if (archivo.is_open()) {
        const auto& mapa = servicio.obtener_grafo();
        cout<< "-----------------------:" << endl;
        cout<< "AEROPUERTOS:" << endl;
        cout<< "-----------------------:" << endl;
        for (const auto& par : mapa) {
            archivo << par.first << endl;
            cout << par.first << endl;
        }
        archivo.close();

    } else {
        cerr << "Error al abrir el archivo: " << nombreArchivoConExtension << endl;
    }
}


// servicio 1
void servicio1(Grafo<int>& servicio) {
    string origen, destino, aerolinea;

    cin.ignore();

    cout << "Origen: ";
    getline(cin, origen);

    cout << "Destino: ";
    getline(cin, destino);

    cout << "Aerolinea: ";
    getline(cin, aerolinea);


    // Buscar vuelo directo
    auto it = servicio.obtener_iterador_vertice(origen);

    if (it != servicio.final_del_grafo()) {
        const list<Grafo<int>::Arco>& arcos = it->second;

        for (const Grafo<int>::Arco& arco : arcos) {
            if (arco.devolver_adyacente() == destino) {
                // Verificar si la aerolínea deseada está en la lista de aerolíneas disponibles
                const auto& aerolineas_asientos = arco.devolver_aerolineas_asientos();
                auto itAerolinea = aerolineas_asientos.find(aerolinea);

                if (itAerolinea != aerolineas_asientos.end()) {
                    // Si existe, muestra la distancia y cantidad de asientos disponibles
                    cout << "Existe un vuelo directo desde " << origen << " a " << destino << " con aerolinea " << aerolinea << "." << endl;
                    cout << "Distancia: " << arco.devolver_dist() << " km" << endl;
                    int cantAsientos = itAerolinea->second;
                    if (cantAsientos > 0){
                       cout << "Asientos Disponibles para reservar: " << cantAsientos << endl;
                    }else{
                        cout << "No hay asientos disponibles para reservar."<< endl;
                    }

                    return;  // Terminar la búsqueda, ya encontramos la ruta
                }
            }
        }

        // Si no se encontró la ruta
        cout << "No se encontro un vuelo directo desde " << origen << " a " << destino << " con aerolinea " << aerolinea << "." << endl;
    } else {
        cout << "No se encontro un vuelo directo desde " << origen << " a " << destino << " con aerolinea " << aerolinea << "." << endl;
    }
}


//servicio 2
template <typename C>
void servicio2(const Grafo<C>& servicio, const string& origen, const string& destino) {
    list<list<string>> caminosPosibles; // Lista de listas para almacenar todos los caminos

    // Inicializar marcadores y camino
    map<string, bool> marcados;
    list<string> camino;
    string aerolineaCamino;
    // Iniciar la búsqueda DFS
    if (!DFS(servicio, origen, destino, camino, marcados, caminosPosibles, aerolineaCamino)) {
        cout << "No se encontraron caminos desde " << origen << " hasta " << destino << "." << endl;
        return;
    }

    // archivo
    string nombre;
    cout << "Ingrese el nombre que quiere para el archivo de texto, sin la extension .txt" << endl;
    cin >> nombre;
    // Agrega la extensión .txt al nombre del archivo
    string nombreArchivoConExtension = nombre + ".txt";
    ofstream archivo(nombreArchivoConExtension);
    //
    if (archivo.is_open()) {

        for (const auto& camino : caminosPosibles) {
            string aerolinea = aerolineaCamino;
            cout << "Para llegar al Aeropuerto de destino, debe pasar por " << camino.size()
                 << " aeropuertos, y usando la Aerolínea: " << aerolinea << endl;
            archivo << "Para llegar al Aeropuerto de destino, debe pasar por " << camino.size()
                    << " aeropuertos, y usando la Aerolínea: " << aerolinea << endl;
            double dist = sumar_distancias(servicio, camino);
            cout << "La distancia total es: " << dist << endl;
            archivo << "La distancia total es: " << dist << endl;
            for (const auto& vertice : camino) {
                cout << vertice << "." << endl;
                archivo << vertice << ", ";
            }
            cout << "Destino alcanzado" << endl;
        }

        // cerrar el arch
        archivo.close();

    } else {
        cerr << "Error al abrir el archivo." << endl;
    }
}


template <typename C>
bool DFS(const Grafo<C>& servicio, const string& origen, const string& destino, list<string>& camino, map<string, bool>& marcados, list<list<string>>& caminosPosibles, string& aerolineaCamino) {
    // Marcar el vértice origen como visitado
    marcados[origen] = true;
    camino.push_back(origen);

    // Si llegamos al destino, agregar el camino a la lista de caminos
    // verifica aerolinea sirve para que agregue solo los caminos que usan una sola aerolínea
    if ((origen == destino) && (verificaAerolinea(servicio, camino, aerolineaCamino))) {
        caminosPosibles.push_back(camino);
        return true; // Se encontró un camino
    } else {
        // Explorar los vértices adyacentes no visitados
        for (const auto& arco : servicio.obtener_grafo().at(origen)) {
            const string& adyacente = arco.devolver_adyacente();
            if (!marcados[adyacente]) {
                if (DFS(servicio, adyacente, destino, camino, marcados, caminosPosibles, aerolineaCamino)) {
                    return true; // Se encontró un camino, terminar la búsqueda
                }
            }
        }
    }

    // Desmarcar el vértice origen antes de retroceder
    marcados[origen] = false;
    camino.pop_back();

    return false; // No se encontró un camino desde este punto
}


//verificaAerolinea sirve para verificar que el camino use siempre la misma aerolinea, y ademas se fija que haya asientos disponibles para viajar
template <typename C>
bool verificaAerolinea(const Grafo<C>& servicio, const list<string>& camino, string& aerolineaCamino) {
    if (camino.empty()) {
        return false; // por si el camino esta vacio
    }

    string aerolinea = ""; // Almacenar la aerolínea actual

    for (auto it = camino.begin(); it != prev(camino.end()); ++it) {
        const string& vueloActual = *it;
        //puntero al iterador
        const string& vueloSiguiente = *next(it);
        //puntero al siguiente

        const list<typename Grafo<C>::Arco>& arcos = servicio.obtener_grafo().at(vueloActual);
        bool aerolineaEncontrada = false;

        for (const typename Grafo<C>::Arco& arco : arcos) {
            if (arco.devolver_adyacente() == vueloSiguiente) {
                // Obtener la aerolínea del arco
                const map<string, int>& aerolineas_asientos = arco.devolver_aerolineas_asientos();

                if (aerolinea.empty()) {
                    // Si aerolinea está vacía, establecerla en la primera aerolínea del primer arco
                    const auto it = aerolineas_asientos.begin();
                    if (it != aerolineas_asientos.end()) {
                        aerolinea = it->first;
                    }
                }

                if ((aerolineas_asientos.find(aerolinea) != aerolineas_asientos.end())&& (aerolineas_asientos.at(aerolinea) > 0) ) {
                    // la aerolínea se encuentra en el arco
                    aerolineaEncontrada = true;
                    aerolineaCamino = aerolinea;
                    break;
                }
            }
        }

        if (!aerolineaEncontrada) {
            return false; // No se encontró la misma aerolínea en un arco
        }
    }

    return true; // Se encontró la misma aerolínea en todos los arcos
}


// recibe un camino y retorna la suma de todas las distancias entre los vertices
//sirve para el servicio 3 tambien
template <typename C>
double sumar_distancias(const Grafo<C>& servicio, const list<string>& vertices) {
    double distancia_total = 0.0;
    auto it = vertices.begin();
    string origen, destino;

    if (it != vertices.end()) {
        origen = *it;
        ++it;
    }

    while (it != vertices.end()) {
        destino = *it;
        // Encuentra el arco entre el origen y el destino
        list<typename Grafo<C>::Arco> adyacentes;
        servicio.devolver_adyacentes(origen, adyacentes);

        for (const typename Grafo<C>::Arco& arco : adyacentes) {
            if (arco.devolver_adyacente() == destino) {
                // Suma la distancia del arco al total
                distancia_total += arco.devolver_dist();
                break;
            }
        }
        origen = destino;
        ++it;
    }
    return distancia_total;
}

// recibe un camino y retorna la suma de todas las distancias entre los vertices
//sirve para el servicio 3 tambien

//
//SERVICIO 3, encontrar un ciclo hamiltoniano, tiene que pasar una vez por cada vertice y llegar al origen
//mostrar  la cantidad de kms del viaje completo y la cantidad de aeropuertos visitados


//todosVisitados sirve como boolean para ver si todos los vertices fueron visitados
template <typename C>
bool todosVisitados(const Grafo<C>& servicio, const map<string, bool>& visitados)  {
    for (const auto& par : visitados) {
        if (!par.second) {
            return false;  // si al menos un vértice no ha sido visitado
        }
    }
    return true;  // todos los vertices fueron visiitados
}

template <typename C>
void servicio3(const Grafo<C>& servicio, const string& origen) {
    map<string, bool> visitados;
    for (const auto& vertice : servicio.obtener_grafo()) {
        visitados[vertice.first] = false;
    }

    visitados[origen] = true;
    list<string> verticesEncontrados;

    if (existeCicloHamiltoniano(servicio, origen, origen, visitados,verticesEncontrados)) {
        verticesEncontrados.push_back(origen);
        verticesEncontrados.push_front(origen);
        cout << "Ruta Para pasar una vez por cada Aeropuerto: ";
        double dist = sumar_distancias(servicio, verticesEncontrados);

        for (const string& vertice : verticesEncontrados) {
            cout << vertice << " ->";
        }
        cout<< "La distancia recorrida sera de: "<< dist << " Kilometros."<<endl;

    } else {
        cout << "No se encontro un ciclo Hamiltoniano." << endl;
    }
}

//backtraking, busca un ciclo que pase por todos los vertices una sola vez
template <typename C>
bool existeCicloHamiltoniano(const Grafo<C>& servicio, const string& inicio, const string& vertice, map<string, bool>& visitado, list<string>& verticesEncontrados) {
    visitado[vertice] = true;

    list<typename Grafo<C>::Arco> adyacentes;
    servicio.devolver_adyacentes(vertice, adyacentes);

    for (const typename Grafo<C>::Arco& arco : adyacentes) {
        const string& adyacente = arco.devolver_adyacente();
        if (adyacente == inicio && todosVisitados(servicio, visitado)) {
            // Encontramos un ciclo hamiltoniano
            return true;
        } else if (!visitado[adyacente]) {
            verticesEncontrados.push_back(adyacente);
            if (existeCicloHamiltoniano(servicio, inicio, adyacente, visitado, verticesEncontrados)) {
                return true;
            }
            verticesEncontrados.pop_back();
        }
    }

    visitado[vertice] = false;
    return false;
}
//


