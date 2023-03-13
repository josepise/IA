#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
using namespace std;

struct state
{
  int fil;
  int col;
  Orientacion brujula;
  
};

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado
      last_action=actIDLE;
      current_state.brujula=norte;
      girar_derecha=false;
      bien_situado=false;
      current_state.fil=current_state.col=99;
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void PonerTerrenoEnMatriz(const vector<unsigned char> &terreno, const state &st,
						  vector < vector< unsigned char >> & matriz);

  private:
  
  // Declarar aquí las variables de estado
  Action last_action;
  state current_state;
  bool girar_derecha;
  bool bien_situado;


};

#endif
