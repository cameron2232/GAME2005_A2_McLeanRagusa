#pragma once
#ifndef __WOOKIEE_H__
#define __WOOKIEE_H__

#include "DisplayObject.h"

class Wookiee :public DisplayObject {
public:
	Wookiee();
	~Wookiee();

	void draw() override;
	void update() override;
	void clean() override;
};

#endif