#include "EntidadBase.h"

unsigned int EntidadBase::m_ProximoIDValido = 0;

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

EntidadBase::EntidadBase(): m_MaquinaDeEstadoFinito(this)
{
	m_ID = EntidadBase::m_ProximoIDValido++;
}

EntidadBase::~EntidadBase()
{
}

//-------------------------------------------------------------//
//-----------------		Metodos		---------------------------//
//-------------------------------------------------------------//

void EntidadBase::Actualizar(float dt)
{
	m_MaquinaDeEstadoFinito.Actualizar(dt);
}
/*
sf::Vector2f EntidadBase::GetPosicion() const
{
	return m_Posicion;
}*/

unsigned int EntidadBase::GetID() const
{
	return m_ID;
}

FSM& EntidadBase::GetFSM()
{
	return m_MaquinaDeEstadoFinito;
}

bool EntidadBase::RecibirMensaje(const Mensaje& msg)
{
	//enviamos el mensaje a la FSM para que intente que sus estados lo tomen y procesen
	return m_MaquinaDeEstadoFinito.RecibirMensaje(msg);
}