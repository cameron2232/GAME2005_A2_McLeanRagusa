#pragma once
#ifndef __GROUND_H__
#define __GROUND_H__
#include "DisplayObject.h"

class Ground : public DisplayObject {
public:
	Ground();
	Ground(float x, float y);
	~Ground();

	void draw() override;
	void update() override;
	void clean() override;
};

#endif // !__GROUND_H__
