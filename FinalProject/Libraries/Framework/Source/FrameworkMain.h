#ifndef __FrameworkMain_H__
#define __FrameworkMain_H__

class GameCore;

class Framework
{
protected:
    GameCore* m_pGameCore;

    bool m_EscapeButtonWillQuit;
    bool m_CloseProgramRequested;

    int m_InitialWidth;
    int m_InitialHeight;

	int m_OldWidth;
	int m_OldHeight;

    int m_CurrentWidth;
    int m_CurrentHeight;

    HWND m_hWnd;
    HGLRC m_hRenderingContext;
    HDC m_hDeviceContext;
    HINSTANCE m_hInstance;

    bool m_KeyStates[256];
    bool m_MouseButtonStates[3];
    bool m_WindowIsActive;
    bool m_FullscreenMode;

protected:
    void ResizeWindow(int width, int height);
    bool CreateGLWindow(char* title, int width, int height, char colorbits, char zbits, char stencilbits, bool fullscreenflag);
    void KillGLWindow();
    void GenerateXInputEvents();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
    Framework();

    void Init(int width, int height);
    int Run(GameCore* pGameCore);
    void Shutdown();

    void SetWindowSize(int width, int height);
    Point GetWindowSize();
	Point GetPreviousWindowSize();

    bool IsKeyDown(int value);
    bool IsMouseButtonDown(int id);
    void GetMouseCoordinates(int* mx, int* my);
};

#endif //__WinMain_H__
