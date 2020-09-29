#pragma once

#include "EntidadEscena.h"
#include "SteeringBehaviors.h"

//Representa un objeto de la escena que puede tener steeringbehaviors
class Vehiculo: public EntidadEscena
{
private:

	SteeringBehaviors m_Comportamiento;

public:

	float m_FuerzaSteeringMax;

	float m_VelocidadMax;

	Vehiculo(b2BodyDef& CuerpoDef, const b2FixtureDef& AdornoDef, float escala, sf::Texture* pTextura);

	//Actualiza la entidad
	virtual void Actualizar(float dt);

	float GetFuerzaSteeringMax() const;

	float GetVelocidadMax() const;

	sf::Vector2f GetDireccion() const;

	SteeringBehaviors& GetSteeringBehaviors();

};