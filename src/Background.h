#pragma once
#ifndef __BACKGROUND__
#define __BACKGROUND__
#include "DisplayObject.h"

class Background : public DisplayObject
{
public:
	Background();
	~Background();

	void draw() override;
	void update() override;
	void clean() override;

	float GetXOffset();
	void SetXOffset(float x);

private:
	float m_xOffset;
};

#endif // !__BACKGROUND__
