#pragma once

#include "SFML//Graphics.hpp"
#include "SFML//System.hpp"
#include "SFML//Window.hpp"
#include "SFML//Config.hpp"

//SOLO PARA DEBUG
#include <Box2D/Box2D.h>

class Vehiculo;

class EntidadEscena;

//La tolerancia que se calculara como =- mirandose, es decir que
//el numero debera ser positivo y si se elige 10 seran 10 grados de tolerancia 
//para cada lado
#define TOLERANCIA_MIRANDOSE_GRADOS 10


//Representa los comportamientos de un Vehiculo
class SteeringBehaviors
{
private:

	enum {	NadaBit             = 0x00000,
			BuscarBit           = 0x00002,
			EscaparBit          = 0x00004,
			ArribarBit          = 0x00008,
			InterceptarBit      = 0x00010,
			EvadirBit			= 0x00020,
			DeambularBit        = 0x00040,
			EvadirObstaculosBit = 0x00080,
			EvadirParedesBit    = 0x00100,
			EsconderseBit       = 0x00200
		  };

	enum Desaceleracion{ Lento  = 3,
						 Normal = 2,
						 Rapido = 1
						};


	int m_ComportamientosActivos;

	Vehiculo* m_pVehiculoDueño;

	sf::Vector2f m_FuerzaComportamiento;

	sf::Vector2f Buscar(const sf::Vector2f& PosObjetivo);

	sf::Vector2f Escapar(const sf::Vector2f& PosPeligro);

	sf::Vector2f Arribar(const sf::Vector2f& PosObjetivo, Desaceleracion Tipo);

	sf::Vector2f Interceptar(const Vehiculo& Acechado);

	sf::Vector2f Evadir(const Vehiculo& Acechador);

	sf::Vector2f Deambular();

	sf::Vector2f EvadirObstaculos(std::vector<EntidadEscena*> &Obstaculos);
public:
	struct Pared;
private:

	sf::Vector2f EvadirParedes(std::vector<Pared*> &Paredes);

	sf::Vector2f EsconderseGetPosicionEscondite(const sf::Vector2f &posObstaculo, const float radioObstaculo, const sf::Vector2f &posPeligro);
	sf::Vector2f Esconderse(const sf::Vector2f& PosPeligro, std::vector<EntidadEscena*> &Obstaculos);

	sf::Vector2f CalcularSumaTruncada();
	sf::Vector2f CalcularSumaTruncadaPriorizada();

public:

	float m_PesoBuscar;
	float m_PesoEscapar;
	float m_PesoArribar;
	float m_PesoInterceptar;
	float m_PesoEvadir;
	float m_PesoDeambular;
	float m_PesoEvadirObstaculos;
	float m_PesoEvadirParedes;
	float m_PesoEsconderse;

	sf::Vector2f m_PosObjetivo;

	Vehiculo* m_pPresa;

	Vehiculo* m_pAcechador;

	//SOLO PARA DEBUG
	sf::Vector2f m_InterceptarPosicionPredicion;

	//SOLO PARA DEBUG
	sf::Vector2f m_EvadirPosicionPredicion;

	float m_DeambularRuido;
	float m_DeambularDistancia;
	float m_DeambularRadio;
	sf::Vector2f m_DeambularObjetivo;

	float m_EvadirObstaculosRectanguloMin;
	//SOLO PARA DEBUG
	b2Vec2 m_EvadirObstaculosPuntoCercaColision;
	//SOLO PARA DEBUG
	b2Vec2 m_EvadirObstaculosPuntoObjetoColision;
	//SOLO PARA DEBUG
	float m_EvadirObstaculosRadioExpandido;
	std::vector<EntidadEscena*>* m_pObstaculos;

	//Estos sensores que corresponden al comportamiento de EvadorParedes
	//consisten en un vector de direccion que servira para lanzar un rayo
	//por sensor en la direccion que especifica el usuario
	struct Sensores
	{
			Sensores(Vehiculo* pCaster, float angulo);
			Sensores();
			
			sf::Vector2f GetOrigen() const;
			sf::Vector2f GetDireccion() const;
			sf::Vector2f Evaluar(float t) const;
			bool TestColision(const sf::Vector2f &Normal, const sf::Vector2f &Punto, float &tColision) const;
			float DistPuntoPlano(const sf::Vector2f &Normal, const sf::Vector2f &Punto, const sf::Vector2f &Q) const;
		private:
			Vehiculo* m_pCaster;
			float m_Angulo;
	};
	std::vector<Sensores> m_Sensores;

	//SOLO PARA DEBUG
	std::vector<sf::Vector2f> m_EvadirParedesPuntoColision;

	//Representa una normal
	//  0=(P-P1)*N
	// -d=P*N
	struct Pared
	{
		Pared(sf::Vector2f Normal, sf::Vector2f m_Punto);
		sf::Vector2f m_Normal;
		sf::Vector2f m_Punto;
	};
	std::vector<Pared*>* m_pParedes;

	//SOLO PARA DEBUG
	std::vector<sf::Vector2f> m_EscondersePuntoEscondito;

	//Parametros:
	//	-Un puntero al vehiculo el cual posee los comportamiento de steering de este objeto
	SteeringBehaviors(Vehiculo* pVehiculoDueño);

	enum TipoDeCombinacionDePatrones{SumaTruncada,SumaTruncadaPriorizada};
	TipoDeCombinacionDePatrones m_TipoCombinacion;
	sf::Vector2f Calcular();

	bool AgregarFuerza(const sf::Vector2f &fuerzaAgregar);

	void BuscarOn();
	void BuscarOff();

	void EscaparOn();
	void EscaparOff();

	void ArribarOn();
	void ArribarOff();

	void InterceptarOn();
	void InterceptarOff();

	void EvadirOn();
	void EvadirOff();

	void DeambularOn();
	void DeambularOff();

	void EvadirObstaculosOn();
	void EvadirObstaculosOff();

	void EvadirParedesOn();
	void EvadirParedesOff();

	void EsconderseOn();
	void EsconderseOff();

	bool isBuscarOn() const;
	bool isEscaparOn() const;
	bool isArribarOn() const;
	bool isInterceptarOn() const;
	bool isEvadirOn() const;
	bool isDeambularOn() const;
	bool isEvadirObstaculosOn() const;
	bool isEvadirParedesOn() const;
	bool isEsconderseOn() const;
};