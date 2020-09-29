#include "SteeringBehaviors.h"

#include "Vehiculo.h"

#include "Matematica.h"

#include <limits>

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

SteeringBehaviors::SteeringBehaviors(Vehiculo* pVehiculoDueño):
									m_pVehiculoDueño(pVehiculoDueño),
									m_ComportamientosActivos(0),
									m_pPresa(NULL),
									m_pAcechador(NULL),
									m_DeambularRuido(3.0f),
									m_DeambularDistancia(60.0f),
									m_DeambularRadio(25.0f),
									m_DeambularObjetivo(sf::Vector2f(1.0f,0.0f)),
									m_EvadirObstaculosRectanguloMin(80),
									m_pObstaculos(NULL),
									m_pParedes(NULL),
									m_TipoCombinacion(SumaTruncadaPriorizada),
									m_PesoBuscar(1.0f),
									m_PesoEscapar(1.0f),
									m_PesoArribar(1.0f),
									m_PesoInterceptar(1.0f),
									m_PesoEvadir(1.0f),
									m_PesoDeambular(1.0f),
									m_PesoEvadirObstaculos(1.0f),
									m_PesoEvadirParedes(1.0f),
									m_PesoEsconderse(1.0f)
{
	//SOLO PARA DEBUG
	m_EvadirParedesPuntoColision.resize(3);
}


//-------------------------------------------------------------//
//-----------------		Metodos		---------------------------//
//-------------------------------------------------------------//

sf::Vector2f SteeringBehaviors::Buscar(const sf::Vector2f& PosObjetivo)
{
	sf::Vector2f VelocidadDeseada = Normalizar(PosObjetivo - m_pVehiculoDueño->GetPosicion())
													* m_pVehiculoDueño->GetVelocidadMax();
	return (VelocidadDeseada - m_pVehiculoDueño->GetVelocidadLineal());
}

sf::Vector2f SteeringBehaviors::Escapar(const sf::Vector2f& PosPeligro)
{
	//Solo escapa si esta en la distancia de panico del origen del peligro
	const double DistanciaDePanico = 100.0 * 100.0;
	if( Norma(m_pVehiculoDueño->GetPosicion()-PosPeligro) > DistanciaDePanico )
		return sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f VelocidadDeseada = Normalizar(m_pVehiculoDueño->GetPosicion() - PosPeligro)
													* m_pVehiculoDueño->GetVelocidadMax();
	return (VelocidadDeseada - m_pVehiculoDueño->GetVelocidadLineal());
}

sf::Vector2f SteeringBehaviors::Arribar(const sf::Vector2f& PosObjetivo, Desaceleracion Tipo)
{
	sf::Vector2f HaciaObjetivo = PosObjetivo - m_pVehiculoDueño->GetPosicion();

	float dist = Norma(HaciaObjetivo);
	if( dist > 0.0f )//si todavia no estamos sobre el objetivo
	{
		//es necesaria para utilizar la desaceleracion
		const float CoefDesaceleracion = 0.3f;
		//calculamos la velocidad para llegar al objetivo dada la desaceleracion que deseamos
		float velocidad = dist / ((float)Tipo * CoefDesaceleracion);
		//nos aseguramos que la velocidad no excede el limite del vehiculo
		velocidad = std::min(velocidad, m_pVehiculoDueño->GetVelocidadMax());

		sf::Vector2f VelocidadDeseada = (HaciaObjetivo/dist) * velocidad ;// (HaciaObjetivo/dist) es solo HaciaObjetivo normalizado

		return (VelocidadDeseada - m_pVehiculoDueño->GetVelocidadLineal());
	}
	return sf::Vector2f(0.0f, 0.0f);
}

