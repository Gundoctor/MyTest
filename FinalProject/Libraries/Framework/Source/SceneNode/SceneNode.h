#ifndef __SCENENODE_H__
#define __SCENENODE_H__

class ShaderProgram;

class SceneNode
{
public:
	SceneNode(Vector2 pos, Vector2 scale);
	SceneNode(SceneNode* pParent, Vector2 pos, Vector2 scale);
	virtual ~SceneNode();

	void AttachChild(SceneNode* pChild);
	SceneNode* DetachChild(SceneNode* child);

	void Draw(Vector2 camPos, Vector2 camProj, ShaderProgram* pShader = 0);
	void Update(float deltatime);

	Vector2			GetWorldPosition() const;

	// Getters
	Vector2            GetPosition() const;
	virtual Vector2    GetScale() const;

	// Setters
	virtual void    SetPosition(Vector2 pos);
	virtual void    SetScale(Vector2 scale);

protected:
	// Protected member variables
	Vector2             m_scale;
	Vector2             m_position;

	virtual void DrawCurrent(Vector2 worldPos, Vector2 camPos, Vector2 camProj, ShaderProgram* pShader);
	virtual void UpdateCurrent(float deltatime);

private:
	// Private member variables
	SceneNode *					m_pParent;
	std::vector<SceneNode*>		m_pChildren;

	// Private methods
	void DrawChildren(Vector2 camPos, Vector2 camProj, ShaderProgram* pShader);
	void UpdateChildren(float deltatime);

};

#endif
