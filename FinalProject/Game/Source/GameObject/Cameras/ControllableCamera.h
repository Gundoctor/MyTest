#ifndef __CONTROLLABLECAMERA_H__
#define __CONTROLLABLECAMERA_H__

class ControllableCamera : public Camera
{
public:
    ControllableCamera(vec2 pos, vec2 proj, vec2 vpCoord = vec2(0.0f, 0.0f), vec2 vpSize = vec2(600.0f, 600.0f));
    virtual ~ControllableCamera();

    void OnEvent(InputEvent *pEvent);

protected:
	void UpdateCurrent(float deltatime);

private:
    CameraController* m_controller;

};

#endif
