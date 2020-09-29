#pragma once

#include <Box2D/Box2D.h>

#include "SFML//Graphics.hpp"
#include "SFML//System.hpp"
#include "SFML//Window.hpp"
#include "SFML//Config.hpp"

#include "EntidadBase.h"

//Representa un objeto en la escena
//Basicamente es una union entre la simulacion de Box2D y una representacion usando SFML
//Este objeto sera el responsable de:
//	-Mantener sincronizada la simulacion de 1 cuerpo rigido y SFML
//  -Proporcionar toda la funcionalidad de EntidadBase
//					* Un ID unico de entidad
//					* Una maquina de estado finito
//					* Capacidad de actualizarse
class EntidadEscena: public EntidadBase
{
//protected:
public:

	//Interfaz dibujable de SFML
	sf::Drawable* m_pGrafico;

	//Interfaz transformable de SFML
	sf::Transformable* m_pTransformable;

	//El cuerpo rigido del avatar
	b2Body*    m_pCuerpo;

	//El fixture del cuerpo rigido, se almacena para limpiar el avatar
	//de la escena
	b2Fixture* m_pAdorno;

	//Variables auxiliares, para no alocar memoria en cada frame
	b2Vec2     m_posAux;

	float      m_angAux;

	float      m_BoundingCircleRadio;

public:

	//Parametros:
	//	-Un puntero al cuerpo rigido a mantener
	//	-Un puntero al fixture del cuerpo rigido a mantener
	//	-Un puntero a una image de SFML asociado con este cuerpo rigido
	//	-Un puntero a el mundo de la simulacion de Box2D
	//Nota: pTextura se setea en el sprite asi que debe mantenerse
	EntidadEscena(b2BodyDef& CuerpoDef, const b2FixtureDef& AdornoDef, float escala, sf::Texture* pTextura);

	EntidadEscena(b2BodyDef& CuerpoDef, const b2FixtureDef& AdornoDef, sf::Shape* pForma);

	~EntidadEscena();

	void SetSubRect(sf::IntRect& Rect);

	//Actualiza la entidad
	virtual void Actualizar(float dt);

	b2Transform GetTransformacion() const;

	void GetMarcoReferencia(b2Vec2 &LocalI, b2Vec2 &LocalJ, b2Vec2 &LocalO) const;

	b2Vec2 GetLocalI() const;

	b2Vec2 GetLocalJ() const;

	b2Vec2 GetLocalO() const;

	float GetBoundingCircleRadio() const;

	sf::Vector2f GetPosicion() const;

	float GetAngulo() const;

	sf::Vector2f GetVelocidadLineal() const;

	float GetVelocidadAngular() const;

	b2Body* GetCuerpo();

	void SetPosicion(float x, float y);

	void SetAngulo(float Angulo);

	//Dibuja el avatar
	//Parametros:
	//	-Una referencia al RenderWindow donde dibujar el avatar
	void Dibujar(sf::RenderWindow &RW);

	//Aplica una fuerza sobre el cuerpo
	//Parametros:
	//	-El vector de fuerza a aplicar
	//	-El origen de aplicacion de la fuerza en coordenadas del mundo
	void AplicarFuerzaMundo(const sf::Vector2f& fuerza, const sf::Vector2f& origen = sf::Vector2f(0.0f,0.0f));

	//Aplica una fuerza sobre el cuerpo
	//Parametros:
	//	-El vector de fuerza a aplicar
	//	-El origen de aplicacion de la fuerza en coordenadas locales
	void AplicarFuerzaLocal(const sf::Vector2f& fuerza, const sf::Vector2f& origen = sf::Vector2f(0.0f,0.0f));
};