sf::Vector2f SteeringBehaviors::Interceptar(const Vehiculo& Acechado)
{
	assert(&Acechado != NULL);

	sf::Vector2f haciaAcechado = Acechado.GetPosicion() - m_pVehiculoDueño->GetPosicion();
	float angDif = Dot(m_pVehiculoDueño->GetDireccion(), Acechado.GetDireccion());

	angDif = (180.0f/PI)*acos(angDif);
	//Si estan mirandose de frente no predecimos sino que solo avanzamos hacia donde esta ahora
	if( Dot(haciaAcechado, m_pVehiculoDueño->GetDireccion() ) > 0.0f &&
		angDif > (180-TOLERANCIA_MIRANDOSE_GRADOS) &&
		angDif < (180+TOLERANCIA_MIRANDOSE_GRADOS) )
		return Buscar(Acechado.GetPosicion());

	float dtPrediccion = Norma(haciaAcechado) / (m_pVehiculoDueño->GetVelocidadMax()+Norma(Acechado.GetVelocidadLineal()));

	sf::Vector2f posAcechadoPrediccion = Acechado.GetPosicion() + Acechado.GetVelocidadLineal()*dtPrediccion;
	//SOLO PARA DEBUG
	m_InterceptarPosicionPredicion = posAcechadoPrediccion;

	return Buscar(posAcechadoPrediccion);
}

sf::Vector2f SteeringBehaviors::Evadir(const Vehiculo& Acechador)
{
	assert(&Acechador != NULL);

	sf::Vector2f haciaAsechador = Acechador.GetPosicion() - m_pVehiculoDueño->GetPosicion();

	float dtPrediccion = Norma(haciaAsechador) / (m_pVehiculoDueño->GetVelocidadMax()+Norma(Acechador.GetVelocidadLineal()));

	sf::Vector2f posAsechadorPrediccion = Acechador.GetPosicion() + Acechador.GetVelocidadLineal()*dtPrediccion;
	//SOLO PARA DEBUG
	m_EvadirPosicionPredicion = posAsechadorPrediccion;

	return Escapar(posAsechadorPrediccion);
}

sf::Vector2f SteeringBehaviors::Deambular()
{
	//movemos el objetivo de deambular
	m_DeambularObjetivo += sf::Vector2f(RandUnitario()*m_DeambularRuido,
										RandUnitario()*m_DeambularRuido);

	//proyectamos el objetivo sobre el circulo al rededor del vehiculo
	m_DeambularObjetivo = m_DeambularObjetivo/Norma(m_DeambularObjetivo);
	m_DeambularObjetivo *= m_DeambularRadio;

	//movemos el circulo(y el punto) adelante de vehiculo
	sf::Vector2f m_DeambularLocal = sf::Vector2f(m_DeambularDistancia, 0.0f) + m_DeambularObjetivo;

	//transformamos el punto y el circulo al espacio del mundo para tener la fuerza de steering en el mundo
	b2Transform T = m_pVehiculoDueño->GetTransformacion();
	b2Vec2 ObjetivoMundo = b2Mul(T, b2Vec2(m_DeambularLocal.x,m_DeambularLocal.y));
	return sf::Vector2f(ObjetivoMundo.x,ObjetivoMundo.y)-m_pVehiculoDueño->GetPosicion();
}

