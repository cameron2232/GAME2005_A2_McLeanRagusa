#include "ExampleSprite.h"

#include "TextureManager.h"

ExampleSprite::ExampleSprite()
{
	TextureManager::Instance().load("../Assets/textures/ship3.png", "ship");

	auto size = TextureManager::Instance().getTextureSize("ship");
	setWidth(size.x);
	setHeight(size.y);
}

ExampleSprite::~ExampleSprite()
= default;

void ExampleSprite::draw()
{
	TextureManager::Instance().draw("ship",
		getTransform()->position.x, getTransform()->position.y, 0, 255, true);
}

void ExampleSprite::update()
{
}

void ExampleSprite::clean()
{
}
