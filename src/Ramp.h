#pragma once
#ifndef __RAMP_H__
#define __RAMP_H__

#include "DisplayObject.h"
#define PIXELS_PER_METER 40

class Ramp : public DisplayObject {
public:
	Ramp();
	~Ramp();

	void draw() override;
	void update() override;
	void clean() override;

	float GetRampWidth();
	void SetRampWidth(float w);
	float GetRampHeight();
	void SetRampHeight(float h);
	float GetRampAngle();

	glm::vec2 GetRampStart();
	glm::vec2 GetRampEnd();

	//used for scrolling
	float GetXOffset();
	void SetXOffset(float x);

private:
	float m_width;
	float m_height;
	float m_Xoffset;
};

#endif // !__RAMP_H__
