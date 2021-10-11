#pragma once
#ifndef __EXAMPLESPRITE__
#define __EXAMPLESPRITE__
#include "DisplayObject.h"

class ExampleSprite final : public DisplayObject
{
public:
	// constructors
	ExampleSprite();

	// destructor
	~ExampleSprite();

	// life cycle functions
	void draw() override;
	void update() override;
	void clean() override;
private:

};

#endif /* defined (__OBSTACLE__) */