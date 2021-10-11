#include "Background.h"
#include "TextureManager.h"

Background::Background()
{
	TextureManager::Instance().load("../Assets/textures/Tatooine_Background.png", "background");
	auto size = TextureManager::Instance().getTextureSize("background");

	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(0, 600 - size.y);
}

Background::~Background()
{
}

void Background::draw()
{
	TextureManager::Instance().draw("background", getTransform()->position.x, getTransform()->position.y, 0, 255, false);
}

void Background::update()
{
}

void Background::clean()
{
}
