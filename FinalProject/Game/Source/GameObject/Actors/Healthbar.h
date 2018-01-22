#ifndef __HEALTHBAR_H__
#define __HEALTHBAR_H__

class ShaderProgram;
class GameObject;
class Mesh;

class Healthbar : public SceneNode
{
public:
    Healthbar(GameObject* gObj, ShaderProgram* pShader, vec2 pos, vec2 scale);
    ~Healthbar();

    // Setters
    void SetGameObject(GameObject* gObj);
    void SetShader(ShaderProgram* pShader);
    void SetMesh(Mesh* pMesh);

protected:
    void DrawCurrent(Vector2 worldPos, vec2 camPos, vec2 camProj, ShaderProgram* pShader) override;
    void UpdateCurrent(float deltatime) override;

private:
    GameObject* m_pGObj;
    ShaderProgram* m_pShader;

	ColorStruct m_currentHealthCol;
    ColorStruct m_lostHealthCol;

    Mesh* m_pMesh;


	void Initialize();
    
};


#endif