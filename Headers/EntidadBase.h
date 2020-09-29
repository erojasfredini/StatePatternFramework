#pragma once

#include "SFML//Graphics.hpp"

#include "FSM.h"

#include "Mensaje.h"

class EntidadBase
{
	private:
		//Numero de identificacion unico de cada entidad
		//se asigna automaticamente en el constructor
		unsigned int m_ID;

		//El proximo numero de identificacion valido
		static unsigned int m_ProximoIDValido;

	protected:

		//La posicion global de la entidad en la escena
		//sf::Vector2f m_Posicion;

		//La maquina de estado finito de la entidad
		FSM m_MaquinaDeEstadoFinito;

	public:
		EntidadBase();

		virtual ~EntidadBase();

		//Actualiza la entidad
		virtual void Actualizar(float dt);

		//sf::Vector2f GetPosicion() const;

		unsigned int GetID() const;

		FSM& GetFSM();

		bool RecibirMensaje(const Mensaje& msg);
};