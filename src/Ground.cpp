#include "Ground.h"
#include "TextureManager.h"

Ground::Ground() : m_coefficientOfFriction(0.42f)
{
	TextureManager::Instance().load("../Assets/textures/Metal_Ground.png", "ground");
	auto size = TextureManager::Instance().getTextureSize("ground");

	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(size.x / 2, 700 - size.y / 2);
}

Ground::Ground(float x, float y) : Ground() {
	getTransform()->position = glm::vec2(x + getWidth() / 2, 700 - getHeight() / 2);
}

Ground::~Ground()
{
}

void Ground::draw()
{
	TextureManager::Instance().draw("ground", getTransform()->position.x - m_xOffset, getTransform()->position.y, 0, 255, true);
}

void Ground::update()
{
}

void Ground::clean()
{
}

float Ground::GetFrictionCoefficient()
{
	return m_coefficientOfFriction;
}

void Ground::SetFrictionCoefficient(float co)
{
	m_coefficientOfFriction = co;
}

float Ground::GetXOffset()
{
	return m_xOffset;
}

void Ground::SetXOffset(float x)
{
	m_xOffset = x;
}
