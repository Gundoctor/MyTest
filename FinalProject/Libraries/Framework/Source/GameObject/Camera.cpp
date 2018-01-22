#include "FrameworkPCH.h"

Camera::Camera(unsigned int id, vec2 pos, vec2 proj, vec2 vpCoord, vec2 vpSize) :
    GameObject(id, pos, 0),
    m_projection(proj),
    m_viewportCoord(vpCoord),
    m_viewportSize(vpSize),
    m_flags(0),
    m_drawMode(GL_TRIANGLES),
    m_pFollowTarget(nullptr),
    m_pBackgroundMesh(nullptr)
{
}

Camera::~Camera()
{
}

void Camera::DrawCurrent(Vector2 worldPos, vec2 camPos, vec2 projScale, ShaderProgram *pShader)
{
    // DO NOTHING
}

void Camera::DrawOverride(vec2 camPos, vec2 projScale, ShaderProgram *pShader, GLenum mode)
{
    if (m_pMesh == nullptr)
        return;

	// Set color
	m_pMesh->SetColor(m_color);

    if (pShader != nullptr)
        m_pMesh->Draw(m_projection, m_position, camPos, projScale, pShader, mode);
    else
        m_pMesh->Draw(m_projection, m_position, camPos, projScale, m_pShader, m_drawMode);
}

void Camera::DrawBackground()
{
	if (m_pBackgroundMesh != nullptr)
	{
        m_pBackgroundMesh->SetColor(m_backgroundColor);
		m_pBackgroundMesh->Draw(m_viewportSize, m_position, m_position, m_viewportSize, m_pShader);
	}
}

void Camera::PreDrawSetup()
{
    glViewport((GLint)m_viewportCoord.x, (GLint)m_viewportCoord.y,
        (GLsizei)m_viewportSize.x, (GLsizei)m_viewportSize.y);
}

void Camera::SetupCamera(ShaderProgram* pShader, GLenum drawMode, unsigned int flags)
{
    m_pShader = pShader;
    m_drawMode = drawMode;
    m_flags = flags;
}

vec2 Camera::GetProjection()
{
    return m_projection;
}

vec2 Camera::GetVPCoord()
{
	return m_viewportCoord;
}

vec2 Camera::GetVPSize()
{
	return m_viewportSize;
}

GLenum Camera::GetDrawMode() const
{
    return m_drawMode;
}

bool Camera::CamNeedsMoreShaderSetup()
{
    return m_flags & CAMERA_FLAG_SHADER_SETUP;
}

bool Camera::IsOverrider()
{
	return m_flags & CAMERA_FLAG_OVERRIDER;
}

bool Camera::CamCanResize()
{
    return m_flags & CAMERA_FLAG_RESIZE;
}

ColorStruct Camera::GetBackgroundColor()
{
    return m_backgroundColor;
}

void Camera::SetProjection(vec2 proj)
{
    m_projection = proj;
}

void Camera::SetVPCoord(vec2 vpCoord)
{
    m_viewportCoord = vpCoord;
}

void Camera::SetVPSize(vec2 vpSize)
{
    m_viewportSize = vpSize;
}

void Camera::SetCamNeedsMoreShaderSetup(bool setup)
{
    if (setup)
        m_flags |= CAMERA_FLAG_SHADER_SETUP;
    else
        m_flags &= ~CAMERA_FLAG_SHADER_SETUP;
}

void Camera::SetCamResizeable(bool canResize)
{
    if (canResize)
        m_flags |= CAMERA_FLAG_RESIZE;
    else
        m_flags &= ~CAMERA_FLAG_RESIZE;
}

void Camera::SetDrawMode(GLenum drawMode)
{
    m_drawMode = drawMode;
}

void Camera::SetFollowTarget(GameObject *target)
{
    m_pFollowTarget = target;
}

void Camera::SetBackgroundMesh(Mesh* mesh)
{
    m_pBackgroundMesh = mesh;

    if (m_pBackgroundMesh != nullptr)
	    m_pBackgroundMesh->SetColor(ColorStruct(100, 100, 100, 255));
}

void Camera::SetBackgroundColor(ColorStruct col)
{
    m_backgroundColor = col;
}