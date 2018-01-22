#include "GamePCH.h"

GameSample::GameSample(Framework* pFramework)
    : GameCore(pFramework),
    m_pPlayer(nullptr)
{
    m_eventManager = new EventManager();
    m_resourceManager = new MyResourceManager();
    m_collisionSystem = new CollisionSystem();
    m_sceneManager = new SceneManager(this, (MyResourceManager*)m_resourceManager, m_collisionSystem);

	m_worldOrigin = WORLD_ORIGIN;
	m_worldSize = WORLD_SIZE;
}

GameSample::~GameSample()
{
    delete m_eventManager;
    delete m_resourceManager;
    delete m_sceneManager;
    delete m_collisionSystem;
}

void GameSample::OnSurfaceChanged(int width, int height)
{
    // Set OpenGL to draw to the entire window.
    glViewport(0, 0, width, height);

	// Modify cameras in currentScene
	m_sceneManager->UpdateCameraViewports(this);
}

void GameSample::LoadContent()
{
    // Turn on depth buffer testing.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Turn on alpha blending.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create our shaders.
	ShaderProgram* defaultShader = new ShaderProgram("Data/Shaders/color.vert", "Data/Shaders/color.frag");
	ShaderProgram* minimapShader = new ShaderProgram("Data/Shaders/greyscale.vert", "Data/Shaders/color.frag");

	// Add to map
    m_resourceManager->AddShader(ShaderName::Default, defaultShader);
	m_resourceManager->AddShader(ShaderName::Minimap, minimapShader);

	// Make a circle
    Mesh *circleMesh = new Mesh();
    circleMesh->GeneratePolygon(CICLEPOINTS);
    m_resourceManager->AddMesh(MeshType::Circle, circleMesh);


	// Generate Scenes
	// Dependent on Player and Camera being set up atm
    m_sceneManager->LoadContent();
    m_sceneManager->SetCurrentScene(SceneName::Tilemap);


    // Setup the values we will clear to, then actually clear the color and depth buffers.
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f); // (Red, Green, Blue, Alpha) - Dark blue
    glClearDepth(1); // 1 is maximum depth

    // Check for errors.
    CheckForGLErrors();
}

void GameSample::Update(float deltatime)
{
    if (m_pFramework->IsKeyDown('1'))
    {
        m_pFramework->SetWindowSize(630, 490);
		SetAspectRatio(630, 490);

		// Create game event to change tilemap to small
		Event* pEvent = new GameEvent(GameEventType::ChangeTilemap, 0);
		m_eventManager->PushEvent(pEvent);
    }
    if (m_pFramework->IsKeyDown('2'))
    {
        m_pFramework->SetWindowSize(700, 700);
		SetAspectRatio(700, 700);

		// Create game event to change tilemap to medium
		Event* pEvent = new GameEvent(GameEventType::ChangeTilemap, 1);
		m_eventManager->PushEvent(pEvent);
    }
    if (m_pFramework->IsKeyDown('3'))
    {
        m_pFramework->SetWindowSize(1050, 700);
		SetAspectRatio(1050, 700);

		// Create game event to change tilemap to large
		Event* pEvent = new GameEvent(GameEventType::ChangeTilemap, 2);
		m_eventManager->PushEvent(pEvent);
    }

    // Process all events
    m_eventManager->ProcessEvents(this);

	// Update current scene
	m_sceneManager->Update(deltatime);
}

void GameSample::Draw()
{
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw current scene
	m_sceneManager->Draw();

    // Check for errors.
    CheckForGLErrors();
}

bool GameSample::ProcessEvent(Event *pEvent)
{
    bool eventWasProcessed = false;

	eventWasProcessed = m_sceneManager->OnEvent(pEvent);

    return eventWasProcessed;
}

void GameSample::AddEvent(Event* ev)
{
    if (ev->GetEventType() != EventType::NotSet)
    {
        m_eventManager->PushEvent(ev);
    }
}

ShaderProgram* GameSample::GetShader(ShaderName::ShaderName shaderName) const
{
    return m_resourceManager->GetShader(shaderName);
}

vec2 GameSample::GetWorldOrigin()
{
	return m_worldOrigin;
}

vec2 GameSample::GetWorldSize()
{
	return m_worldSize;
}

Player* GameSample::GetPlayer()
{
    return m_pPlayer;
}