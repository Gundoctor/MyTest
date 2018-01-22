#include "GamePCH.h"


Healthbar::Healthbar(GameObject* gObj, ShaderProgram* pShader, vec2 pos, vec2 scale) :
    SceneNode(gObj, pos, scale),
    m_pGObj(gObj),
    m_pShader(pShader),
    m_pMesh(nullptr)
{
	Initialize();
}

Healthbar::~Healthbar()
{

}

void Healthbar::Initialize()
{
	SetScale(vec2(50.0f, 8.0f));

	m_lostHealthCol = { 60, 0, 0, 255 };
	m_currentHealthCol = { 255, 0, 0, 255 };
}

void Healthbar::SetGameObject(GameObject* gObj)
{
    m_pGObj = gObj;
}

void Healthbar::SetShader(ShaderProgram* pShader)
{
    m_pShader = pShader;
}

void Healthbar::SetMesh(Mesh* pMesh)
{
    m_pMesh = pMesh;
}

void Healthbar::DrawCurrent(Vector2 worldPos, vec2 camPos, vec2 camProj, ShaderProgram* pShader)
{
    assert(m_pMesh != nullptr);

	// don't draw if its game object is inactive
	if (!m_pGObj->IsActive())
		return;

    // custom shader
    glUseProgram(m_pShader->GetProgram());

    // Pass health value to shader
    {
		float health = (float)(m_pGObj->GetHealth()); 
		float maxHealth = (float)(m_pGObj->GetMaxHealth());

        GLuint uLoc = glGetUniformLocation(m_pShader->GetProgram(), "u_Health");
        if (uLoc != -1)
        { 
            glUniform1f(uLoc, health);
        }

		uLoc = glGetUniformLocation(m_pShader->GetProgram(), "u_MaxHealth");
		if (uLoc != -1)
		{
			glUniform1f(uLoc, maxHealth);
		}

        // Remaining health color
        uLoc = glGetUniformLocation(m_pShader->GetProgram(), "u_LostHealthCol");
        if (uLoc != -1)
        {
            float charSize = 255;
            glUniform4f(uLoc, (float)m_lostHealthCol.r / charSize, (float)m_lostHealthCol.g / charSize, (float)m_lostHealthCol.b / charSize, (float)m_lostHealthCol.a / charSize);
        }
    }

	m_pMesh->SetColor(m_currentHealthCol);

    // Draw
    m_pMesh->Draw(m_scale, worldPos, camPos, camProj, m_pShader);
}

void Healthbar::UpdateCurrent(float deltatime)
{
}