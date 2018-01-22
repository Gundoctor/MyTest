#ifndef __GameCore_H__
#define __GameCore_H__

class Framework;
class ShaderProgram;

class GameCore
{
public:
    GameCore(Framework* pFramework);
    virtual ~GameCore();

    virtual void OnSurfaceChanged(int width, int height) = 0;
    virtual void LoadContent() = 0;

    virtual void Update(float deltatime) = 0;
    virtual void Draw() = 0;

    virtual void AddEvent(Event* pEvent) { };
    virtual bool ProcessEvent(Event* pEvent) { return false; };

	void SetAspectRatio(int width, int height);
	Vector2Int GetAspectRatio();

    Point GetWindowSize() const;
	Point GetPreviousWindowSize() const;

	double GetTimeSinceStart();

	Vector2Int GetMouseCoordinates();

protected:
    Framework* m_pFramework;

    double m_gameStartTime;

	Vector2Int m_aspectRatio;

};

#endif //__GameCore_H__