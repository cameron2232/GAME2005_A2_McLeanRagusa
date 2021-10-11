#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	updateLabels();
	drawDisplayList();

	if (!m_pParticle->isBeingThrown())
		Util::DrawLine(m_pParticle->getInitialPos(), glm::vec2(m_pParticle->getInitialPos().x + (cos(m_pParticle->getLaunchAngle() * Util::Deg2Rad) * m_pParticle->getInitialVelocity()),
				m_pParticle->getInitialPos().y - (sin(m_pParticle->getLaunchAngle() * Util::Deg2Rad) * m_pParticle->getInitialVelocity())));

	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	if (m_pParticle->isBeingThrown())
		scrollScene();

	if (CollisionManager::circleAABBCheck(m_pParticle, m_pGround[0]) || CollisionManager::circleAABBCheck(m_pParticle, m_pGround[1]))
	{
		if (m_pParticle->isBeingThrown())
			m_pParticle->setIsAnimated(true);
		m_pParticle->setIsBeingThrown(false);
		m_pStormTroopers->setEnabled(false);
	}
		

	updateDisplayList();	
}

void PlayScene::clean()
{
	removeAllChildren();

}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance().quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_SPACE))
	{
		m_pParticle->setIsBeingThrown(true);
		m_pStormTroopers->setEnabled(true);
		canEditValues = false;

		for (int i = 0; i < 3; i++)
			m_pLabels[i]->setEnabled(false);
		for (int i = 3; i < 6; i++)
			m_pLabels[i]->setEnabled(true);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance().changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance().changeSceneState(END_SCENE);
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Physics Simulation";

	canEditValues = true;

	m_pBackground = new Background();
	addChild(m_pBackground);

	m_pGround.push_back(new Ground());
	m_pGround.push_back(new Ground(1920, 636));
	for (int i = 0; i < 2; i++)
		addChild(m_pGround[i]);

	m_pParticle = new Particle();

	//Note: Original position is 400, 464. Add these values when calculating placement
	m_pStormTroopers = new StormTroopers();
	m_pStormTroopers->getTransform()->position = m_pParticle->getInitialPos();
	m_pStormTroopers->getTransform()->position.x += 485 * PIXELS_PER_METER;
	m_pStormTroopers->getTransform()->position.y = m_pParticle->getInitialPos().y - (m_pStormTroopers->getHeight() / 2) + (m_pParticle->getHeight() / 2);
	addChild(m_pStormTroopers);

	addChild(m_pParticle);

	m_pWookiee = new Wookiee();
	m_pWookiee->getTransform()->position = m_pParticle->getInitialPos();
	m_pWookiee->getTransform()->position.y -= (m_pWookiee->getHeight() / 2) - (m_pParticle->getHeight() / 2);
	addChild(m_pWookiee);

	SDL_Color black = { 0, 0, 0, 255 };
	m_pLabels.push_back(new Label("X Velocity: " + std::to_string(cos(m_pParticle->getLaunchAngle() * Util::Deg2Rad) * m_pParticle->getInitialVelocity()) + "m/s", "Consolas", 16, black, glm::vec2(130.0f, 10.0f)));
	m_pLabels.push_back(new Label("Y Velocity: " + std::to_string(sin(m_pParticle->getLaunchAngle() * Util::Deg2Rad) * m_pParticle->getInitialVelocity()) + "m/s", "Consolas", 16, black, glm::vec2(130.0f, 30.0f)));
	m_pLabels.push_back(new Label("Press [SPACE] to throw the Thermal Detonator or [`] for options", "Consolas", 20, black, glm::vec2(400.0f, 500.0f)));
	m_pLabels.push_back(new Label("X Distance: " + std::to_string(m_pParticle->getDeltaTotalX()) + "m", "Consolas", 16, black, glm::vec2(130.0f, 10.0f)));
	m_pLabels.back()->setEnabled(false);
	m_pLabels.push_back(new Label("Y Distance: " + std::to_string(m_pParticle->getDeltaTotalY() * -1) + "m", "Consolas", 16, black, glm::vec2(130.0f, 30.0f)));
	m_pLabels.back()->setEnabled(false);
	m_pLabels.push_back(new Label("Time Elapsed: " + std::to_string(m_pParticle->getTotalFlightTime()) + "s", "Consolas", 16, black, glm::vec2(130.0f, 50.0f)));
	m_pLabels.back()->setEnabled(false);

	for (int i = 0; i < m_pLabels.size(); i++)
		addChild(m_pLabels[i]);
	

	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::scrollScene()
{
	//get the particles change in X and Y
	float deltaX = m_pParticle->getDeltaX();
	float deltaY = m_pParticle->getDeltaY();

	//used to slowly move the background
	int backgroundOffsetX = m_pParticle->getDeltaTotalX() / 75;
	int backgroundOffsetY = m_pParticle->getDeltaTotalY() / 75;

	//updates the background position as the particle moves
	m_pBackground->getTransform()->position = glm::vec2(0 - backgroundOffsetX, (600 - m_pBackground->getHeight()) - backgroundOffsetY);

	//used to prevent the particle's focus from dropping below the ground
	if (m_pGround[0]->getTransform()->position.y - m_pParticle->getTransform()->position.y < 200) {
		deltaY = 0;
	}
	else
		m_pParticle->getTransform()->position.y -= deltaY;

	//keeps the particle in the center of the X axis
	m_pParticle->getTransform()->position.x -= deltaX;

	//updates the initial position as the particle moves (required so that scrolling scene does not mess up results)
	m_pParticle->setInitialPos(glm::vec2(m_pParticle->getInitialPos().x - deltaX, m_pParticle->getInitialPos().y - deltaY));

	//updates the ground scrolling along X
	for (int i = 0; i < m_pGround.size(); i++) {
		m_pGround[i]->getTransform()->position.x -= deltaX;
		m_pGround[i]->getTransform()->position.y -= deltaY;

		if (m_pGround[i]->getTransform()->position.x < -m_pGround[i]->getWidth() / 2) {
			for (int j = 0; j < m_pGround.size(); j++) {
				m_pGround[j]->getTransform()->position.x += m_pGround[j]->getWidth();
			}
		}
	}

	//updates Wookiees position
	m_pWookiee->getTransform()->position.x -= deltaX;
	m_pWookiee->getTransform()->position.y -= deltaY;

	//updates the Stormtroopers position
	m_pStormTroopers->getTransform()->position.x -= deltaX;
	m_pStormTroopers->getTransform()->position.y -= deltaY;
}

void PlayScene::reset()
{
	m_pParticle->setIsBeingThrown(false);
	m_pParticle->setInitialPos(glm::vec2(400.0f, 464.0f));
	m_pParticle->getTransform()->position = m_pParticle->getInitialPos();
	m_pParticle->clearThrownSettings();

	m_pWookiee->getTransform()->position = m_pParticle->getInitialPos();
	m_pWookiee->getTransform()->position.y -= (m_pWookiee->getHeight() / 2) - (m_pParticle->getHeight() / 2);

	m_pStormTroopers->setXDistance(m_pStormTroopers->getXDistance());
	m_pStormTroopers->getTransform()->position.y = m_pParticle->getInitialPos().y - (m_pStormTroopers->getHeight() / 2) + (m_pParticle->getHeight() / 2);
	m_pStormTroopers->setEnabled(true);

	m_pBackground->getTransform()->position = glm::vec2(0, 600 - m_pBackground->getHeight());
	m_pGround[0]->getTransform()->position = glm::vec2(m_pGround[0]->getWidth() / 2, 636 - m_pGround[0]->getHeight() / 2);
	m_pGround[1]->getTransform()->position = glm::vec2(1920 + m_pGround[1]->getWidth() / 2, 636 - m_pGround[1]->getHeight() / 2);

	for (int i = 0; i < 3; i++)
		m_pLabels[i]->setEnabled(true);
	for (int i = 3; i < 6; i++)
		m_pLabels[i]->setEnabled(false);

	canEditValues = true;
	m_pParticle->setIsPlaying(false);
	m_pParticle->setEnabled(true);
}

void PlayScene::updateLabels()
{
	m_pLabels[0]->setText("X Velocity: " + std::to_string(cos(m_pParticle->getLaunchAngle() * Util::Deg2Rad) * m_pParticle->getInitialVelocity()) + "m/s");
	m_pLabels[1]->setText("Y Velocity: " + std::to_string(sin(m_pParticle->getLaunchAngle() * Util::Deg2Rad) * m_pParticle->getInitialVelocity()) + "m/s");
	m_pLabels[3]->setText("X Distance: " + std::to_string(m_pParticle->getDeltaTotalX() / PIXELS_PER_METER) + "m");
	m_pLabels[4]->setText("Y Distance: " + std::to_string(m_pParticle->getDeltaTotalY() * -1 / PIXELS_PER_METER) + "m");
	m_pLabels[5]->setText("Time Elapsed: " + std::to_string(m_pParticle->getTotalFlightTime()) + "s");
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Your Window Title Goes Here", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	float initialVelocity = m_pParticle->getInitialVelocity();
	if (ImGui::SliderFloat("Initial Velocity", &initialVelocity, 20.0f, 200.0f)) {
		if (canEditValues)
			m_pParticle->setInitialVelocity(initialVelocity);
	}

	ImGui::Separator();

	float launchAngle = m_pParticle->getLaunchAngle();
	if (ImGui::SliderFloat("Launch Angle", &launchAngle, 1.0f, 90.0f)) {
		if (canEditValues)
			m_pParticle->setLaunchAngle(launchAngle);
	}

	ImGui::Separator();

	float gravity = m_pParticle->getGravity();
	if (ImGui::SliderFloat("Gravity", &gravity, -1.0f, -20.0f)) {
		if (canEditValues)
			m_pParticle->setGravity(gravity);
	}

	ImGui::Separator();

	int xPos = m_pStormTroopers->getXDistance();
	if (ImGui::SliderInt("Stormtroopers distance", &xPos, 1, 920)) {
		if (canEditValues)
			m_pStormTroopers->setXDistance(xPos);
	}

	ImGui::Separator();

	if (ImGui::Button("Set Values to Solution 1")) {
		reset();
		m_pParticle->setInitialVelocity(95);
		m_pParticle->setLaunchAngle(15.88963);
		m_pParticle->setGravity(-9.8);
		m_pStormTroopers->setXDistance(485);
	}

	if (ImGui::Button("Set Values to Solution 2")) {
		reset();
		m_pParticle->setInitialVelocity(95);
		m_pParticle->setLaunchAngle(45);
		m_pParticle->setGravity(-9.8);
		m_pStormTroopers->setXDistance(920);
	}

	if (ImGui::Button("Toggle Visibility of Stormtroopers"))
	{
		m_pStormTroopers->setEnabled(!m_pStormTroopers->isEnabled());
	}

	if (ImGui::Button("Reset")) {
		reset();
	}
	
	ImGui::End();
}
