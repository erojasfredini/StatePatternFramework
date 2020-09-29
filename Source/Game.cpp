#include "Game.h"

b2World Game::m_World(b2Vec2(0,0),false);

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

Game::Game(sf::VideoMode ModoVideo, std::string TextoVentana, unsigned long EstiloVentana, sf::WindowSettings ConfiguracionVentana, int fps): 
				m_Window(ModoVideo, TextoVentana, EstiloVentana, ConfiguracionVentana),
				m_dt(1.0f/fps)
{
	m_Window.SetFramerateLimit(fps);
	m_Window.Show(true);	
}

//-------------------------------------------------------------//
//-----------------		Metodos		---------------------------//
//-------------------------------------------------------------//

void Game::Iniciar()
{
	this->LoadRecursos();//cargamos los recursos, por ejemplo imagenes, sonidos, fuentes(en este caso solo una imagen, la de la animacion)
	this->CrearEscena();//creamos la escena... en este caso solo consiste en posicionar el sprite de animacion en una posicion

	sf::Event m_Evento;

	while( m_Window.IsOpened() )
	{
		//Atrapamos los eventos para cerra la ventana
		while( m_Window.GetEvent(m_Evento) )
			this->ProcesarEventos(m_Evento);

		this->Actualizar();    //Actualizamos la escena

		this->Dibujar();       //Dibujamos todo el juego

	}
}

void Game::ProcesarEventos(const sf::Event& Evento)
{
	switch( Evento.Type )
	{
	case sf::Event::Closed:
			m_Window.Close();
		break;

	case sf::Event::KeyReleased:
		{
			if( Evento.Key.Code == sf::Key::Escape )
				m_Window.Close();
		}
		break;
	}
}