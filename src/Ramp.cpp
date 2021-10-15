#include "Ramp.h"
#include "TextureManager.h"
#include "Util.h"

Ramp::Ramp(): m_width(4), m_height(3)
{
    TextureManager::Instance().load("../Assets/textures/Ramp.png", "ramp");
    
    setWidth(m_width * PIXELS_PER_METER);
    setHeight(m_height * PIXELS_PER_METER);
    getTransform()->position = glm::vec2(400, 500 - (m_height * 20));
}

Ramp::~Ramp() = default;

void Ramp::draw()
{
    TextureManager::Instance().drawWithCustomSize("ramp", getTransform()->position.x, getTransform()->position.y, 0, 255, getWidth(), getHeight(), true);
}

void Ramp::update()
{
}

void Ramp::clean()
{
}

float Ramp::GetRampWidth()
{
    return m_width;
}

void Ramp::SetRampWidth(float w)
{
    m_width = w;
    setWidth(w * PIXELS_PER_METER);
}

float Ramp::GetRampHeight()
{
    return m_height;
}

void Ramp::SetRampHeight(float h)
{
    m_height = h;
    getTransform()->position.y = (500 - (h * 20));
    setHeight(h * PIXELS_PER_METER);
}

float Ramp::GetRampAngle()
{
    return atan(m_height/m_width) * Util::Rad2Deg;
}

glm::vec2 Ramp::GetRampStart()
{
    return getTransform()->position - glm::vec2(getWidth() / 2, getHeight() / 2);
}

glm::vec2 Ramp::GetRampEnd()
{
    return getTransform()->position + glm::vec2(getWidth() / 2, getHeight() / 2);
}
