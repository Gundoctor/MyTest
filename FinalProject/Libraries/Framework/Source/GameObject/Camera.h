#ifndef __CAMERA_H__
#define __CAMERA_H__

class ShaderProgram;
class Mesh;
class GameObject;


#define CAMERA_FLAG_OVERRIDER        0x0001
#define CAMERA_FLAG_SHADER_SETUP     0x0002
#define CAMERA_FLAG_RESIZE           0x0004

class Camera : public GameObject
{
public:
    Camera(unsigned int id, vec2 pos, vec2 proj, vec2 vpCoord = vec2(0.0f, 0.0f), vec2 vpSize = vec2(600.0f, 600.0f));
	virtual ~Camera();

    virtual void DrawOverride(vec2 camPos, vec2 projScale, ShaderProgram *pShader = 0, GLenum mode = GL_TRIANGLES) override;
    virtual void DrawBackground();

	virtual void PreDrawSetup();
    void SetupCamera(ShaderProgram* pShader, GLenum drawMode, unsigned int flags = 0);

	// GETTERS
    vec2 GetProjection();
	vec2 GetVPCoord();
	vec2 GetVPSize();

    GLenum GetDrawMode() const;
	bool CamNeedsMoreShaderSetup();
	bool IsOverrider();
    bool CamCanResize();

    ColorStruct GetBackgroundColor();

	// SETTERS
    void SetProjection(vec2);
	void SetVPCoord(vec2 vpCoord);
	void SetVPSize(vec2 vpSize);

    void SetCamNeedsMoreShaderSetup(bool setup);
    void SetCamResizeable(bool canResize);

    void SetDrawMode(GLenum drawMode);
    void SetFollowTarget(GameObject *target);
    void SetBackgroundMesh(Mesh* mesh);

    void SetBackgroundColor(ColorStruct col);

protected:
	// Protected member variables
    vec2            m_projection;
    vec2            m_viewportCoord;
    vec2            m_viewportSize;

    unsigned int    m_flags;
    GLenum          m_drawMode;

    GameObject*     m_pFollowTarget;
    Mesh*           m_pBackgroundMesh;
    ColorStruct     m_backgroundColor;

	// Protected methods
	virtual void UpdateCurrent(float deltatime) = 0;
	virtual void DrawCurrent(Vector2 worldPos, vec2 camPos, vec2 projScale, ShaderProgram* pShader = 0) override;

};

#endif