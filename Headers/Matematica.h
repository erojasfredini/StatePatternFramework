#pragma once

#include "SFML//Graphics.hpp"
#include "SFML//System.hpp"
#include "SFML//Window.hpp"
#include "SFML//Config.hpp"

#define PI 3.1415

sf::Vector2f Normalizar(const sf::Vector2f& a);

sf::Vector2f Truncar(const sf::Vector2f& a, float max);

float Dot(const sf::Vector2f& a, const sf::Vector2f& b);

float Norma(const sf::Vector2f& a);

float Cruz2D(const sf::Vector2f& a, const sf::Vector2f& b);

float max(float a, float b);

float min(float a, float b);

float clamp(float a, float minimo, float maximo);

float RandUnitario();

int Rand(int min, int max);

struct b2Transform;
struct b2Vec2;

b2Transform MundoALocal(const b2Vec2 &localI, const b2Vec2 &localJ, const b2Vec2 &localO);