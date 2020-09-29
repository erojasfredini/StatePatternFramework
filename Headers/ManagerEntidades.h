#pragma once

#include <map>

#include "EntidadBase.h"

struct ManagerEntidades
{
	private:

		ManagerEntidades();

		ManagerEntidades(const ManagerEntidades& cpy);

		ManagerEntidades& operator=(const ManagerEntidades& cpy);

		std::map<unsigned int, EntidadBase*> m_Entidades;

	public:

		//Nos devuelve la unica instancia de la clase
		static ManagerEntidades* Singleton();

		//Obtener una entidad por su numero de ID
		EntidadBase* GetEntidad(unsigned int ID);

		//Elimina una entidad del manager
		//Devuelve true si borro la entidad, devuelve false si no estaba registrada la entidad
		bool DesregistrarEntidad(EntidadBase* pEntidad);

		//Agrega una entidad al manager
		void RegistrarEntidad(EntidadBase* pNuevaEntidad);
};