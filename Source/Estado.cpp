#include "Estado.h"

#include "EntidadBase.h"

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

Estado::Estado()
{
}

Estado::Estado(std::string Nombre): m_Nombre(Nombre)
{
}

Estado::~Estado()
{
	//... no hay ningun recurso que liberar asi que no hacemos nada
}

//-------------------------------------------------------------//
//-----------------		Metodos		---------------------------//
//-------------------------------------------------------------//

std::string Estado::GetNombre()
{
	return m_Nombre;
}

bool Estado::OnMensaje(EntidadBase* pEntidad, const Mensaje& msg)
{
	return false;//por defecto no lo maneja al mensaje
}