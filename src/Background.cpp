#include "Background.h"
#include "TextureManager.h"

Background::Background()
{
	TextureManager::Instance().load("../Assets/textures/Warehouse.jpg", "warehouse");
	auto size = TextureManager::Instance().getTextureSize("warehouse");

	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(0, 600 - size.y);
}

Background::~Background()
{
}

void Background::draw()
{
	TextureManager::Instance().draw("warehouse", getTransform()->position.x - m_xOffset, getTransform()->position.y, 0, 255, false);
}

void Background::update()
{
}

void Background::clean()
{
}

float Background::GetXOffset()
{
	return m_xOffset;
}

void Background::SetXOffset(float x)
{
	m_xOffset = x;
}
