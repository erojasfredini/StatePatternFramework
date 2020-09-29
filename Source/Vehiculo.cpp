#include "Vehiculo.h"

#include <iostream>

#include "Matematica.h"

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

Vehiculo::Vehiculo(b2BodyDef& CuerpoDef, const b2FixtureDef& AdornoDef, float escala, sf::Texture* pTextura):
					EntidadEscena(CuerpoDef, AdornoDef, escala, pTextura), m_Comportamiento(this)
{
	//Linkeado al Vehiculo
	// El Link esta a la EntidadEscena
	//m_pCuerpo->SetUserData((void*)this);
	//m_pAdorno->SetUserData((void*)tdhis);
}

//-------------------------------------------------------------//
//-----------------		Metodos		---------------------------//
//-------------------------------------------------------------//

void Vehiculo::Actualizar(float dt)
{
	EntidadEscena::Actualizar(dt);

	sf::Vector2f fuerzaComportamiento = m_Comportamiento.Calcular();

	AplicarFuerzaLocal(fuerzaComportamiento);

	//update the heading if the vehicle has a velocity greater than a very small
	//value
	if( m_pCuerpo->GetLinearVelocity().Length() > 0.00000001f )
	{
		//limitamos la velocidad lineal maxima
		if(m_pCuerpo->GetLinearVelocity().Length() > GetVelocidadMax())
			m_pCuerpo->SetLinearVelocity(b2Vec2(m_pCuerpo->GetLinearVelocity().x/m_pCuerpo->GetLinearVelocity().Length()*GetVelocidadMax(),
												m_pCuerpo->GetLinearVelocity().y/m_pCuerpo->GetLinearVelocity().Length()*GetVelocidadMax()) );

		sf::Vector2f adelante = GetDireccion();
		sf::Vector2f haciaMovimiento = GetVelocidadLineal();
		haciaMovimiento /= Norma(haciaMovimiento);
		float dir = Cruz2D(adelante, haciaMovimiento);
		float vel = Dot(adelante, haciaMovimiento);

		vel = acos(clamp(vel, -1.0f, 1.0f));

		if( dir < 0)
			m_pCuerpo->SetAngularVelocity(-1*vel/3.14f);
		else
			m_pCuerpo->SetAngularVelocity(1*vel/3.14f);
		//if( dir > 0.0f )//contrario a agujas del reloj
			//m_pCuerpo->SetAngularVelocity(-dir);
		//else//agujas del reloj
			//m_pCuerpo->SetAngularVelocity(dir);
	}
}

float Vehiculo::GetFuerzaSteeringMax() const
{
	return m_FuerzaSteeringMax;
}

float Vehiculo::GetVelocidadMax() const
{
	return m_VelocidadMax;
}

sf::Vector2f Vehiculo::GetDireccion() const
{
	b2Transform T = m_pCuerpo->GetTransform();
	b2Vec2 adelante(1.0f, 0.0f);
	adelante = b2Mul(T.q, adelante);//transformamos con la rotacion del mundo
	adelante.Normalize();//no hace falta... pero quien me va a culpar por no tener fe ciega en las matematicas :P
	return sf::Vector2f(adelante.x, adelante.y);
}

SteeringBehaviors& Vehiculo::GetSteeringBehaviors()
{
	return m_Comportamiento;
}