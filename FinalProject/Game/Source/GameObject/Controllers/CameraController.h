#ifndef __CAMERACONTROLLER_H__
#define __CAMERACONTROLLER_H__

#define CAMERA_UP       0x0100
#define CAMERA_DOWN     0x0200
#define CAMERA_RIGHT    0x0400
#define CAMERA_LEFT     0x0800
#define CAMERA_ZOOM_IN  0x1000
#define CAMERA_ZOOM_OUT 0x2000


class CameraController
{
public:
    CameraController();
    ~CameraController();

    unsigned int GetCurrentFlags();
    void OnEvent(InputEvent *pEvent);

private:
    unsigned int m_currentFlags;
};


#endif
