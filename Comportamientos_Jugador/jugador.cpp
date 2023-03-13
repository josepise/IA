#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;



Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;
	int a;

	switch (last_action)
	{
		case actFORWARD:
			switch (current_state.brujula)
			{
				case norte: current_state.fil--; break;
				case noreste: current_state.fil--; current_state.col++; break;
				case este: current_state.col++;break;
				case sureste: current_state.fil++; current_state.col++; break;
				case sur: current_state.fil++; break;
				case suroeste: current_state.fil++; current_state.col--; break;
				case oeste: current_state.col--; break;
				case noroeste: current_state.fil--; current_state.col--; break;
			}
			break;

		case actTURN_SL:
			a=current_state.brujula;
			a=(a+7)%8;
			current_state.brujula=static_cast<Orientacion>(a);
			break;

		case actTURN_SR:
			a=current_state.brujula;
			a=(a+1)%8;
			current_state.brujula=static_cast<Orientacion>(a);
			break;
		
		case actTURN_BL:
			a=current_state.brujula;
			a=(a+5)%8;
			current_state.brujula=static_cast<Orientacion>(a);
			break;

		case actTURN_BR:
			a=current_state.brujula;
			a=(a+3)%8;
			current_state.brujula=static_cast<Orientacion>(a);
			break;

	}

	if(sensores.terreno[0]=='G' and !bien_situado)
	{
		current_state.fil = sensores.posF;
		current_state.col = sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado=true;
	}

	if(bien_situado)
	{
	
		PonerTerrenoEnMatriz(sensores.terreno, current_state, mapaResultado);
	}


	cout << "Posicion: fila " << sensores.posF << " columna " << sensores.posC << " ";
	switch(sensores.sentido){
		case 0: cout << "Norte" << endl; break;
		case 1: cout << "Noreste" << endl; break;
		case 2: cout << "Este" << endl; break;
		case 3: cout << "Sureste" << endl; break;
		case 4: cout << "Sur " << endl; break;
		case 5: cout << "Suroeste" << endl; break;
		case 6: cout << "Oeste" << endl; break;
		case 7: cout << "Noroeste" << endl; break;
	}

	//Decidir la nueva acción
	if( (sensores.terreno[2] == 'T' or sensores.terreno[2] == 'S' or 
		 sensores.terreno[2] == 'G') and sensores.superficie[2] == '_')
	{
		accion=actFORWARD;
	}
	else if (!girar_derecha)
	{
		accion=actTURN_SL;
		girar_derecha=(rand()%2==0);
	}
	else
	{
		accion=actTURN_SR;
		girar_derecha=(rand()%2==0);
	}

	cout << "Terreno: ";

	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];
	cout << endl;

	cout << "Superficie: ";
	for (int i=0; i<sensores.superficie.size(); i++)
		cout << sensores.superficie[i];
	cout << endl;

	cout << "Colisión: " << sensores.colision << endl;
	cout << "Reset: " << sensores.reset << endl;
	cout << "Vida: " << sensores.vida << endl;
	cout << endl;


	// Determinar el efecto de la ultima accion enviada
	last_action=accion;
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}

void ComportamientoJugador:: PonerTerrenoEnMatriz(const vector<unsigned char> &terreno, const state &st,
						  vector < vector< unsigned char >> & matriz)
{
	int casilla=0;

	switch (st.brujula)
	{
		case norte:
			
			for(int i=0; i <4; i++)
			{
				for(int e=-i; e<=i; e++)
				{
					matriz[st.fil-i][st.col+e]=terreno[casilla];
					casilla++;
				}
			}
			break;
		
		case sur:
			for(int i=0; i<4; i++)
			{
				for(int e=i; e>=(-i); e--)
				{
					matriz[st.fil+i][st.col+e]=terreno[casilla];
					casilla++;
				}
			}
			break;

		case este:
			for(int e=0; e<4; e++)
			{
				for(int i=-e; i<=e; i++)
				{
					matriz[st.fil+i][st.col+e]=terreno[casilla];
					casilla++;
				}
			}
			break;

		case oeste:
			for(int e=0; e<4; e++)
			{
				for(int i=e; i>=-e; i--)
				{
					matriz[st.fil+i][st.col-e]=terreno[casilla];
					casilla++;
				}
			}
			break;

		case noreste:
			for(int d=0; d<4; d++)
			{
				if(d!=0)
				{
					for(int e=0; e<=d; e++)
					{
						matriz[st.fil-d][st.col+e]=terreno[casilla];
						casilla++;
					}
					for(int i=d; i>=st.fil ; i--)
					{
						matriz[st.fil-i][st.col+d]=terreno[casilla];
						casilla++;
					}
				}
				else
				{
					matriz[st.fil][st.col]=terreno[casilla];
					casilla++;
				}
			}
		default:
			matriz[st.fil][st.col]=terreno[0];
			break;
	}

}