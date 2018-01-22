#include "GamePCH.h"

#pragma comment( lib, "../Output/Framework/Debug/Framework.lib" )
#pragma comment( lib, "opengl32.lib" )

#define SCREEN_WIDTH 630
#define SCREEN_HEIGHT 490

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    srand((unsigned int)time(NULL));

    Framework* pFramework = new Framework();

    GameCore* pGame = new GameSample( pFramework );
	pGame->SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);

    pFramework->Init( SCREEN_WIDTH, SCREEN_HEIGHT );
    pFramework->Run( pGame );
    pFramework->Shutdown();


    delete( pGame );
    delete( pFramework );
}
