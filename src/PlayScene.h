#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Background.h"
#include "Ground.h"
#include "Ramp.h"
#include "LootBox.h"
#include "Button.h"
#include "Label.h"
#include <iomanip>

#define PIXELS_PER_METER 40

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	float calculateNetAcceleration();
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

	void scrollScene();
	void reset();
	void setSurfaceToRamp();
	void setSurfaceToGround();
	void updateLabels();
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	// UI Items
	Button* m_pBackButton;
	bool m_simulationActive;

	//other items
	Background* m_pBackground;
	Ramp* m_pRamp;
	LootBox* m_pLootBox;
	std::vector<Ground*> m_pGround;
	std::vector<Label*> m_pLabels;

	float m_gravity;
	float m_coefficientOfFriction;
	int m_groundPosition;
	bool m_boxOnGround;
	bool m_boxStopped;
	float totalTime;
};

#endif /* defined (__PLAY_SCENE__) */