#pragma once

#include <string>

#include "Mensaje.h"

class EntidadBase;

class Estado
{
	protected:

		std::string m_Nombre;

	public:
		Estado();

		Estado(std::string Nombre);

		virtual ~Estado();

		std::string GetNombre();

		//this will execute when the state is entered
		virtual void Entrar(EntidadBase* pEntidad)=0;

		//this is called by the miner’s update function each update step
		virtual void Ejecutar(EntidadBase* pEntidad)=0;

		//this will execute when the state is exited
		virtual void Salir(EntidadBase* pEntidad)=0;

		//Recibe un mensaje e intenta manejarlo
		virtual bool OnMensaje(EntidadBase* pEntidad, const Mensaje& msg);
};