sf::Vector2f SteeringBehaviors::EvadirObstaculos(std::vector<EntidadEscena*> &Obstaculos)
{
	assert(&Obstaculos != NULL);

	float largoRectanguloColision = m_EvadirObstaculosRectanguloMin +
			(Norma(m_pVehiculoDueño->GetVelocidadLineal())/m_pVehiculoDueño->GetVelocidadMax()) * m_EvadirObstaculosRectanguloMin;

	//Estas variables contendran el resultado de la colision mas cercana, si es que existe colision claro
	//de no existir colision mantendran los mismos valores
	EntidadEscena* pEntidadColisionCercana = NULL;
	float          PICercano               = std::numeric_limits<float>::max();
	b2Vec2         PosicionLocalCercana    = b2Vec2(std::numeric_limits<float>::max(),std::numeric_limits<float>::max());

	for(std::vector<EntidadEscena*>::iterator it=Obstaculos.begin(); it != Obstaculos.end(); ++it)
	{
		b2Transform WtoL = MundoALocal(m_pVehiculoDueño->GetLocalI(), m_pVehiculoDueño->GetLocalJ(), m_pVehiculoDueño->GetLocalO());
		b2Vec2 ObsPosLocal = b2Mul(WtoL, b2Vec2((*it)->GetPosicion().x, (*it)->GetPosicion().y));

		//Eliminar a los que estan hacia atras
		if( ObsPosLocal.x >= 0.0f )
		{

			//Eliminar a los que no colisionen el rectangulo de colision
			float radioExpandido = m_pVehiculoDueño->GetBoundingCircleRadio() + (*it)->GetBoundingCircleRadio();
			if( abs(ObsPosLocal.y) < radioExpandido )
			{
				float& cX = ObsPosLocal.x;
				float& cY = ObsPosLocal.y;

				float sqrtParte = sqrt(radioExpandido*radioExpandido-cY*cY);//notar que usamos el radio expandido
				//float PuntoI = cX - sqrtParte;
				float PuntoI = cX - sqrt( -4*cY*cY+4*radioExpandido*radioExpandido )/2;

				//esto es temporal ya que no hago ordenamiento espacial y solo proceso los
				//obstaculos cercanos
				if( PuntoI > largoRectanguloColision )
					continue;

				if( PuntoI <= 0.0f )
					PuntoI = cX + sqrt( -4*cY*cY+4*radioExpandido*radioExpandido )/2;
					//PuntoI = cX + sqrtParte;

				if( PuntoI < PICercano )
				{
					//SOLO PARA DEBUG
					m_EvadirObstaculosRadioExpandido = radioExpandido;

					PICercano = PuntoI;
					pEntidadColisionCercana = *it;
					PosicionLocalCercana = ObsPosLocal;
				}
			}
		}
	}

	b2Vec2 fuerza(0.0f, 0.0f);
	//Si hay alguna colision
	if( pEntidadColisionCercana != NULL )
	{
		float multiplicador = 1.0f + (largoRectanguloColision - PosicionLocalCercana.x)/largoRectanguloColision;
		fuerza.y = ( pEntidadColisionCercana->GetBoundingCircleRadio() - PosicionLocalCercana.y ) * multiplicador;
		float pesoFreno = 0.2f;
		fuerza.x = ( pEntidadColisionCercana->GetBoundingCircleRadio() - PosicionLocalCercana.x ) * pesoFreno;
	}

	//SOLO PARA DEBUG
	if( pEntidadColisionCercana != NULL )
		if( PosicionLocalCercana.y > 0.0f )
			m_EvadirObstaculosPuntoCercaColision = b2Vec2(PICercano, 0.0f);//m_pVehiculoDueño->GetBoundingCircleRadio());
		else
			m_EvadirObstaculosPuntoCercaColision = b2Vec2(PICercano, 0.0f);//-m_pVehiculoDueño->GetBoundingCircleRadio());
	else
		m_EvadirObstaculosPuntoCercaColision = b2Vec2(99999.0f, 99999.0f);
	//SOLO PARA DEBUG
	if( pEntidadColisionCercana != NULL )
		m_EvadirObstaculosPuntoObjetoColision = PosicionLocalCercana;
	else
		m_EvadirObstaculosPuntoObjetoColision = b2Vec2(99999.0f, 99999.0f);

	if( pEntidadColisionCercana != NULL )
		fuerza = b2Mul(m_pVehiculoDueño->GetTransformacion().q, fuerza);//devuelve la fuerza calculada
	return sf::Vector2f(fuerza.x, fuerza.y);//devuelve vector 0
}

sf::Vector2f SteeringBehaviors::EvadirParedes(std::vector<Pared*> &Paredes)
{
	assert(&Paredes != NULL);

	sf::Vector2f fuerza(0.0f, 0.0f);

	sf::Vector2f PuntoColision;
	Pared* pParedColision;
	float tColision;

	//SOLO PARA DEBUG
	m_EvadirParedesPuntoColision[0] = sf::Vector2f(9999.0f, 9999.0f);
	m_EvadirParedesPuntoColision[1] = sf::Vector2f(9999.0f, 9999.0f);
	m_EvadirParedesPuntoColision[2] = sf::Vector2f(9999.0f, 9999.0f);

	//Por cada sensor
	for(int f=0; f < m_Sensores.size() ; ++f)
	{
		float tColisionCercano = std::numeric_limits<float>::max();
		//por cada pared buscamos la colision, que se produzca, mas cercana con el sensor
		for(std::vector<Pared*>::iterator itWall=Paredes.begin(); itWall != Paredes.end() ; ++itWall)
		{
			//Probamos si este sensor detecta una colision con esta pared... a una distancia menor a 200
			if( m_Sensores[f].TestColision((*itWall)->m_Normal, (*itWall)->m_Punto, tColision) && tColision < 200 )
			{
				if( tColision < tColisionCercano )
				{
					tColisionCercano = tColision;
					PuntoColision = m_Sensores[f].Evaluar(tColision);
					pParedColision = *itWall;
				}
			}
		}

		//Si hubo colision sumamos la fuerza de separacion
		if( tColisionCercano < std::numeric_limits<float>::max() )
		{
			float penetracion = m_Sensores[f].DistPuntoPlano(pParedColision->m_Normal, pParedColision->m_Punto, m_Sensores[f].Evaluar(200));
			fuerza += pParedColision->m_Normal * (-penetracion);//El - es porque penetracion da la distancia hasta el plano en la direcion de la normal

			//SOLO PARA DEBUG
			m_EvadirParedesPuntoColision[f] = PuntoColision;
		}
	}

	return fuerza;
}

