#pragma once
#ifndef __LOOTBOX_H__
#define __LOOTBOX_H__

#include "DisplayObject.h"

class LootBox : public DisplayObject {
public:
	LootBox();
	~LootBox();

	void draw() override;
	void update() override;
	void clean() override;

	void UpdatePos(glm::vec2 pos);

	glm::vec2 GetInitialPos();
	void SetInitialPos(glm::vec2 pos);
	float GetAngle();
	void SetAngle(float angle);
	float GetMass();
	void SetMass(float mass);
	bool CanMove();
	void SetCanMove(bool flag);
	glm::vec2 GetInitialVelocity();
	void SetInitialVelocity(glm::vec2 vel);
	void ResetTime();
	glm::vec2 GetDistance();


	float GetXOffset();
	void SetXOffset(float x);
	float GetTotalDisplacement();
private:
	glm::vec2 m_initialPos;
	glm::vec2 m_prevPos;
	glm::vec2 m_initalVelocity;
	float m_angle;
	float m_mass;
	bool m_canMove;
	float m_totalMovementTime;
	float m_xOffset;
	int startingX;
	float totalX;
	float totalY;
	glm::vec2 currentVelocity;

	float calculateDisplacement(float initialStart, float initialVelocity, float acceleration);
	float calculateCurrentVelocity(float initialVelocity, float acceleration);
};

#endif // !__LOOTBOX_H__
