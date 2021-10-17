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

	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{		
	if (m_simulationActive) {
		float totalAccel = calculateNetAcceleration();

		m_pLootBox->getRigidBody()->acceleration.x = totalAccel * cos(m_pLootBox->GetAngle() * Util::Deg2Rad);
		m_pLootBox->getRigidBody()->acceleration.y = totalAccel * sin(m_pLootBox->GetAngle() * Util::Deg2Rad);


		if (m_pLootBox->getTransform()->position.y >= m_groundPosition && !m_boxOnGround) {
			m_pLootBox->getTransform()->position.y = m_groundPosition;
			setSurfaceToGround();
		}

		if (m_pLootBox->getRigidBody()->velocity.x <= 0.0f && m_boxOnGround) {
			m_pLootBox->getRigidBody()->velocity.x = 0.0f;
			m_boxStopped = true;
			m_pLootBox->SetCanMove(false);
		}

		scrollScene();
	}

	updateLabels();
	updateDisplayList();
}

float PlayScene::calculateNetAcceleration()
{
	float forceOfGravity = m_pLootBox->GetMass() * m_gravity;
	float normalForce = -forceOfGravity * cos(m_pLootBox->GetAngle() * Util::Deg2Rad);
	float forceOfFriction = m_coefficientOfFriction * normalForce;

	float accelerationGrav = (forceOfGravity * sin(m_pLootBox->GetAngle() * Util::Deg2Rad)) / m_pLootBox->GetMass();
	float accelerationFric = (forceOfFriction / m_pLootBox->GetMass());
	return accelerationFric + accelerationGrav;
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
		reset();
		m_simulationActive = true;
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

	m_simulationActive = false;
	m_gravity = 9.8;
	m_groundPosition = 500;
	totalTime = 0;

	m_pBackground = new Background();
	addChild(m_pBackground);

	m_pGround.push_back(new Ground());
	m_pGround.push_back(new Ground(1920, 700));
	for (int i = 0; i < 2; i++)
		addChild(m_pGround[i]);

	m_pRamp = new Ramp();
	addChild(m_pRamp);

	m_pLootBox = new LootBox();
	m_pLootBox->UpdatePos(m_pRamp->GetRampStart());
	m_pLootBox->SetAngle(m_pRamp->GetRampAngle());
	m_pLootBox->SetInitialVelocity(glm::vec2(0.0f, 0.0f));
	addChild(m_pLootBox);

	SDL_Color color = { 0, 0, 0, 255 };
	m_pLabels.push_back(new Label("X Velocity: " + std::to_string(m_pLootBox->getRigidBody()->velocity.x) + "m/s" 
		+ " | Y Velocity: " + std::to_string(m_pLootBox->getRigidBody()->velocity.y) + "m/s", "Consolas", 14, color, glm::vec2(200.0f, 510.0f)));
	m_pLabels.push_back(new Label("X Acceleration: " + std::to_string(m_pLootBox->getRigidBody()->acceleration.x) + "m/s^2"
		+ " | Y Acceleration: " + std::to_string(m_pLootBox->getRigidBody()->acceleration.y) + "m/s^2", "Consolas", 14, color, glm::vec2(250.0f, 530.0f)));
	m_pLabels.push_back(new Label("Total Accel.: " + std::to_string(magnitude(m_pLootBox->getRigidBody()->acceleration.x, m_pLootBox->getRigidBody()->acceleration.y)) + "m/s^2"
		+ " | Total Vel.:" + std::to_string(magnitude(m_pLootBox->getRigidBody()->velocity.x, m_pLootBox->getRigidBody()->velocity.y)) + "m/s", "Consolas", 14, color, glm::vec2(220.0f, 550.0f)));
	m_pLabels.push_back(new Label("X Distance: " + std::to_string(m_pLootBox->GetDistance().x / PIXELS_PER_METER) + "m", "Consolas", 14, color, glm::vec2(100.0f, 570.0f)));
	m_pLabels.push_back(new Label("Time Elapsed: " + std::to_string(m_pLootBox->GetTotalTime()) + "s", "Consolas", 14, color, glm::vec2(100.0f, 590.0f)));

	for (int i = 0; i < m_pLabels.size(); i++)
		addChild(m_pLabels[i]);
	

	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::scrollScene()
{
	int offset = m_pLootBox->GetTotalDisplacement();

	m_pLootBox->SetXOffset(offset);
	for (int i = 0; i < m_pGround.size(); i++) {
		m_pGround[i]->SetXOffset( offset);
	}
	m_pRamp->SetXOffset(offset);
	m_pBackground->SetXOffset(offset / 2.5);
}

void PlayScene::reset()
{
	m_simulationActive = false;
	m_boxOnGround = false;
	m_boxStopped = false;
	m_coefficientOfFriction = 0.0f;
	m_pLootBox->SetCanMove(true);
	m_pLootBox->UpdatePos(m_pRamp->GetRampStart());
	m_pLootBox->SetAngle(m_pRamp->GetRampAngle());
	m_pLootBox->SetInitialVelocity(glm::vec2(0.0f, 0.0f));
	m_pLootBox->getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	m_pLootBox->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	m_pLootBox->ResetTime();

	m_pLootBox->SetXOffset(0);
	for (int i = 0; i < m_pGround.size(); i++) {
		m_pGround[i]->SetXOffset(0);
	}
	m_pRamp->SetXOffset(0);
	m_pBackground->SetXOffset(0);

	m_pLabels[0]->setText("X Velocity: 0.000000m/s |  Y Velocity: 0.000000m/s");

	m_pLabels[1]->setText("X Acceleration: 0.000000m/s^2  |  Y Acceleration: 0.000000m/s^2");

	m_pLabels[2]->setText("Total Accel: 0.000000m/s^2  |  Total Vel.: 0.00000m/s");

	m_pLabels[3]->setText("X Distance: 0.000000m");

	m_pLabels[4]->setText("Time Elapsed: 0.000000s");

}

void PlayScene::setSurfaceToGround()
{
	m_boxOnGround = true;
	m_coefficientOfFriction = m_pGround[0]->GetFrictionCoefficient();
	m_pLootBox->SetAngle(0.0f);
	m_pLootBox->SetInitialPos(m_pLootBox->getTransform()->position);
	m_pLootBox->SetInitialVelocity(glm::vec2(m_pLootBox->getRigidBody()->velocity.x, 0.0f));
	m_pLootBox->getRigidBody()->velocity.y = 0.0f;
	m_pLootBox->ResetTime();
}

void PlayScene::updateLabels()
{
	m_pLabels[0]->setText("X Velocity: " + std::to_string(m_pLootBox->getRigidBody()->velocity.x) + "m/s"
		+ " | Y Velocity: " + std::to_string(m_pLootBox->getRigidBody()->velocity.y) + "m/s");

	m_pLabels[1]->setText("X Acceleration: " + std::to_string(m_pLootBox->getRigidBody()->acceleration.x) + "m/s^2"
		+ " | Y Acceleration: " + std::to_string(m_pLootBox->getRigidBody()->acceleration.y) + "m/s^2");

	m_pLabels[2]->setText("Total Accel.: " + std::to_string(magnitude(m_pLootBox->getRigidBody()->acceleration.x, m_pLootBox->getRigidBody()->acceleration.y)) + "m/s^2"
		+ " | Total Vel.:" + std::to_string(magnitude(m_pLootBox->getRigidBody()->velocity.x, m_pLootBox->getRigidBody()->velocity.y)) + "m/s");
	if(m_boxOnGround)
	{
		m_pLabels[3]->setText("X Distance: " + std::to_string(m_pLootBox->GetDistance().x / PIXELS_PER_METER) + "m");
		m_pLabels[4]->setText("Time Elapsed: " + std::to_string(m_pLootBox->GetTotalTime()) + "s");
	}
}

float PlayScene::magnitude(float a, float b)
{
	return sqrt(pow(a, 2) + pow(b, 2));
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Your Window Title Goes Here", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	float rampWidth = m_pRamp->GetRampWidth();
	if (ImGui::SliderFloat("Ramp Width (m)", &rampWidth, 1.0f, 10.0f)) {
		if (!m_simulationActive) {
			m_pRamp->SetRampWidth(rampWidth);
			m_pLootBox->UpdatePos(m_pRamp->GetRampStart());
			m_pLootBox->SetAngle(m_pRamp->GetRampAngle());
		}
	}

	float rampHeight = m_pRamp->GetRampHeight();
	if (ImGui::SliderFloat("Ramp Height (m)", &rampHeight, 1.0f, 10.0f)) {
		if (!m_simulationActive) {
			m_pRamp->SetRampHeight(rampHeight);
			m_pLootBox->UpdatePos(m_pRamp->GetRampStart());
			m_pLootBox->SetAngle(m_pRamp->GetRampAngle());
		}
	}

	ImGui::Separator();

	float mass = m_pLootBox->GetMass();
	if (ImGui::SliderFloat("Box Mass (Kg)", &mass, 1.0f, 100.0f)) {
		if (!m_simulationActive)
			m_pLootBox->SetMass(mass);
	}

	ImGui::Separator();

	float gravity = m_gravity;
	if (ImGui::SliderFloat("Gravity (m/s^2)", &gravity, 1.0f, 50.0f)) {
		if (!m_simulationActive)
			m_gravity = gravity;
	}

	ImGui::Separator();

	float coefficient = m_pGround[0]->GetFrictionCoefficient();
	if (ImGui::SliderFloat("Friction Coefficient", &coefficient, 0.1f, 1.0f)) {
		if (!m_simulationActive) {
			m_pGround[0]->SetFrictionCoefficient(coefficient);
			m_pGround[1]->SetFrictionCoefficient(coefficient);
		}
	}

	ImGui::Separator();

	if (ImGui::Button("Reset")) {
		reset();
	}
	
	ImGui::End();
}