sf::Vector2f SteeringBehaviors::EsconderseGetPosicionEscondite(const sf::Vector2f &posObstaculo, const float radioObstaculo, const sf::Vector2f &posPeligro)
{
	float distanciaEscondite = radioObstaculo + m_pVehiculoDueño->GetBoundingCircleRadio();
	sf::Vector2f AObstaculo = Normalizar(posObstaculo - posPeligro);
	return ( posObstaculo+AObstaculo*distanciaEscondite );
}

sf::Vector2f SteeringBehaviors::Esconderse(const sf::Vector2f& PosPeligro, std::vector<EntidadEscena*> &Obstaculos)
{
	float distMenor = std::numeric_limits<float>::max();
	sf::Vector2f PosEsconditeMenor;

	//SOLO PARA DEBUG
	m_EscondersePuntoEscondito.clear();

	//Recorremos los obstaculos y buscamos la posicion de escondite mas cercana
	for(int o=0; o < Obstaculos.size() ; ++o)
	{
		sf::Vector2f PosEscondite =
			EsconderseGetPosicionEscondite(Obstaculos[o]->GetPosicion(), Obstaculos[o]->GetBoundingCircleRadio(), PosPeligro);

		//SOLO PARA DEBUG
		m_EscondersePuntoEscondito.push_back(PosEscondite);

		float dist = Norma(PosEscondite-m_pVehiculoDueño->GetPosicion());
		if( dist < distMenor )
		{
			distMenor         = dist;
			PosEsconditeMenor = PosEscondite;
		}
	}

	//si no hay ninguna posicion de escondite escapamos del peligro
	if( distMenor == std::numeric_limits<float>::max() )
		return Escapar(PosPeligro);

	//nos dirigimos rapidamente al escondite
	return Arribar(PosEsconditeMenor, Rapido);
}

sf::Vector2f SteeringBehaviors::Calcular()
{
	switch( m_TipoCombinacion )
	{
		case SumaTruncada:
			return CalcularSumaTruncada();
			break;
		case SumaTruncadaPriorizada:
			return CalcularSumaTruncadaPriorizada();
			break;
	}
	return sf::Vector2f(0.0f, 0.0f);
}

sf::Vector2f SteeringBehaviors::CalcularSumaTruncada()
{
	//reseteamos la fuerza
	m_FuerzaComportamiento = sf::Vector2f(0.0f, 0.0f);

	//Sumamos las fuerzas de cada comportamiento activo
	if( isBuscarOn() )
		m_FuerzaComportamiento += Buscar(m_PosObjetivo);

	if( isEscaparOn() )
		m_FuerzaComportamiento += Escapar(m_PosObjetivo);

	if( isArribarOn() )
		m_FuerzaComportamiento += Arribar(m_PosObjetivo, Desaceleracion::Lento);

	if( isInterceptarOn() )
		m_FuerzaComportamiento += Interceptar(*m_pPresa);

	if( isDeambularOn() )
		m_FuerzaComportamiento += Deambular();

	if( isEvadirObstaculosOn() )
		m_FuerzaComportamiento += EvadirObstaculos(*m_pObstaculos);

	if( isEvadirParedesOn() )
		m_FuerzaComportamiento += EvadirParedes(*m_pParedes);

	if( isEsconderseOn() )
		m_FuerzaComportamiento += Esconderse(m_PosObjetivo, *m_pObstaculos);

	return Truncar(m_FuerzaComportamiento, m_pVehiculoDueño->GetFuerzaSteeringMax());
}

