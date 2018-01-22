#include "FrameworkPCH.h"

GameObject::GameObject(unsigned int id, vec2 pos, vec2 scale) :
	SceneNode(pos, scale),
    m_objectID(id),
    m_pMesh(nullptr),
    m_pShader(nullptr),
	m_color(ColorStruct(255, 255, 255, 255)),
    m_maxHealth(HEALTH_DEFAULT),
    m_health(HEALTH_DEFAULT),
	m_isActive(false)
{
}

GameObject::GameObject(unsigned int id, SceneNode* pParent, vec2 pos, vec2 scale) :
	SceneNode(pParent, pos, scale),
	m_objectID(id),
	m_pMesh(nullptr),
	m_pShader(nullptr),
	m_color(ColorStruct(255, 255, 255, 255)),
    m_maxHealth(HEALTH_DEFAULT),
    m_health(HEALTH_DEFAULT),
	m_isActive(false)
{

}

GameObject::~GameObject()
{
}

void GameObject::DrawCurrent(Vector2 worldPos, vec2 camPos, vec2 projScale, ShaderProgram *pShader)
{
    assert(m_pMesh != nullptr);

	// Don't draw if self or parent is not active
	if (IsActive())
	{
		// Set Color
		m_pMesh->SetColor(m_color);

		// Draw
		if (m_pShader != nullptr)
		{
			m_pMesh->Draw(m_scale, worldPos, camPos, projScale, m_pShader);
		}
		else
		{
			m_pMesh->Draw(m_scale, worldPos, camPos, projScale, pShader);
		}
	}
}

void GameObject::DrawOverride(vec2 camPos, vec2 projScale, ShaderProgram *pShader, GLenum mode)
{
    assert(m_pMesh != nullptr);
    assert(pShader != nullptr);

	// Set Color
	m_pMesh->SetColor(m_color);
    
	// Draw
    m_pMesh->Draw(m_scale, GetWorldPosition(), camPos, projScale, pShader, mode);
}

void GameObject::SetSize(float size)
{
    m_scale.x = size;
    m_scale.y = size;
}

void GameObject::SetMesh(Mesh* pMesh)
{
	m_pMesh = pMesh;
}

void GameObject::SetShader(ShaderProgram* pShader)
{
    m_pShader = pShader;
}

void GameObject::SetColor(ColorStruct color)
{
	m_color = color;
}

void GameObject::SetHealth(int health)
{
    m_health = health;
}

unsigned int GameObject::GetObjectID() const
{
    return m_objectID;
}

float GameObject::GetSize() const
{
    float size = (m_scale.x + m_scale.y) / 2;
    return size;
}

ShaderProgram* GameObject::GetShader() const
{
    return m_pShader;
}

int GameObject::GetHealth() const
{
    return m_health;
}

int GameObject::GetMaxHealth() const
{
    return m_maxHealth;
}

bool GameObject::IsActive() const
{
	return m_isActive;
}

void GameObject::Activate()
{
	m_isActive = true;
}

void GameObject::Deactivate()
{
	m_isActive = false;
}