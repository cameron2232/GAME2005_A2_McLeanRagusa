#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Particle.h"
#include "Background.h"
#include "Ground.h"
#include "Wookiee.h"
#include "Stormtroopers.h"
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
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

	void scrollScene();
	void reset();
	void updateLabels();
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	// UI Items
	Button* m_pBackButton;
	bool canEditValues;

	Particle* m_pParticle;
	Wookiee* m_pWookiee;
	StormTroopers* m_pStormTroopers;
	Background* m_pBackground;
	std::vector<Ground*> m_pGround;
	std::vector<Label*> m_pLabels;
};

#endif /* defined (__PLAY_SCENE__) */