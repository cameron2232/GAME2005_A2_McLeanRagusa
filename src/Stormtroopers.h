#pragma once
#ifndef __STORMTROOPERS_H__
#define __STORMTROOPERS_H__

#include "DisplayObject.h"

class StormTroopers : public DisplayObject {
public:
	StormTroopers();
	~StormTroopers();

	void draw() override;
	void update() override;
	void clean() override;

	int getXDistance();
	void setXDistance(int x);

private:
	int xDistance;
};

#endif // !__STORMTROOPERS_H__
