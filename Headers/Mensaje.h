#pragma once

struct Mensaje
{
	//Identificador del tipo de mensaje que representa la instancia
	int m_TipoMensaje;

	//El ID de la entidad que recibira el mensaje
	unsigned int m_IDDestinatario;

	//El ID de la entidad que envia el mensaje
	unsigned int m_IDRemitente;

	//El lapso en el que se enviara el mensaje
	double m_TiempoEnvio;

	//Un puntero a una informacion extra particular de cada tipo de mensaje
	void* m_pInformacionExtra;

	Mensaje(int TipoMensaje, unsigned int IDDestinatario, unsigned int IDRemitente, 
		double TiempoEnvio, void* pInformacionExtra): 
													m_TipoMensaje(TipoMensaje),
													m_IDDestinatario(IDDestinatario),
													m_IDRemitente(IDRemitente),
													m_TiempoEnvio(TiempoEnvio),
													m_pInformacionExtra(pInformacionExtra)
	{}

	bool operator<(const Mensaje& B) const
	{
		return (m_TiempoEnvio < B.m_TiempoEnvio);
	}
};