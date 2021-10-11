#include "Ground.h"
#include "TextureManager.h"

Ground::Ground()
{
	TextureManager::Instance().load("../Assets/textures/Sand_Ground.png", "ground");
	auto size = TextureManager::Instance().getTextureSize("ground");

	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(size.x / 2, 636 - size.y / 2);
}

Ground::Ground(float x, float y) : Ground() {
	getTransform()->position = glm::vec2(x + getWidth() / 2, y - getHeight() / 2);
}

Ground::~Ground()
{
}

void Ground::draw()
{
	TextureManager::Instance().draw("ground", getTransform()->position.x, getTransform()->position.y, 0, 255, true);
}

void Ground::update()
{
}

void Ground::clean()
{
}
