#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
using namespace std;

struct state
{
  int fil;
  int col;
  Orientacion brujula;
  bool encerrado;
  int contador_encerrado;
  bool norte;
  bool sur;
  bool comienzo;
  int tamano;
};

struct power_up
{
  bool zapa;
  bool biki;
  bool girado_zapas;
  int contador;
  bool cargado;
};

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado
      last_action=actIDLE;
      current_state.brujula=norte;
      current_state.fil=current_state.col=99;
      current_state.encerrado=false;
      girar=false;
      power.zapa=false;
      power.biki=false;
      current_state.comienzo=true;
      current_state.contador_encerrado=0;
      current_state.tamano=size;
      power.contador=0;
      power.cargado=false;
      power.girado_zapas=false;
      bien_situado=false;
      current_state.norte=false;
      current_state.sur=false;
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);

    //Función encargada del pintado de la matriz con lo detectado con los sensores
    void PonerTerrenoEnMatriz(const vector<unsigned char> &terreno, const state &st,
						  vector < vector< unsigned char >> & matriz);

    //Función encargada de detectar si existen lobos en la superficie.
    bool hayLobos(const vector<unsigned char> &superficie);
    
    //Función encargada de detectar si existen aldeano en la superficie.
    bool hayAldeanos(const vector <unsigned char> &superficie);
    
    //Genera una acción aleatoria dependiendo del terreno y la superficie tomada con los sensores.
    Action accionAleatoria(const vector <unsigned char> &terreno, const vector<unsigned char> &superficie);

    //Comprueba si la casilla enfrente del robot es válida para poder moverse
    bool puedoAndar(const vector<unsigned char> &terreno, const vector <unsigned char> &superficie);


    //pair<int,int> mirarMatriz(const vector<vector<unsigned char>> &terreno);
    //Action accionGuiada(pair<int,int> par, Action accion,const vector<unsigned char> &terreno, const vector<unsigned char> &superficie);
  private:
  
  // Declarar aquí las variables de estado
  Action last_action;
  state current_state;
  bool girar;
  bool bien_situado;
  power_up power;

};

#endif
