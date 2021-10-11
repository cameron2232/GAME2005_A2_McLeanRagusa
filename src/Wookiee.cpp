#include "Wookiee.h"
#include "TextureManager.h"

Wookiee::Wookiee()
{
	TextureManager::Instance().load("../Assets/textures/Wookiee.png", "wookiee");
	auto size = TextureManager::Instance().getTextureSize("wookiee");

	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(0.0f, 0.0f);
}

Wookiee::~Wookiee()
{
}

void Wookiee::draw()
{
	TextureManager::Instance().draw("wookiee", getTransform()->position.x, getTransform()->position.y, 0, 255, true);
}

void Wookiee::update()
{
}

void Wookiee::clean()
{
}
