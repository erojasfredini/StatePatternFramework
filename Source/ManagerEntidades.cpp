#include "ManagerEntidades.h"

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

ManagerEntidades::ManagerEntidades()
{}

ManagerEntidades::ManagerEntidades(const ManagerEntidades& cpy)
{
	this->m_Entidades = cpy.m_Entidades;
}

ManagerEntidades& ManagerEntidades::operator=(const ManagerEntidades& cpy)
{
	this->m_Entidades = cpy.m_Entidades;
	return *this;
}

//-------------------------------------------------------------//
//-----------------		Metodos		---------------------------//
//-------------------------------------------------------------//

ManagerEntidades* ManagerEntidades::Singleton()
{
	static ManagerEntidades instancia;
	return &instancia;
}

EntidadBase* ManagerEntidades::GetEntidad(unsigned int ID)
{
	std::map<unsigned int, EntidadBase*>::iterator res = m_Entidades.find(ID);
	if( res != m_Entidades.end() )
		return res->second;//devolvemos el puntero a la entidad
	else
		return NULL;//no existe esa entidad
}

bool ManagerEntidades::DesregistrarEntidad(EntidadBase* pEntidad)
{
	std::map<unsigned int, EntidadBase*>::iterator res = m_Entidades.find( pEntidad->GetID() );
	if( res != m_Entidades.end() )
	{
		m_Entidades.erase( res );
		return true;
	}else
		return false;
}

void ManagerEntidades::RegistrarEntidad(EntidadBase* pNuevaEntidad)
{
	m_Entidades[pNuevaEntidad->GetID()] = pNuevaEntidad;
}