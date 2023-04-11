#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

vector <int> izquierda_car={9,10,11,4,5,1};
vector <int> derecha_car={13,14,15,7,8,3};

Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;
	int a;
	bool encontrado=false;

	
	if(sensores.terreno[0]==('T'||'S'||'D'||'K'||'X'))
		current_state.comienzo=false;

	if(sensores.reset)
	{	
		power.zapa=false;
		power.biki=false;
		bien_situado=false;
	}
	
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

	if(sensores.posF!=-1 and !bien_situado)
	{
		current_state.fil = sensores.posF;
		current_state.col = sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado=true;
	}

	if(sensores.terreno[0]=='D')
		power.zapa=true;
	else if (sensores.terreno[0]=='K')
		power.biki=true;

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
	if (sensores.terreno[0] == 'X' && sensores.bateria<=3900)
	{
		    accion=actIDLE;
			power.cargado=true;
			cout << "Batería Recargandose: " << sensores.bateria << "/5000" << endl;
	}
	else if((sensores.terreno[2]=='M' && sensores.terreno[1]=='M' && sensores.terreno[3]=='M')||
			(sensores.terreno[2]=='P' && sensores.terreno[1]=='P' && sensores.terreno[3]=='P')|| current_state.encerrado)
	{
		if(!current_state.encerrado)
		{
			current_state.encerrado=true;
			accion=actTURN_SR;
		}
		else
		{
			if(sensores.terreno[2]== 'M'||sensores.terreno[2]== 'P')
			{	
				if(last_action==actTURN_SR)
					accion=actTURN_SR;
				else if(last_action==actTURN_SL)
					accion=actTURN_SL;
				else
				{
					if(rand()%2==0)
						accion=actTURN_SR;
					else
						accion=actTURN_SL;
				}
			}
			else if(((sensores.terreno[1]=='M' || sensores.terreno[3]=='M') || (sensores.terreno[1]=='P' || sensores.terreno[3]=='P'))
					 && sensores.superficie[2]=='_')
			{
				if(puedoAndar(sensores.terreno,sensores.superficie))
						accion=actFORWARD;
			}
			else if((sensores.terreno[1]!='M' && sensores.terreno[5]=='M')  ||(sensores.terreno[1]!='P' && sensores.terreno[5]=='P') )
			{
				accion=actTURN_SL;
				current_state.encerrado=false;
			}
			else if((sensores.terreno[3]!='M' && sensores.terreno[7]=='M')  || ( sensores.terreno[3]!='P' && sensores.terreno[5]=='P'))
			{
				accion=actTURN_SR;
				current_state.encerrado=false;
			}
			else 
			{
				if(sensores.superficie[2]=='_')
				{
					if(sensores.terreno[7]=='M') current_state.encerrado=false;
					
					if( (sensores.terreno[2] == 'T' or sensores.terreno[2] == 'S' or 
						sensores.terreno[2] == 'G' or sensores.terreno[2] == 'K' or
						sensores.terreno[2] == 'D' or sensores.terreno[2] == 'X' or
						(sensores.terreno[2]== 'A' && power.biki) or 
						(sensores.terreno[2] == 'B' && power.zapa)))
						accion=actFORWARD;
				}
				else if(sensores.terreno[3]!='M')
				{
					accion=actTURN_SR;
				}
				else
					accion=actTURN_SL;
				
			}

		}

		if(current_state.contador_encerrado!=20) current_state.contador_encerrado++;
		else 
		{
			current_state.contador_encerrado=0; 
			current_state.encerrado=false;
		}
	}
	else if( (sensores.terreno[2] == 'T' or sensores.terreno[2] == 'S' or 
		 sensores.terreno[2] == 'G' or sensores.terreno[2] == 'K' or
		 sensores.terreno[2] == 'D' or sensores.terreno[2] == 'X' or
		 (sensores.terreno[2]== 'A' and power.biki) or 
		 (sensores.terreno[2] == 'B' and power.zapa) ) and sensores.superficie[2]=='_' and !hayLobos(sensores.superficie))
	{
		int casilla=-1;

		if(!power.biki || !power.zapa || !bien_situado || sensores.bateria <2500)
		{
			for(int i=1; i<16 && !encontrado; i++)
			{
				if((!power.zapa && sensores.terreno[i]=='D') || 
				   (!power.biki && sensores.terreno[i]== 'K') ||
				   (!bien_situado && sensores.terreno[i]=='G')||
				   (sensores.terreno[i] =='X' && sensores.bateria <3000 && !power.cargado))
				{
					casilla=i;
					encontrado=true;
				}
			}
		}
		
		if(casilla!=-1 && !power.girado_zapas)
		{
			if(find(izquierda_car.begin(),izquierda_car.end(),casilla)!=izquierda_car.end())
			{
				accion=actTURN_SL;
				power.girado_zapas=true;
			}
			else if(find(derecha_car.begin(),derecha_car.end(),casilla)!=derecha_car.end())
			{
				accion=actTURN_SR;
				power.girado_zapas=true;
			}
			else
				accion=actFORWARD;
		}
		else
		{

			if (last_action==actFORWARD)
			{
				int aux=rand()%8;
				
				if(aux!=0 && sensores.terreno[2]!='P')accion=actFORWARD;
				else accion=accionAleatoria(sensores.terreno,sensores.superficie);
				
			}
			else
			{
				accion=accionAleatoria(sensores.terreno, sensores.superficie);
			}
			
			power.girado_zapas=false;
		}	
	}
	else if (hayLobos(sensores.superficie))
	{
		girar=(rand()%2==0);
		if(!girar)
			accion=actTURN_BR;
		else 
			accion=actTURN_BL;
		
	}
	else
	{/*
		int aux=rand()%10;
		if (last_action==actFORWARD)
		{

			if(aux!=0 && puedoAndar(sensores.terreno,sensores.superficie))
			{
				accion=actFORWARD;
			}
			else accion=accionAleatoria(sensores.terreno, sensores.superficie);
			
		}
		else */accion=accionAleatoria(sensores.terreno, sensores.superficie);
		
		
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
						matriz[st.fil+c][st.col+d]=terreno[casilla];
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
			casilla++;
			break;
	}

}

