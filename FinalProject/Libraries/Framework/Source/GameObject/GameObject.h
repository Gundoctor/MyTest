#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#define HEALTH_DEFAULT 100

class Mesh;
class ShaderProgram;

class GameObject : public SceneNode
{
public:
	GameObject(unsigned int id, vec2 pos, vec2 scale);
	GameObject(unsigned int id, SceneNode* pParent, vec2 pos, vec2 scale);
	virtual ~GameObject() = 0;

	virtual void    DrawOverride(vec2 camPos, vec2 projScale, ShaderProgram *pShader = 0, GLenum mode = GL_TRIANGLES);

	// SETTERS
    virtual void    SetSize(float size);

	virtual void    SetMesh(Mesh* pMesh);
    void            SetShader(ShaderProgram* pShader);

	virtual void    SetColor(ColorStruct color);
    void            SetHealth(int health);


	// GETTERS
    unsigned int    GetObjectID() const;
	virtual float   GetSize() const;
	ShaderProgram*  GetShader() const;
    int             GetHealth() const;
    int             GetMaxHealth() const;


	bool IsActive() const;
	virtual void Activate();
	virtual void Deactivate();


protected:
	// Protected member variables
    Mesh*           m_pMesh;
    ShaderProgram*  m_pShader;
	int             m_health;
	int             m_maxHealth;

	ColorStruct		m_color;

	// Protected methods
	virtual void    UpdateCurrent(float deltatime) = 0;
	virtual void    DrawCurrent(Vector2 worldPos, vec2 camPos, vec2 projScale, ShaderProgram *pShader = 0) override;

private:
    unsigned int    m_objectID;

	bool m_isActive;

};


#endif