#ifndef __STATICCAMERA_H__
#define __STATICCAMERA_H__


class StaticCamera : public Camera
{
public:
    StaticCamera(vec2 pos, vec2 proj, vec2 vpCoord = vec2(0.0f, 0.0f), vec2 vpSize = vec2(600.0f, 600.0f));
    virtual ~StaticCamera();

protected:
    void UpdateCurrent(float deltatime);

};

#endif