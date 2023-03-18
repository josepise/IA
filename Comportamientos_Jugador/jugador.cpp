#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include <cmath>

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
	else if(current_state.brujula==norte)
	{
		int buff;
		bool encontrado=false;

		for(int i=0 ; i<12 && encontrado; i++)
		{
			if(sensores.terreno[i]=='G')
			{
				buff=i;
				encontrado=true;
			}
		}
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
		 sensores.terreno[2] == 'G' or sensores.terreno[2] == 'K' or
		 sensores.terreno[2] == 'D' or sensores.terreno[2] == 'X') and sensores.superficie[2] == '_')
	{
		accion=actFORWARD;
	}
	else if (sensores.terreno[0] == 'X' && sensores.bateria!=5000)
	{
		    accion=actIDLE;
			cout << "Batería Recargandose: " << sensores.bateria << "/5000" << endl;
	}
	else if (sensores.terreno[2]== 'M' or (sensores.terreno[1]== 'M' and sensores.terreno[2]== 'M'))
	{
		
		int casillas_n=3, casillas_g=4;
		int muros=0 , espacios=0;
		bool encontrado = false;
		
		if(current_state.brujula%2 == 0 && sensores.terreno[2]!='M')
		{
			
			for(int i =1 ; i<=casillas_n || !encontrado ; i++)
			{
				int var=pow(i,2)+i-1;
				if(sensores.terreno[var]== 'M') muros++;
				else 
				{
					encontrado=true;
					espacios=i;
				}
			}
		}
		else if(current_state.brujula%2 != 0)
		{
			
			for(int i=1; i<=casillas_g || !encontrado ; i++)
			{
				int var=-0.333*pow(i,3)+4*pow(i,2)-8.667*i+6;
				
				if(sensores.terreno[var]=='M') muros++;
				else 
				{
					encontrado=true;
					espacios=var;
					cout << "Sensor NA" << var << endl;
					cout << current_state.brujula << endl;
				}
			}
			
		}
		else
		{
			espacios=-1;
		}

		switch(espacios)
		{
			case 1:
			case -1:
				accion=actTURN_SL;
				break;
			default:
				accion=actFORWARD;
				break;

		}
	}

	else if (!girar_derecha)
	{
		girar_derecha=(rand()%2==0);
		if(!girar_derecha)
			accion=actTURN_SL;
		else 
			accion=actTURN_BL;
	}
	else
	{
		girar_derecha=(rand()%2==0);

		if(!girar_derecha)
			accion=actTURN_SR;
		else 
			accion=actTURN_BR;
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
			for(int d=0; d < 4 ; d++)
			{
				if (d==0) 
				{
					matriz[st.fil][st.col]=terreno[casilla];
					casilla++;
				}
				else{

					for(int c=0; c<d; c++)
					{
						matriz[st.fil-d][st.col+c]=terreno[casilla];
						casilla++;
					}

					for(int f=0; f<=d; f++)
					{
						matriz[st.fil-d+f][st.col+d]=terreno[casilla];
						casilla++;
					}

					
					
				}
			}
			break;
	
		case noroeste:
			for(int d=0; d < 4 ; d++)
			{
				if (d==0) 
				{
					matriz[st.fil][st.col]=terreno[casilla];
					casilla++;
				}
				else{

					for(int c=0; c<d; c++)
					{
						matriz[st.fil-c][st.col-d]=terreno[casilla];
						casilla++;
					}

					for(int f=0; f<=d; f++)
					{
						matriz[st.fil-d][st.col-d+f]=terreno[casilla];
						casilla++;
					}
					
				}
			}
			break;
		
		case sureste:
			for(int d=0; d < 4 ; d++)
			{
				if (d==0) 
				{
					matriz[st.fil][st.col]=terreno[casilla];
					casilla++;
				}
				else{

					for(int c=0; c<d; c++)
					{
						matriz[st.fil+d][st.col+c]=terreno[casilla];
						casilla++;
					}

					for(int f=0; f<=d; f++)
					{
						matriz[st.fil+d][st.col+d-f]=terreno[casilla];
						casilla++;
					}

					
					
				}
			}
			break;
		
		case suroeste:
			for(int d=0; d < 4 ; d++)
			{
				if (d==0) 
				{
					matriz[st.fil][st.col]=terreno[casilla];
					casilla++;
				}
				else{

					for(int c=0; c<d; c++)
					{
						matriz[st.fil+d][st.col-c]=terreno[casilla];
						casilla++;
					}

					for(int f=0; f<=d; f++)
					{
						matriz[st.fil+d-f][st.col-d]=terreno[casilla];
						casilla++;
					}

					
					
				}
			}
			break;
		default:
			matriz[st.fil][st.col]=terreno[0];
			break;
	}

}