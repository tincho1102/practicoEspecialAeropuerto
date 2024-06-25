#ifndef GRAFO_H_
#define GRAFO_H_
#include <map>
#include <vector>
#include <string>
#include <list>
#include <Reservas.h>

using namespace std;

template <typename C> class Grafo
{
public:
    class Arco
    {
    public:
        Arco(const string& destino, double distancia, bool cabotaje, const map<string, int>& aerolineas_asientos);
        string devolver_adyacente() const;
        int devolver_dist() const;
        bool esCabotaje() const;
        const map<string, int>& devolver_aerolineas_asientos() const;

    private:
        string destino;
        double distancia;
        bool cabotaje;
        map<string, int> aerolineas_asientos;
    };
    public:
        typename map<string, list<Arco>>::const_iterator obtener_iterador_vertice(const string& vertice) const {
        return grafo.find(vertice);
        }

        const map<string, list<Arco>>& obtener_grafo() const {
            return grafo;
        }

    public:
    Grafo();
    Grafo(const Grafo& otroGrafo);

    ~Grafo();

    Grafo& operator=(const Grafo& otroGrafo);

    bool esta_vacio() const;
    int devolver_longitud() const;
    bool existe_vertice(const string& vertice) const;
    bool existe_arco(const string& origen, const string& destino) const;
    const C& costo_arco(const string& origen, const string& destino) const;
    void devolver_vertices(list<string>& vertices) const;
    void devolver_adyacentes(const string& origen, list<Arco>& adyacentes) const;
    void agregar_vertice(const string& vertice);
    void eliminar_vertice(const string& vertice);
    void modificar_costo_arco(const string& origen, const string& destino, const C& costo);
    void agregar_arco(const string& origen, const string& destino, double distancia, bool cabotaje, const map<string, int>& aerolineas_asientos);
    void eliminar_arco(const string& origen, const string& destino);

    typename map<string, list<typename Grafo<C>::Arco>>::const_iterator final_del_grafo() const;
    void vaciar();

private:
    map< string, list<Arco>> grafo;
};

//funciones de la clase arco

template <typename C> Grafo<C>::Arco::Arco(const string& dest, double dist, bool cabotaje, const map<string, int>& aerolineas_asientos)
    : destino(dest), distancia(dist), cabotaje(cabotaje), aerolineas_asientos(aerolineas_asientos) {}

template <typename C> string Grafo<C>::Arco::devolver_adyacente() const
{
    return destino;
}

template <typename C> int Grafo<C>::Arco::devolver_dist() const
{
    return distancia;
}

template <typename C> bool Grafo<C>::Arco::esCabotaje() const
{
    return cabotaje;
}

template <typename C> const map<string, int>& Grafo<C>::Arco::devolver_aerolineas_asientos() const {
    return aerolineas_asientos;
}

//funciones de la clase Grafo

template <typename C> Grafo<C>::Grafo()
{

}

template <typename C> Grafo<C>::Grafo(const Grafo& otroGrafo)
{

}

template <typename C> Grafo<C>::~Grafo()
{
    grafo.clear();
}

template <typename C> Grafo<C>& Grafo<C>::operator=(const Grafo& otroGrafo)
{
    // Implementa el operador de asignación según tus necesidades
    return *this;
}

template <typename C> bool Grafo<C>::esta_vacio() const
{
    return (grafo.empty());
}

template <typename C> int Grafo<C>::devolver_longitud() const
{
    return grafo.size();
}

template <typename C> bool Grafo<C>::existe_vertice(const string& vertice) const
{
    bool existe = false;
    typename map<string, list<Arco>>::const_iterator it = grafo.find(vertice);
    existe = (it != grafo.end());


    return existe;
}

template <typename C> bool Grafo<C>::existe_arco(const string& origen, const string& destino) const
{
    bool existe = false;
    typename map<string, list<Arco>>::const_iterator itm = grafo.find(origen);
    if(itm != grafo.end())
    {
        typename list<Arco>::const_iterator itl = itm->second.begin();
        while(!existe && (itl != itm->second.end()))
        {
            if(itl->devolver_adyacente() == destino)
                existe = true;
            itl++;
        }
    }

    return existe;
}

template <typename C> const C& Grafo<C>::costo_arco(const string& origen, const string& destino) const
{
    typename map<string, list<Arco>>::const_iterator it = grafo.find(origen);
    if(it != grafo.end())
    {
        typename list<Arco>::const_iterator itl = it->second.begin();
        while(itl != it->second.end())
        {
            if(itl->devolver_adyacente() == destino)
                return itl->devolver_dist();
            itl++;
        }
    }
}

template <typename C> void Grafo<C>::devolver_vertices(list<string>& vertices) const
{
    typename map< string, list<Arco> >::const_iterator it = grafo.begin();
    while (it != grafo.end())
    {
        vertices.push_back(it->first);
        it++;
    }
}

template <typename C> void Grafo<C>::devolver_adyacentes(const string& origen, list<Arco>& adyacentes) const
{
    typename map<string, list<Arco>>::const_iterator it = grafo.find(origen);
    if(it != grafo.end())
        adyacentes = it->second;
}

template <typename C> void Grafo<C>::agregar_vertice(const string& vertice)
{
    grafo.insert({vertice, list<Arco>()});
}

template <typename C> void Grafo<C>::eliminar_vertice(const string& vertice)
{
    if (existe_vertice(vertice)) {
        grafo.erase(vertice);
    }
}

template <typename C> void Grafo<C>::modificar_costo_arco(const string& origen, const string& destino, const C& costo)
{

}

template <typename C> void Grafo<C>::agregar_arco(const string& origen, const string& destino, double distancia, bool cabotaje, const map<string, int>& aerolineas_asientos)
{
    if (existe_vertice(origen) && existe_vertice(destino)) {
        Arco nuevo_arco(destino, distancia, cabotaje, aerolineas_asientos);
        grafo.at(origen).emplace_back(nuevo_arco);
    }
}

template <typename C> void Grafo<C>::eliminar_arco(const string& origen, const string& destino)
{
    if (existe_arco(origen, destino)) {
        grafo.at(origen) = Arco("", 0, false);
    }
}

template <typename C> void Grafo<C>::vaciar()
{
    grafo.clear();
}

template <typename C>
typename map<string, list<typename Grafo<C>::Arco>>::const_iterator Grafo<C>::final_del_grafo() const {
    return grafo.end();
}


#endif /* GRAFO_H_ */