sf::Vector2f SteeringBehaviors::CalcularSumaTruncadaPriorizada()
{
	//reseteamos la fuerza
	m_FuerzaComportamiento = sf::Vector2f(0.0f, 0.0f);

	//Sumamos las fuerzas de cada comportamiento activo
	if( isEvadirParedesOn() )
		if( !AgregarFuerza( EvadirParedes(*m_pParedes)*m_PesoEvadirParedes ) )
			return m_FuerzaComportamiento;

	if( isEvadirObstaculosOn() )
		if( !AgregarFuerza( EvadirObstaculos(*m_pObstaculos)*m_PesoEvadirObstaculos ) )
			return m_FuerzaComportamiento;

	if( isDeambularOn() )
		if( !AgregarFuerza( Deambular()*m_PesoDeambular ) )
			return m_FuerzaComportamiento;

	if( isBuscarOn() )
		if( !AgregarFuerza( Buscar(m_PosObjetivo)*m_PesoBuscar ) )
			return m_FuerzaComportamiento;

	if( isEscaparOn() )
		if( !AgregarFuerza( Escapar(m_PosObjetivo)*m_PesoEscapar ) )
			return m_FuerzaComportamiento;

	if( isArribarOn() )
		if( !AgregarFuerza( Arribar(m_PosObjetivo, Desaceleracion::Lento)*m_PesoArribar ) )
			return m_FuerzaComportamiento;

	if( isInterceptarOn() )
		if( !AgregarFuerza( Interceptar(*m_pPresa)*m_PesoInterceptar ) )
			return m_FuerzaComportamiento;

	if( isEvadirOn() )
		if( !AgregarFuerza( Evadir(*m_pAcechador)*m_PesoEvadir ) )
			return m_FuerzaComportamiento;

	if( isEsconderseOn() )
		if( !AgregarFuerza( Esconderse(m_PosObjetivo, *m_pObstaculos)*m_PesoEsconderse ) )
			return m_FuerzaComportamiento;

	//no es necesario truncarlo, pero de todas formas por las dudas lo hacemos
	return Truncar(m_FuerzaComportamiento, m_pVehiculoDueño->GetFuerzaSteeringMax());
}

bool SteeringBehaviors::AgregarFuerza(const sf::Vector2f &fuerzaAgregar)
{
	float magFActual  = Norma(m_FuerzaComportamiento);//magnitud fuerza actual
	float magFAgregar = Norma(fuerzaAgregar);         //magnitud fuerza agregar

	if( magFActual >= m_pVehiculoDueño->GetFuerzaSteeringMax() )
		return false;

	if( (magFActual+magFAgregar) < m_pVehiculoDueño->GetFuerzaSteeringMax() )
		m_FuerzaComportamiento += fuerzaAgregar;
	else
	{
		float magDif = m_pVehiculoDueño->GetFuerzaSteeringMax() - magFActual;
		m_FuerzaComportamiento += (fuerzaAgregar/magFAgregar)*magDif;
	}
	return true;
}

void SteeringBehaviors::BuscarOn()
{
	m_ComportamientosActivos |= BuscarBit;
}

void SteeringBehaviors::BuscarOff()
{
	m_ComportamientosActivos ^= BuscarBit;
}

void SteeringBehaviors::EscaparOn()
{
	m_ComportamientosActivos |= EscaparBit;
}

void SteeringBehaviors::EscaparOff()
{
	m_ComportamientosActivos ^= EscaparBit;
}

void SteeringBehaviors::ArribarOn()
{
	m_ComportamientosActivos |= ArribarBit;
}

void SteeringBehaviors::ArribarOff()
{
	m_ComportamientosActivos ^= ArribarBit;
}

void SteeringBehaviors::InterceptarOn()
{
	m_ComportamientosActivos |= InterceptarBit;
}

void SteeringBehaviors::InterceptarOff()
{
	m_ComportamientosActivos ^= InterceptarBit;
}

void SteeringBehaviors::EvadirOn()
{
	m_ComportamientosActivos |= EvadirBit;
}

