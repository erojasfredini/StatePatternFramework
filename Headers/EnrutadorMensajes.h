#pragma once

#include <set>

#include "Mensaje.h"

struct EnrutadorMensajes
{
	private:

		EnrutadorMensajes();

		EnrutadorMensajes(const EnrutadorMensajes& cpy);

		EnrutadorMensajes& operator=(const EnrutadorMensajes& cpy);

		std::set<Mensaje> m_ColaPrioridadMensajes;

		//Envia un mensaje a su destinatario
		//Este metodo se llamara para enviar efectivamente un mensaje antes de ser eliminado de la cola
		void _EnrutarMensaje(Mensaje& msg);

	public:

		//Nos devuelve la unica instancia de la clase
		static EnrutadorMensajes* Singleton();

		//Agregar un mensaje para ser enviado
		void EnviarMensaje(int TipoMensaje, unsigned int IDDestinatario, unsigned int IDRemitente, double retardo, double tiempoAplicacion, void* InformacionExtra);

		//Esta funcion debe ser ejecutada en el loop y es la cual envia los mensajes una vez
		//que llego su tiempo de envio
		void ProcesarMensajes(double tiempoAplicacion);
};