bool ComportamientoJugador::hayLobos(const vector<unsigned char> &superficie)
{
	bool lobos=false;

	for(int i=1; i<9 && !lobos; i++)
		if(superficie[i]=='l') lobos=true;

	return lobos;
}

bool ComportamientoJugador::hayAldeanos(const vector <unsigned char> &superficie)
{
	bool aldeano=false;

	for(int i=1; i<9 && !aldeano; i++)
		if(superficie[i]=='l') aldeano=true;

	return aldeano;
}

pair<int,int> ComportamientoJugador::mirarMatriz(const vector<vector<unsigned char>> &terreno)
{
	vector<vector<unsigned char>>::const_iterator inicio=terreno.begin();
	vector<vector<unsigned char>>::const_iterator mitad=terreno.begin()+current_state.fil;

	pair<int,int> cuenta;

	cuenta.first=0;
	cuenta.second=0;

	for(inicio; inicio!=mitad; ++inicio)
	{
		for(vector<unsigned char>::const_iterator i=(*inicio).begin(); i!=(*inicio).end();++i)
			if(*i=='?')cuenta.first++;
	}
	for(mitad+1;mitad!=terreno.end(); ++mitad)
	{
		for(vector<unsigned char>::const_iterator i=(*mitad).begin(); i!=(*mitad).end();++i)
			if(*i=='?')cuenta.second++;
	}

	cout << "Cuenta" << cuenta.first << ":" << cuenta.second << endl;

	return cuenta;
}

