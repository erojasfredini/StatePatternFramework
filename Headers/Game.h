#pragma once

#include "SFML//Graphics.hpp"
#include "SFML//System.hpp"
#include "SFML//Window.hpp"
#include "SFML//Config.hpp"

#include <Box2D//Box2D.h>

//Clase principal del juego, contiene toda la estructura de juego
//Este objeto sera el responsable de:
//	-Crear la escena
//	-Dibujar la pantalla
//	-Actualizar el juego
class Game
{
public:

	//================================================
	/////////	Game Propiedades publicas	 /////////
	//================================================

	//Ventana principal
	sf::RenderWindow m_Window;

	//Mundo de box2d
	static b2World m_World;

	float m_dt;

	std::map<int,sf::Image> m_ManagerDeImagenes;

	//================================================
	/////////////	 Game Metodos		 /////////////
	//================================================

	Game(sf::VideoMode ModoVideo, std::string TextoVentana, unsigned long EstiloVentana, sf::ContextSettings Contexto, int fps);

	//Loop principal del juego
	void Iniciar();

	//Crea la escena
	virtual void CrearEscena()=0;

	//Procesa un evento
	void ProcesarEventos(const sf::Event& Evento);

	//Actualiza la escena
	virtual void Actualizar(const float dt)=0;

	//Dibuja la escena
	virtual void Dibujar(const float dt)=0;

	//Carga los recursos a usar
	virtual void LoadRecursos()=0;
};