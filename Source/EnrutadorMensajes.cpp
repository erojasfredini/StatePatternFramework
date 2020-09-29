#include "EnrutadorMensajes.h"

#include "ManagerEntidades.h"

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

EnrutadorMensajes::EnrutadorMensajes()
{}

EnrutadorMensajes::EnrutadorMensajes(const EnrutadorMensajes& cpy)
{
	this->m_ColaPrioridadMensajes = cpy.m_ColaPrioridadMensajes;
}

EnrutadorMensajes& EnrutadorMensajes::operator=(const EnrutadorMensajes& cpy)
{
	this->m_ColaPrioridadMensajes = cpy.m_ColaPrioridadMensajes;
	return *this;
}

void EnrutadorMensajes::_EnrutarMensaje(Mensaje& msg)
{
	EntidadBase* destinatario = ManagerEntidades::Singleton()->GetEntidad(msg.m_IDDestinatario);

	destinatario->RecibirMensaje(msg);
}

//-------------------------------------------------------------//
//-----------------		Metodos		---------------------------//
//-------------------------------------------------------------//

EnrutadorMensajes* EnrutadorMensajes::Singleton()
{
	static EnrutadorMensajes instancia;
	return &instancia;
}

void EnrutadorMensajes::EnviarMensaje(int TipoMensaje, unsigned int IDDestinatario, unsigned int IDRemitente,
	double retardo, double tiempoAplicacion, void* InformacionExtra)
{
	Mensaje msg(TipoMensaje, IDDestinatario, IDRemitente, tiempoAplicacion+retardo, InformacionExtra);

	//insertamos el mensaje en la cola, donde sera ordenado por m_TiempoEnvio
	m_ColaPrioridadMensajes.insert(msg);
}

void EnrutadorMensajes::ProcesarMensajes(double tiempoAplicacion)
{
	//recorremos los mensajes eliminando los que estan listos para ser enviados
	while( m_ColaPrioridadMensajes.begin()->m_TiempoEnvio <= tiempoAplicacion )
	{
		Mensaje msg = *m_ColaPrioridadMensajes.begin();

		_EnrutarMensaje(msg);

		m_ColaPrioridadMensajes.erase(m_ColaPrioridadMensajes.begin());
	}
}