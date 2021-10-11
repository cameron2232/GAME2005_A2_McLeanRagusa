#include "Stormtroopers.h"
#include "TextureManager.h"

StormTroopers::StormTroopers()
{
	TextureManager::Instance().load("../Assets/textures/Stormtroopers.png", "stormtroopers");
	auto size = TextureManager::Instance().getTextureSize("stormtroopers");

	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(0.0f, 0.0f);
	setXDistance(485);
}

StormTroopers::~StormTroopers()
{
}

void StormTroopers::draw()
{
	TextureManager::Instance().draw("stormtroopers", getTransform()->position.x, getTransform()->position.y, 0, 255, true);
}

void StormTroopers::update()
{
}

void StormTroopers::clean()
{
}

int StormTroopers::getXDistance()
{
	return ((xDistance - 400) / 40);
}

void StormTroopers::setXDistance(int x)
{
	xDistance = (x * 40) + 400;
	getTransform()->position.x = xDistance;
}
