#include "Matematica.h"

#include <limits>

#include <Box2D/Box2D.h>

sf::Vector2f Normalizar(const sf::Vector2f& a)
{
	float norm = sqrt(a.x*a.x+a.y*a.y);
	if( norm == 0.0f )
		return a;//el vector 0
	sf::Vector2f res = a / norm;
	return res;
}

sf::Vector2f Truncar(const sf::Vector2f& a, float max)
{
	float mag = Norma(a);
	if( mag > max )
		return ( (a/mag)*max );
	else
		return a;
}

float Dot(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return (a.x*b.x+a.y*b.y);
}

float Norma(const sf::Vector2f& a)
{
	return sqrt(a.x*a.x+a.y*a.y);
}

float Cruz2D(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return (a.x*b.y-a.y*b.x);
}

float max(float a, float b)
{
	return (a<b)?b:a;
}

float min(float a, float b)
{
	return (a>b)?b:a;
}

float clamp(float a, float minimo, float maximo)
{
	return max(minimo,min(maximo,a));
}

float RandUnitario()
{
	return ( (rand()/(float)RAND_MAX)*2.0f-1.0f );
}

int Rand(int min, int max)
{
	return (int)( (rand()/(float)RAND_MAX)*(max-min)+min );
}

b2Transform MundoALocal(const b2Vec2 &localI, const b2Vec2 &localJ, const b2Vec2 &localO)
{
	b2Transform WtoL;

	b2Rot rot;
	rot.s = localJ.x;
	rot.c = localI.x;
	WtoL.p = b2Vec2(-b2Dot(localI,localO), -b2Dot(localJ,localO));

	return WtoL;
}