Action ComportamientoJugador::accionGuiada(pair<int,int> par, Action accion, const vector<unsigned char> &terreno, const vector<unsigned char> &superficie)
{
	Action accion_g;
	if(par.first<par.second) 
	{
		current_state.sur=true;
		current_state.norte=false;
	}
	else 
	{
		current_state.norte=true;
		current_state.sur=false;
	}

	if(current_state.norte && current_state.brujula==(Orientacion::sureste||Orientacion::suroeste||Orientacion::sur) && !power.girado_zapas)
	{
		cout << "Aqui NORTE" << endl;
		switch (current_state.brujula)
		{
		
			case Orientacion::sureste:
				if((terreno[2]=='P' && terreno[5]=='P')) accion_g=actTURN_SL;
				else 
				{
					if(rand()%2==0)accion_g=actTURN_SR;
					else accion_g=actTURN_BR;
				}
				break;
			case Orientacion::suroeste:
				if((terreno[2]=='P' && terreno[5]=='P')) accion_g=actTURN_SR;
				else 
				{
					if(rand()%2==0)accion_g=actTURN_SL;
					else accion_g=actTURN_BL;
				}
				break;
			case Orientacion::sur:
				if((terreno[1]=='P' && terreno[5]=='P')) accion_g=actTURN_SR;
				else 
				{
					if(rand()%2==0)accion_g=actTURN_BL;
					else accion_g=actTURN_BR;
				}			
		}
		power.girado_zapas=true;
		power.contador=0;
	}
	else if( current_state.sur && current_state.brujula==(Orientacion::noreste||Orientacion::noroeste||Orientacion::norte) && !power.girado_zapas)
	{
		cout << "AQUI SUR" << endl;
		switch (current_state.brujula)
		{
		
			case Orientacion::noreste:
				if((terreno[2]=='P' && terreno[5]=='P')) accion_g=actTURN_SL;
				else 
				{
					if(rand()%2==0)accion_g=actTURN_SR;
					else accion_g=actTURN_BR;
				}
				break;
			case Orientacion::noroeste:
				if((terreno[2]=='P' && terreno[5]=='P')) accion_g=actTURN_SR;
				else 
				{
					if(rand()%2==0)accion_g=actTURN_SL;
					else accion_g=actTURN_BL;
				}
				break;
			case Orientacion::norte:
				if((terreno[1]=='P' && terreno[5]=='P')) accion_g=actTURN_SR;
				else 
				{
					if(rand()%2==0)accion_g=actTURN_BL;
					else accion_g=actTURN_BR;
				}			
		}
		power.girado_zapas=true;
		power.contador=0;
	}
	else
	{
		int aux=rand()%10;

		if(aux!=0 && (terreno[2] == 'T' or terreno[2] == 'S' or 
					terreno[2] == 'G' or terreno[2] == 'K' or
					terreno[2] == 'D' or terreno[2] == 'X' or
					(terreno[2]== 'A' && power.biki) or 
					(terreno[2] == 'B' && power.zapa)) and superficie[2]=='_')
		{
			accion_g=actFORWARD;
		}
		else
		{ 	
			aux=rand()%2;
			switch (aux)
			{

				case 0:
					if(terreno[1]=='P' && terreno[5]=='P')accion_g=actTURN_SR; 
					else accion_g=actTURN_SL;
					break;
				case 1:
					if(terreno[3]=='P' && terreno[7]=='P') accion_g=actTURN_SL; 
					else accion_g=actTURN_SR;
					break;
			}
		}

		power.contador++;

		if(power.contador==8)
			power.girado_zapas=false;
		
	
	}

	current_state.norte=false;
	current_state.sur=false;
	cout << "Accion :" << accion_g << endl;
	return accion_g;

}

Action ComportamientoJugador:: accionAleatoria(const vector<unsigned char> &terreno, const vector <unsigned char> &superficie)
{
	Action accion;
	int aux=rand()%8;
	cout << "ACCION ALEATORIA" << endl;
	switch (aux)
	{

		case 0:
			if(!puedoAndar(terreno,superficie)) accion=actTURN_BR;
			else if( (terreno[1]=='P' && terreno[5]=='P') ||
				(!power.biki && terreno[1]=='A' && terreno[5]=='A') ||
				(!power.zapa && terreno[1]=='B' && terreno[5]=='B')) accion=actTURN_SR; 
			else accion=actTURN_SL;
			break;
		case 1:
			if(!puedoAndar(terreno,superficie)) accion=actTURN_BL;
			else if( (terreno[3]=='P' && terreno[7]=='P')||
				(!power.biki && terreno[3]=='A' && terreno[7]=='A') ||
				(!power.zapa && terreno[3]=='B' && terreno[7]=='B')) accion=actTURN_SL; 
			else accion=actTURN_SR;
			break;

		default:
			if(puedoAndar(terreno,superficie)) accion=actFORWARD;
			break;
	}
	
	return accion;
}

bool ComportamientoJugador:: puedoAndar(const vector<unsigned char> &terreno, const vector <unsigned char> &superficie)
{
	return  ((terreno[2] == 'T' or terreno[2] == 'S' or terreno[2] == 'G' or terreno[2] == 'K' or
		 		 terreno[2] == 'D' or terreno[2] == 'X' or (terreno[2]== 'A' and power.biki) or 
		 		(terreno[2] == 'B' and power.zapa) ) and superficie[2]=='_' and !hayLobos(superficie));
}