void SteeringBehaviors::EvadirOff()
{
	m_ComportamientosActivos ^= EvadirBit;
}

void SteeringBehaviors::DeambularOn()
{
	m_ComportamientosActivos |= DeambularBit;
}

void SteeringBehaviors::DeambularOff()
{
	m_ComportamientosActivos ^= DeambularBit;
}

void SteeringBehaviors::EvadirObstaculosOn()
{
	m_ComportamientosActivos |= EvadirObstaculosBit;
}

void SteeringBehaviors::EvadirObstaculosOff()
{
	m_ComportamientosActivos ^= EvadirObstaculosBit;
}

void SteeringBehaviors::EvadirParedesOn()
{
	m_ComportamientosActivos |= EvadirParedesBit;
}

void SteeringBehaviors::EvadirParedesOff()
{
	m_ComportamientosActivos ^= EvadirParedesBit;
}

void SteeringBehaviors::EsconderseOn()
{
	m_ComportamientosActivos |= EsconderseBit;
}

void SteeringBehaviors::EsconderseOff()
{
	m_ComportamientosActivos ^= EsconderseBit;
}

bool SteeringBehaviors::isBuscarOn() const
{
	return (m_ComportamientosActivos & BuscarBit);
}

bool SteeringBehaviors::isEscaparOn() const
{
	return (m_ComportamientosActivos & EscaparBit);
}

bool SteeringBehaviors::isArribarOn() const
{
	return (m_ComportamientosActivos & ArribarBit);
}

bool SteeringBehaviors::isInterceptarOn() const
{
	return (m_ComportamientosActivos & InterceptarBit);
}

bool SteeringBehaviors::isEvadirOn() const
{
	return (m_ComportamientosActivos & EvadirBit);
}

bool SteeringBehaviors::isDeambularOn() const
{
	return (m_ComportamientosActivos & DeambularBit);
}

bool SteeringBehaviors::isEvadirObstaculosOn() const
{
	return (m_ComportamientosActivos & EvadirObstaculosBit);
}

bool SteeringBehaviors::isEvadirParedesOn() const
{
	return (m_ComportamientosActivos & EvadirParedesBit);
}

bool SteeringBehaviors::isEsconderseOn() const
{
	return (m_ComportamientosActivos & EsconderseBit);
}

SteeringBehaviors::Sensores::Sensores(): m_pCaster(NULL)
{}

SteeringBehaviors::Sensores::Sensores(Vehiculo* pCaster, float angulo): m_pCaster(pCaster), m_Angulo(angulo)
{}

sf::Vector2f SteeringBehaviors::Sensores::GetOrigen() const
{
	return m_pCaster->GetPosicion();
}

sf::Vector2f SteeringBehaviors::Sensores::GetDireccion() const
{
	sf::Vector2f dir = m_pCaster->GetDireccion();
	b2Vec2 d(dir.x, dir.y);
	b2Transform T(b2Vec2(0.0f, 0.0f), b2Rot(m_Angulo));
	d = b2Mul(T, d);
	return sf::Vector2f(d.x, d.y);
}

sf::Vector2f SteeringBehaviors::Sensores::Evaluar(float t) const
{
	assert( t > 0.0f );
	return (GetOrigen()+GetDireccion()*t);
}

bool SteeringBehaviors::Sensores::TestColision(const sf::Vector2f &Normal, const sf::Vector2f &Punto, float &tColision) const
{
	float d = Dot(Punto, Normal);
	sf::Vector2f O = GetOrigen();
	float den = Dot(GetDireccion(),Normal);//den es de denominador... de la ecuacion
	if( den == 0.0f )
		return false;
	tColision = ( d - Dot(O,Normal) ) / den;
	if( tColision >= 0.0f )
		return true;
	else
		return false;
}

float SteeringBehaviors::Sensores::DistPuntoPlano(const sf::Vector2f &Normal, const sf::Vector2f &Punto, const sf::Vector2f &Q) const
{
	float d = Dot(Punto, Normal);
	return (Dot(Q,Normal)-d);
}

SteeringBehaviors::Pared::Pared(sf::Vector2f Normal, sf::Vector2f Punto): m_Normal(Normal), m_Punto(Punto)
{}