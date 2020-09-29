#include "EntidadEscena.h"

#include "Game.h"

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

EntidadEscena::EntidadEscena(const b2BodyDef& CuerpoDef, const b2FixtureDef& AdornoDef, float escala, sf::Image* pTextura)
{
	m_pCuerpo = Game::m_World.CreateBody(&CuerpoDef);
	m_pAdorno = m_pCuerpo->CreateFixture(&AdornoDef);

	m_posAux = m_pCuerpo->GetPosition();
	
	m_pGrafico = new sf::Sprite;
	dynamic_cast<sf::Sprite*>(m_pGrafico)->SetImage(*pTextura);
	dynamic_cast<sf::Sprite*>(m_pGrafico)->SetCenter(pTextura->GetWidth()/2.0f, pTextura->GetHeight()/2.0f);
	dynamic_cast<sf::Sprite*>(m_pGrafico)->SetScale(escala, escala);
	m_pGrafico->SetPosition( m_posAux.x, m_posAux.y);

	m_BoundingCircleRadio = m_pAdorno->GetAABB().GetExtents().Length();
}

EntidadEscena::EntidadEscena(const b2BodyDef& CuerpoDef, const b2FixtureDef& AdornoDef, sf::Shape* pForma)
{
	m_pCuerpo = Game::m_World.CreateBody(&CuerpoDef);
	m_pAdorno = m_pCuerpo->CreateFixture(&AdornoDef);

	m_posAux = m_pCuerpo->GetPosition();
	
	m_pGrafico = new sf::Shape(*pForma);
	m_pGrafico->SetPosition( m_posAux.x, m_posAux.y);

	m_BoundingCircleRadio = m_pAdorno->GetAABB().GetExtents().Length();
}

EntidadEscena::~EntidadEscena()
{
	m_pCuerpo->DestroyFixture(m_pAdorno);

	m_pAdorno = NULL;

	(Game::m_World).DestroyBody(m_pCuerpo);

	m_pCuerpo = NULL;
}

//-------------------------------------------------------------//
//-----------------		Metodos		---------------------------//
//-------------------------------------------------------------//

void EntidadEscena::SetSubRect(sf::IntRect& Rect)
{
	sf::Sprite* pSprite = dynamic_cast<sf::Sprite*>(m_pGrafico);
	if( pSprite )
	{
		pSprite->SetSubRect(Rect);
		//Centro para manejar el sprite desde el centro, al igual que el b2Body
		pSprite->SetCenter(Rect.GetWidth()/2.0f,Rect.GetHeight()/2.0f);
	}
}

void EntidadEscena::Actualizar(float dt)
{
	EntidadBase::Actualizar(dt);

	m_posAux = m_pCuerpo->GetPosition();
	
	m_pGrafico->SetPosition(m_posAux.x, m_posAux.y);

	m_angAux = m_pCuerpo->GetAngle();//entrega el angulo en radianes

	m_pGrafico->SetRotation(-m_angAux*180/3.14f);//recibe el angulo en grados
}

b2Transform EntidadEscena::GetTransformacion() const
{
	return m_pCuerpo->GetTransform();
}

void EntidadEscena::GetMarcoReferencia(b2Vec2 &LocalI, b2Vec2 &LocalJ, b2Vec2 &LocalO) const
{
	b2Transform T = m_pCuerpo->GetTransform();
	LocalI = T.R.col1;
	LocalJ = T.R.col2;
	LocalO = T.position;
}

b2Vec2 EntidadEscena::GetLocalI() const
{
	b2Transform T = m_pCuerpo->GetTransform();
	return T.R.col1;
}

b2Vec2 EntidadEscena::GetLocalJ() const
{
	b2Transform T = m_pCuerpo->GetTransform();
	return T.R.col2;
}

b2Vec2 EntidadEscena::GetLocalO() const
{
	b2Transform T = m_pCuerpo->GetTransform();
	return T.position;
}

float EntidadEscena::GetBoundingCircleRadio() const
{
	return m_BoundingCircleRadio;
}

sf::Vector2f EntidadEscena::GetPosicion() const
{
	b2Vec2 posAux = m_pCuerpo->GetPosition();
	return sf::Vector2f(posAux.x, posAux.y);
}

float EntidadEscena::GetAngulo() const
{
	return m_pCuerpo->GetAngle();
}

sf::Vector2f EntidadEscena::GetVelocidadLineal() const
{
	b2Vec2 posAux = m_pCuerpo->GetLinearVelocity();
	return sf::Vector2f(posAux.x, posAux.y);
}

float EntidadEscena::GetVelocidadAngular() const
{
	return m_pCuerpo->GetAngularVelocity();
}

b2Body* EntidadEscena::GetCuerpo()
{
	return m_pCuerpo;
}

void EntidadEscena::SetPosicion(float x, float y)
{
	m_pCuerpo->SetTransform(b2Vec2(x,y), m_pCuerpo->GetAngle());
}

void EntidadEscena::SetAngulo(float Angulo)
{
	m_pCuerpo->SetTransform(m_pCuerpo->GetPosition(), Angulo);
}

void EntidadEscena::Dibujar(sf::RenderWindow &RW)
{
	RW.Draw(*m_pGrafico);
}

void EntidadEscena::AplicarFuerzaMundo(const sf::Vector2f& fuerza, const sf::Vector2f& origen)
{
	m_pCuerpo->ApplyForce(b2Vec2(fuerza.x,fuerza.y), b2Vec2(origen.x,origen.y));
}

void EntidadEscena::AplicarFuerzaLocal(const sf::Vector2f& fuerza, const sf::Vector2f& origen)
{
	b2Transform T = m_pCuerpo->GetTransform();//transformacion de Local -> Mundo
	b2Vec2 aux = m_pCuerpo->GetLocalCenter()+b2Vec2(origen.x,origen.y);
	m_pCuerpo->ApplyForce(b2Vec2(fuerza.x,fuerza.y), b2Mul(T, aux)/*transformamos de Local al Mundo*/);
}