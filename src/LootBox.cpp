#include "LootBox.h"
#include "TextureManager.h"
#include "Util.h"
#include "Game.h"

LootBox::LootBox() : m_angle(0.0f), m_mass(12.8f), m_canMove(false)
{
	TextureManager::Instance().load("../Assets/textures/Loot_Box.png", "box");
	auto size = TextureManager::Instance().getTextureSize("box");

	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(0.0f, 0.0f);
}

LootBox::~LootBox() = default;

void LootBox::draw()
{
	TextureManager::Instance().draw("box", getTransform()->position.x - m_xOffset, getTransform()->position.y, m_angle, 255, true);
}

void LootBox::update()
{
	if (m_canMove) {
		float delta = TheGame::Instance().getDeltaTime();
		m_totalMovementTime += delta;

		getTransform()->position.x = calculateDisplacement(m_initialPos.x, m_initalVelocity.x, getRigidBody()->acceleration.x);
		getTransform()->position.y = calculateDisplacement(m_initialPos.y, m_initalVelocity.y, getRigidBody()->acceleration.y);

		getRigidBody()->velocity.x = calculateCurrentVelocity(m_initalVelocity.x, getRigidBody()->acceleration.x);
		getRigidBody()->velocity.y = calculateCurrentVelocity(m_initalVelocity.y, getRigidBody()->acceleration.y);
		totalX = getTransform()->position.x - GetInitialPos().x;
		totalY = getTransform()->position.y - GetInitialPos().y;
	}

	
	
}

void LootBox::clean()
{
}

void LootBox::UpdatePos(glm::vec2 pos)
{
	getTransform()->position = pos;
	m_initialPos = pos;
	startingX = pos.x;
}

glm::vec2 LootBox::GetInitialPos()
{
	return m_initialPos;
}

void LootBox::SetInitialPos(glm::vec2 pos)
{
	m_initialPos = pos;
}

float LootBox::GetAngle()
{
	return m_angle;
}

void LootBox::SetAngle(float angle)
{
	m_angle = angle;
}

float LootBox::GetMass()
{
	return m_mass;
}

void LootBox::SetMass(float mass)
{
	m_mass = mass;
}

bool LootBox::CanMove()
{
	return m_canMove;
}

void LootBox::SetCanMove(bool flag)
{
	m_canMove = flag;
}

glm::vec2 LootBox::GetInitialVelocity()
{
	return m_initalVelocity;
}

void LootBox::SetInitialVelocity(glm::vec2 vel)
{
	m_initalVelocity = vel;
}

void LootBox::ResetTime()
{
	m_totalMovementTime = 0;
}
glm::vec2 LootBox::GetDistance()
{
	return glm::vec2(totalX, totalY);
}

float LootBox::GetTotalTime()
{
	return m_totalMovementTime;
}

float LootBox::GetXOffset()
{
	return m_xOffset;
}

void LootBox::SetXOffset(float x)
{
	m_xOffset = x;
}

float LootBox::GetTotalDisplacement()
{
	return getTransform()->position.x - startingX;
}

float LootBox::calculateDisplacement(float initialStart, float initialVelocity, float acceleration)
{
	float scaledVelocity = initialVelocity * PIXELS_PER_METER;
	float scaledAcceleration = acceleration * PIXELS_PER_METER;

	return (initialStart + scaledVelocity * m_totalMovementTime + 0.5 * scaledAcceleration * pow(m_totalMovementTime, 2));
}

float LootBox::calculateCurrentVelocity(float initialVelocity, float acceleration)
{
	return initialVelocity + acceleration * m_totalMovementTime;
}

