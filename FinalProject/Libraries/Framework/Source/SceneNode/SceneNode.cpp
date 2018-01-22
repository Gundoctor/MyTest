#include "FrameworkPCH.h"


SceneNode::SceneNode(Vector2 pos, Vector2 scale) :
	m_pParent(nullptr),
	m_scale(scale),
	m_position(pos)
{

}

SceneNode::SceneNode(SceneNode* pParent, Vector2 pos, Vector2 scale) :
	m_scale(scale),
	m_position(pos)
{
	pParent->AttachChild(this);
}

SceneNode::~SceneNode()
{
	// Only works if structure is acyclic
	for (SceneNode* child : m_pChildren)
	{
		delete child;
	}
	m_pChildren.clear();
}

void SceneNode::AttachChild(SceneNode* pChild)
{
	pChild->m_pParent = this;
	m_pChildren.push_back(pChild);
}

SceneNode* SceneNode::DetachChild(SceneNode* child)
{
	std::vector<SceneNode*>::iterator it = std::find(m_pChildren.begin(), m_pChildren.end(), child);

	// If we found the child to detach
	if (it != m_pChildren.end())
	{
		SceneNode* foundNode = *it;
		foundNode->m_pParent = nullptr;
		
		m_pChildren.erase(it);
		return foundNode;
	}

	return nullptr;
}

void SceneNode::Draw(Vector2 camPos, Vector2 camProj, ShaderProgram* pShader)
{
	Vector2 pos = GetWorldPosition();

	DrawCurrent(pos, camPos, camProj, pShader);
	DrawChildren(camPos, camProj, pShader);
}

void SceneNode::Update(float deltatime)
{
	UpdateCurrent(deltatime);
	UpdateChildren(deltatime);
}

Vector2 SceneNode::GetWorldPosition() const
{
	if (m_pParent == nullptr)
		return m_position;

	Vector2 parentPos = m_pParent->GetWorldPosition();
	return parentPos + m_position;
}

Vector2 SceneNode::GetPosition() const
{
	return m_position;
}

Vector2 SceneNode::GetScale() const
{
	return m_scale;
}

void SceneNode::SetPosition(Vector2 pos)
{
	m_position = pos;
}

void SceneNode::SetScale(Vector2 scale)
{
	m_scale = scale;
}

void SceneNode::DrawCurrent(Vector2 worldPos, Vector2 camPos, Vector2 camProj, ShaderProgram* pShader)
{
	// To be overridden by children classes
}

void SceneNode::DrawChildren(Vector2 camPos, Vector2 camProj, ShaderProgram* pShader)
{
	for (SceneNode* node : m_pChildren)
	{
		node->Draw(camPos, camProj, pShader);
	}
}

void SceneNode::UpdateCurrent(float deltatime)
{
	// To be overridden by children classes
}

void SceneNode::UpdateChildren(float deltatime)
{
	for (SceneNode* node : m_pChildren)
	{
		node->Update(deltatime);
	}
}
