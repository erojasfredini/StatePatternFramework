#include "Game.h"

b2World Game::m_World(b2Vec2(0,0));

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

Game::Game(sf::VideoMode ModoVideo, std::string TextoVentana, unsigned long EstiloVentana, sf::ContextSettings Contexto, int fps) :
	m_Window(ModoVideo, TextoVentana, EstiloVentana, Contexto),
	m_dt(1.0f / (float)fps)
{
	m_Window.setFramerateLimit(fps);
	m_Window.setVisible(true);
}

//-------------------------------------------------------------//
//-----------------		Metodos		---------------------------//
//-------------------------------------------------------------//

void Game::Iniciar()
{
	this->LoadRecursos();//cargamos los recursos, por ejemplo imagenes, sonidos, fuentes(en este caso solo una imagen, la de la animacion)
	this->CrearEscena();//creamos la escena... en este caso solo consiste en posicionar el sprite de animacion en una posicion

	sf::Event m_Evento;
	sf::Clock clock;

	while( m_Window.isOpen() )
	{
		float dt = clock.getElapsedTime().asSeconds();
		clock.restart();

		//Atrapamos los eventos para cerra la ventana
		while( m_Window.pollEvent(m_Evento) )
			this->ProcesarEventos(m_Evento);

		this->Actualizar(dt);    //Actualizamos la escena

		this->Dibujar(dt);       //Dibujamos todo el juego
	}
}

void Game::ProcesarEventos(const sf::Event& Evento)
{
	switch( Evento.type )
	{
	case sf::Event::Closed:
			m_Window.close();
		break;

	case sf::Event::KeyReleased:
		{
			if( Evento.key.code == sf::Keyboard::Escape )
				m_Window.close();
		}
		break;
	}
}