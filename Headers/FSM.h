#pragma once

#include "Estado.h"
#include "Mensaje.h"

class EntidadBase;

class FSM
{
	private:
	
		//Un puntero al estado actual
		Estado* m_pEstadoActual;

		//Un puntero al estado anterior
		Estado* m_pEstadoAnterior;

		//Un puntero al estado gloabal
		Estado* m_pEstadoGlobal;

		//Un puntero a la entidad que posee esta FSM
		EntidadBase* m_pEntidadDueña;

	public:
		FSM(EntidadBase* EntidadDueña);

		//all entities must implement an update function
		void Actualizar(float dt);

		std::string GetNombreEstadoActual();

		std::string GetNombreEstadoAnterior();

		std::string GetNombreEstadoGlobal();

		//Cambia el estado actual de la MEF(FSM)
		void CambiarEstado(Estado* pNuevoEstado);

		void VolverAlEstadoAnterior();

		bool RecibirMensaje(const Mensaje& msg);
};