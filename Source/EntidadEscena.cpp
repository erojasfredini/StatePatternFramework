#include "EntidadEscena.h"

#include "Game.h"

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

EntidadEscena::EntidadEscena(b2BodyDef& CuerpoDef, const b2FixtureDef& AdornoDef, float escala, sf::Texture* pTextura)
{
	b2BodyUserData userData;
	userData.pointer = (uintptr_t)this;
	CuerpoDef.userData = userData;

	m_pCuerpo = Game::m_World.CreateBody(&CuerpoDef);
	m_pAdorno = m_pCuerpo->CreateFixture(&AdornoDef);

	m_posAux = m_pCuerpo->GetPosition();

	m_pGrafico = new sf::Sprite;
	sf::Sprite* s = dynamic_cast<sf::Sprite*>(m_pGrafico);
	m_pTransformable = s;
	s->setTexture(*pTextura);
	s->setOrigin(pTextura->getSize().x/2.0f, pTextura->getSize().y/2.0f);
	s->setScale(escala, escala);
	s->setPosition( m_posAux.x, m_posAux.y);

	m_BoundingCircleRadio = m_pAdorno->GetAABB(0).GetExtents().Length();
}

EntidadEscena::EntidadEscena(b2BodyDef& CuerpoDef, const b2FixtureDef& AdornoDef, sf::Shape* pForma)
{
	b2BodyUserData userData;
	userData.pointer = (uintptr_t)this;
	CuerpoDef.userData = userData;

	m_pCuerpo = Game::m_World.CreateBody(&CuerpoDef);
	m_pAdorno = m_pCuerpo->CreateFixture(&AdornoDef);

	m_posAux = m_pCuerpo->GetPosition();

	m_pGrafico = pForma;
	sf::Shape* s = dynamic_cast<sf::Shape*>(m_pGrafico);
	m_pTransformable = s;
	s->setPosition( m_posAux.x, m_posAux.y);

	m_BoundingCircleRadio = m_pAdorno->GetAABB(0).GetExtents().Length();
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
		pSprite->setTextureRect(Rect);
		//Centro para manejar el sprite desde el centro, al igual que el b2Body
		pSprite->setOrigin(Rect.width/2.0f,Rect.height/2.0f);
	}
}

void EntidadEscena::Actualizar(float dt)
{
	EntidadBase::Actualizar(dt);

	m_posAux = m_pCuerpo->GetPosition();

	m_pTransformable->setPosition(m_posAux.x, m_posAux.y);

	m_angAux = m_pCuerpo->GetAngle();//entrega el angulo en radianes

	m_pTransformable->setRotation(-m_angAux*180/3.14f);//recibe el angulo en grados
}

b2Transform EntidadEscena::GetTransformacion() const
{
	return m_pCuerpo->GetTransform();
}

void EntidadEscena::GetMarcoReferencia(b2Vec2 &LocalI, b2Vec2 &LocalJ, b2Vec2 &LocalO) const
{
	b2Transform T = m_pCuerpo->GetTransform();
	LocalI = T.q.GetXAxis();
	LocalJ = T.q.GetYAxis();
	LocalO = T.p;
}

b2Vec2 EntidadEscena::GetLocalI() const
{
	b2Transform T = m_pCuerpo->GetTransform();
	return T.q.GetXAxis();
}

b2Vec2 EntidadEscena::GetLocalJ() const
{
	b2Transform T = m_pCuerpo->GetTransform();
	return T.q.GetYAxis();
}

b2Vec2 EntidadEscena::GetLocalO() const
{
	b2Transform T = m_pCuerpo->GetTransform();
	return T.p;
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
	RW.draw(*m_pGrafico);
}

void EntidadEscena::AplicarFuerzaMundo(const sf::Vector2f& fuerza, const sf::Vector2f& origen)
{
	m_pCuerpo->ApplyForce(b2Vec2(fuerza.x,fuerza.y), b2Vec2(origen.x,origen.y), true);
}

void EntidadEscena::AplicarFuerzaLocal(const sf::Vector2f& fuerza, const sf::Vector2f& origen)
{
	b2Transform T = m_pCuerpo->GetTransform();//transformacion de Local -> Mundo
	b2Vec2 aux = m_pCuerpo->GetLocalCenter()+b2Vec2(origen.x,origen.y);
	m_pCuerpo->ApplyForce(b2Vec2(fuerza.x,fuerza.y), b2Mul(T, aux)/*transformamos de Local al Mundo*/, true);
}