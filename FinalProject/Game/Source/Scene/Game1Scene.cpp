#include "GamePCH.h"

// void MinimapShaderSetup(ShaderProgram* pShader, double timeSinceGameStarted);

Game1Scene::Game1Scene(SceneManager *sceneManager, MyResourceManager* resourceManager, CollisionSystem* collisionSystem) :
    m_sceneManager(sceneManager),
    m_resourceManager(resourceManager),
    m_collisionSystem(collisionSystem),
    m_target(nullptr),
    m_level(nullptr),
    m_starTransitionObject(nullptr),
    m_enemyCount(5),
    m_transitionIsOn(false),
    m_controlIsOn(true)
{
	m_pControlCam = nullptr;
}

Game1Scene::~Game1Scene()
{
	for (GameObject *gObj : m_localGameObjects)
	{
		delete gObj;
	}
	m_localGameObjects.clear();
}

void Game1Scene::LoadContent()
{
	// ADD MESHES TO RESOURCE MANAGER
	// Make a triangle
	Mesh* triangleMesh = new Mesh();
	triangleMesh->GenerateTriangle();
	m_resourceManager->AddMesh(MeshType::Triangle, triangleMesh);

	// Make a hexagon
	Mesh *hexagonMesh = new Mesh();
	hexagonMesh->GeneratePolygon(HEXAPOINTS);
	m_resourceManager->AddMesh(MeshType::Hexagon, hexagonMesh);

	// Make a square
	Mesh *squareMesh = new Mesh();
	squareMesh->GenerateSquare();
	m_resourceManager->AddMesh(MeshType::Square, squareMesh);

	// Make star
	Mesh* star = new Mesh();
	star->GeneratePolygon(STARPOINTS, true);
	m_resourceManager->AddMesh(MeshType::Star, star);

	// Make a Pentagon
	Mesh* pent = new Mesh();
	pent->GeneratePolygon(PENTAPOINTS, false);
	m_resourceManager->AddMesh(MeshType::Pentagon, pent);



	vec2 worldSize = m_sceneManager->GetGame()->GetWorldSize();
	vec2 worldOrigin = m_sceneManager->GetGame()->GetWorldOrigin();

	// Moveable cam
	m_pControlCam = new ControllableCamera(worldOrigin, worldSize * 5 / 9, vec2(450.0f, 450.0f), vec2(150.0f, 150.0f));
	m_pControlCam->SetupCamera(m_sceneManager->GetShaderFromGame(ShaderName::Minimap), GL_TRIANGLES, CAMERA_FLAG_SHADER_SETUP);
	m_pControlCam->SetMesh(m_resourceManager->GetMesh(MeshType::Square));
	m_pControlCam->SetColor(ColorStruct(0, 255, 0, 255));
	m_pControlCam->SetBackgroundMesh(m_resourceManager->GetMesh(MeshType::Square));
    m_pControlCam->SetBackgroundColor(ColorStruct(100, 100, 100, 255));

	// Static follow cam
	StaticCamera* m_pFollowCam = new StaticCamera(vec2(25.0f, 25.0f), PROJECTION_SCALE, vec2(0.0f, 0.0f), vec2(600.0f, 600.0f));
	m_pFollowCam->SetupCamera(m_sceneManager->GetShaderFromGame(ShaderName::Default), GL_TRIANGLES, CAMERA_FLAG_RESIZE);
	m_pFollowCam->SetMesh(m_resourceManager->GetMesh(MeshType::Square));
	m_pFollowCam->SetColor(ColorStruct(0, 255, 0, 255));
	// m_pFollowCam->SetBackgroundMesh(m_resourceManager->GetMesh(MeshType::GameplayScene_Camera_Background));
	m_pFollowCam->SetFollowTarget(m_sceneManager->GetPlayer());

	// Define cameras and add to vector
	float minimapCamMax = max(worldSize.x, worldSize.y);
	StaticCamera* m_pMinimapCam = new StaticCamera(worldOrigin, vec2(minimapCamMax, minimapCamMax) / 2 + MINIMAP_CAM_PROJECTION_BUFFER, vec2(450.0f, 0.0f), vec2(150.0f, 150.0f));
	m_pMinimapCam->SetupCamera(m_sceneManager->GetShaderFromGame(ShaderName::Default), GL_LINE_LOOP, CAMERA_FLAG_OVERRIDER);
	m_pMinimapCam->SetBackgroundMesh(m_resourceManager->GetMesh(MeshType::Square));
    m_pMinimapCam->SetBackgroundColor(ColorStruct(100, 100, 100, 255));


	// Push Cameras into Vector
	m_cameras.push_back(m_pControlCam);
	m_cameras.push_back(m_pMinimapCam);
	m_cameras.push_back(m_pFollowCam);

    // Make level
    m_level = new SimpleObject(GameObjectType::Level, worldOrigin, worldSize / 2);
    m_level->SetMesh(m_resourceManager->GetMesh(MeshType::Square));
	m_level->SetColor(ColorStruct(0, 0, 0, 255));

	// Create enemies
	m_enemyPool.reserve(ENEMYPOOL_SIZE);
	for (int i = 0; i < ENEMYPOOL_SIZE; ++i)
	{
		// Create and add enemy to vectors
		SimpleObject* enemy = new SimpleObject(GameObjectType::Enemy, vec2(-100.0f, -100.0f), ENEMY_SCALE);
		enemy->SetMesh(m_resourceManager->GetMesh(MeshType::Hexagon));
		enemy->SetColor(ColorStruct(255, 0, 255, 255));
		m_collisionSystem->AddCircleColliderToWorld(enemy);

		m_enemyPool.push_back(enemy);
	}

	// Add Target
	m_target = new SimpleObject(GameObjectType::Target, END_ZONE, 6.0f);
	m_target->SetMesh(m_resourceManager->GetMesh(MeshType::Star));
    m_target->SetColor(ColorStruct(255, 255, 0, 255));
	m_collisionSystem->AddCircleColliderToWorld(m_target);


    // Add objects for transition
    m_starTransitionObject = new SimpleObject(GameObjectType::Misc, START_ZONE, vec2(1.0f, 1.0f));
    m_starTransitionObject->SetMesh(m_resourceManager->GetMesh(MeshType::Star));
    m_starTransitionObject->SetColor(ColorStruct(255, 255, 255, 0));

    // Assign default shader to both
    m_starTransitionObject->SetShader(m_sceneManager->GetShaderFromGame(ShaderName::Default));


	// Add all game objects created in this method to the 
	// localGameObjects vector
	m_localGameObjects.push_back(m_target);
    m_localGameObjects.push_back(m_level);
    m_localGameObjects.push_back(m_starTransitionObject);

	for (std::vector<Camera*>::iterator it = m_cameras.begin(); it != m_cameras.end(); ++it)
	{
		m_localGameObjects.push_back(*it);
	}
	for (std::vector<SimpleObject*>::iterator it = m_enemyPool.begin(); it != m_enemyPool.end(); ++it)
	{
		m_localGameObjects.push_back(*it);
	}

	// Load up level 1
	ReloadLevel(0);
}

void Game1Scene::ReloadActiveGameObjects()
{
    // Clear currently active vector of game objects
    m_gameObjects.clear();

    // Start pushing in the active game objects
    m_gameObjects.push_back(m_sceneManager->GetPlayer());
    m_gameObjects.push_back(m_target);

    // Load in enemies
    for (unsigned int i = 0; i < m_enemyCount; ++i)
    {
        m_gameObjects.push_back(m_enemyPool[i]);
    }
}

void Game1Scene::ReloadLevel(unsigned int numberOfEnemies)
{
	// Add number of enemies to enemyCount
	m_enemyCount += numberOfEnemies;
	if (m_enemyCount > ENEMYPOOL_SIZE)
		m_enemyCount = ENEMYPOOL_SIZE;

    ReloadActiveGameObjects();

    // Load in enemies
    for (unsigned int i = 0; i < m_enemyCount; ++i)
    {
		vec2 enemyPos = GetValidRandomPosition();
        m_enemyPool[i]->SetPosition(enemyPos);
	}

	m_sceneManager->GetPlayer()->SetPosition(START_ZONE);
}

void Game1Scene::UpdateCameraViewports(const GameCore* game)
{
	// Get current window size
	Point currentWindowSizePoint = game->GetWindowSize();
	vec2 currentWindowSize((float)currentWindowSizePoint.x, (float)currentWindowSizePoint.y);

	// Get old window size
	Point prevWindowSizePoint = game->GetPreviousWindowSize();
	vec2 prevWindowSize((float)prevWindowSizePoint.x, (float)prevWindowSizePoint.y);

	// Ratio of new window size to old window size
	vec2 windowSizeScaling = currentWindowSize / prevWindowSize;

	for (Camera *cam : m_cameras)
	{
		// Get vectors to be scaled
		vec2 camVPCoord = cam->GetVPCoord();
		vec2 camVPSize = cam->GetVPSize();
		vec2 camProj = cam->GetProjection();

		// Make new vec2's
		vec2 newCamVPCoord = camVPCoord;
		vec2 newCamVPSize = camVPSize;
		vec2 newCamProj = camProj;


		// Get percentage of old viewport size that the cam took up
		vec2 viewportSizeScale = camVPSize / prevWindowSize;
		vec2 projectionScale = camProj / camVPSize;

		if (cam->CamCanResize())
		{
			// Scale coord based on original size ratio
			newCamVPCoord = currentWindowSize * (camVPCoord / prevWindowSize);
			newCamVPSize = currentWindowSize * viewportSizeScale;
			newCamProj = newCamVPSize * projectionScale;
		}
		else
		{
			// Position the viewport coordinates relative to the whatever quadrant it's in
			vec2 halfVPSize = prevWindowSize / 2;
			vec2 translatedCamVPCoord = camVPCoord - halfVPSize;
			vec2 furthestPoint = translatedCamVPCoord;

			// Find furthest point to new center
			float x1Abs = fabs(furthestPoint.x);
			float x2Abs = fabs(furthestPoint.x + camVPSize.x);
			float y1Abs = fabs(furthestPoint.y);
			float y2Abs = fabs(furthestPoint.y + camVPSize.y);
			furthestPoint.x = max(x1Abs, x2Abs) == x1Abs ? furthestPoint.x : furthestPoint.x + camVPSize.x;
			furthestPoint.y = max(y1Abs, y2Abs) == y1Abs ? furthestPoint.y : furthestPoint.y + camVPSize.y;

			// do scale
			furthestPoint *= windowSizeScaling;

			// Figure out what quadrant it is in
			// mirror back
			float xSign = furthestPoint.x == 0 ? 0 : (furthestPoint.x / fabs(furthestPoint.x));
			float ySign = furthestPoint.y == 0 ? 0 : (furthestPoint.y / fabs(furthestPoint.y));

			// Do final translation of new Full Screen size
			vec2 halfGameVPSize = currentWindowSize / 2;
			furthestPoint += halfGameVPSize;

			newCamVPCoord = furthestPoint;

			// add or subtract depending on the quadrant
			if (xSign > 0)
				newCamVPCoord.x -= camVPSize.x;
			if (ySign > 0)
				newCamVPCoord.y -= camVPSize.y;

			// Keep cam size the same as before (static cam)
			newCamVPSize = camVPSize;
		}

		// Set values to cam
		cam->SetVPCoord(newCamVPCoord);
		cam->SetVPSize(newCamVPSize);
		cam->SetProjection(newCamProj);
	}
}

void Game1Scene::Update(float deltatime)
{
    if (m_transitionIsOn)
    {
        vec2 starCurrentScale = m_starTransitionObject->GetScale();
        float scaleLength = starCurrentScale.Length();
        if (scaleLength > MAX_TRANSITION_SCALE)
        {
            GameEvent* gEvent = new GameEvent(GameEventType::TransitionEnd);
            m_sceneManager->GetGame()->AddEvent(gEvent);
        }
        
        starCurrentScale += TRANSITION_SPEED * deltatime;
        m_starTransitionObject->SetScale(starCurrentScale);
    }

    if (m_controlIsOn)
    {
        for (Camera* cam : m_cameras)
        {
            cam->Update(deltatime);
        }

        for (GameObject *gObj : m_gameObjects)
        {
            gObj->Update(deltatime);
        }

        // DO COLLISION HERE FROM SYSTEM
        m_collisionSystem->CheckCollisions(m_sceneManager->GetGame());
    }
}

void Game1Scene::Draw()
{
    if (m_transitionIsOn)
    {
        // Do cutout thing
        m_starTransitionObject->Draw(m_sceneManager->GetPlayer()->GetPosition(), vec2(20.0f, 20.0f));
        //m_starTransitionBackground->Draw(m_sceneManager->GetPlayer()->GetPosition(), m_sceneManager->GetGame())
    }

	for (Camera *cam : m_cameras)
	{
		bool skipDrawObjects = false;

		// Set up camera to its OpenGL specs
		cam->PreDrawSetup();

		// Setup optional shader stuff
		// Do Blip
		if (cam->CamNeedsMoreShaderSetup())
		{
			// MinimapShaderSetup(cam->GetShader(), m_sceneManager->GetGame()->GetTimeSinceStart());
			double time = fmod(m_sceneManager->GetGame()->GetTimeSinceStart(), 1.0);
			skipDrawObjects = time < 0.15;
		}

		if (!skipDrawObjects)
		{
			if (cam->IsOverrider())
			{
				// Draw all objects for the given camera
				for (GameObject *gObj : m_gameObjects)
				{
					gObj->DrawOverride(cam->GetPosition(), cam->GetProjection(), cam->GetShader(), cam->GetDrawMode());
				}
				for (Camera* drawCam : m_cameras)
				{
					drawCam->DrawOverride(cam->GetPosition(), cam->GetProjection(), cam->GetShader(), cam->GetDrawMode());
				}
			}
			else
			{
				// Draw all objects for the given camera
				for (GameObject *gObj : m_gameObjects)
				{
					gObj->Draw(cam->GetPosition(), cam->GetProjection(), cam->GetShader());
				}
				for (Camera* drawCam : m_cameras)
				{
					drawCam->Draw(cam->GetPosition(), cam->GetProjection(), cam->GetShader());
				}
			}
		}

        m_level->Draw(cam->GetPosition(), cam->GetProjection(), cam->GetShader());
		cam->DrawBackground();
	}
}

bool Game1Scene::OnEvent(Event *pEvent)
{
	switch (pEvent->GetEventType())
	{
	case EventType::Input:
	{
        // Scene specific controls
        // SAVE
        InputEvent* pInput = (InputEvent*)pEvent;
        if (pInput->GetKeyID() == VK_F5 && pInput->GetKeyState() == KeyState::Pressed)
        {
            SaveScene();
        }
        else if (pInput->GetKeyID() == VK_F9 && pInput->GetKeyState() == KeyState::Pressed)
        {
            LoadScene();
        }

        if (m_pControlCam != nullptr)
        {
            m_pControlCam->OnEvent(pInput);
            return true;
        }
	}
	break;
	case EventType::Collision:
	{
		CollisionEvent* pCol = (CollisionEvent*)pEvent;

		// Check if target and player collided
		if (pCol->GetObject1()->GetObjectID() == GameObjectType::Player ||
			pCol->GetObject2()->GetObjectID() == GameObjectType::Player)
		{
			if (pCol->GetObject1()->GetObjectID() == GameObjectType::Target ||
				pCol->GetObject2()->GetObjectID() == GameObjectType::Target)
			{
				GameEvent* gEvent = new GameEvent(GameEventType::TransitionStart);
				m_sceneManager->GetGame()->AddEvent(gEvent);
				return true;
			}
			else
			{
				GameEvent* gEvent = new GameEvent(GameEventType::LoseLevel);
				m_sceneManager->GetGame()->AddEvent(gEvent);
				return true;
			}
		}
	}
	break;
	case EventType::Game:
	{
		GameEvent* pGame = (GameEvent*)pEvent;

		if (pGame->GetGameEventType() == GameEventType::LoseLevel)
		{
			ReloadLevel(0);
			return true;
		}
		else if (pGame->GetGameEventType() == GameEventType::TransitionStart)
		{
            StartTransition();
			return true;
		}
        else if (pGame->GetGameEventType() == GameEventType::TransitionEnd)
        {
            EndTransition();
            return true;
        }
	}
	break;
	default:
		break;
	}

	return false;
}

int Game1Scene::SaveScene()
{
    cJSON* jRoot = cJSON_CreateObject();

    // GAMEPLAY SCENE SAVING
    /*m_controlIsOn = false;
    m_transitionIsOn = true;*/
    // Star scale
    // Star position based on player position -> set after getting player
    cJSON* jGameplayScene = cJSON_CreateObject();
    cJSON_AddItemToObject(jRoot, "Scene", jGameplayScene);
    
    cJSON_AddBoolToObject(jGameplayScene, "control", m_controlIsOn);
    cJSON_AddBoolToObject(jGameplayScene, "transition", m_transitionIsOn);

    // star transition object
    vec2 starScale = m_starTransitionObject->GetScale();
    cJSON_AddNumberToObject(jGameplayScene, "starScaleX", starScale.x);
    cJSON_AddNumberToObject(jGameplayScene, "starScaleY", starScale.y);


    // PLAYER SAVING
    cJSON* jPlayer = cJSON_CreateObject();
    cJSON_AddItemToObject(jRoot, "Player", jPlayer);

    // Add items to player object
    Player* pPlayer = m_sceneManager->GetPlayer();
    vec2 playerPos = pPlayer->GetPosition();
    cJSON_AddNumberToObject(jPlayer, "posx", playerPos.x);
    cJSON_AddNumberToObject(jPlayer, "posy", playerPos.y);


    // ENEMY SAVING
    cJSON* jEnemyArray = cJSON_CreateArray();
    cJSON_AddItemToObject(jRoot, "EnemyArray", jEnemyArray);
    
    for (unsigned int i = 0; i < m_enemyCount; ++i)
    {
        cJSON* jEnemy = cJSON_CreateObject();
        cJSON_AddItemToArray(jEnemyArray, jEnemy);

        vec2 ePos = m_enemyPool[i]->GetPosition();
        cJSON_AddNumberToObject(jEnemy, "posx", ePos.x);
        cJSON_AddNumberToObject(jEnemy, "posy", ePos.y);
    }


    char* jsonstr = cJSON_Print(jRoot);

    // quick write to a location
    FILE* filehandle;
    errno_t error = fopen_s(&filehandle, SAVEFILE_LOC, "wb");

    if (filehandle)
    {
        fwrite(jsonstr, 1, strlen(jsonstr), filehandle);
        fclose(filehandle);
    }


    // Cleanup
    cJSON_Delete(jRoot);
    free(jsonstr);

    return 0;
}

int Game1Scene::LoadScene()
{
    char* fileStr = LoadCompleteFile(SAVEFILE_LOC, 0);
    cJSON* jRoot = cJSON_Parse(fileStr);

    // GAMEPLAY SCENE LOAD
    cJSON* jScene = cJSON_GetObjectItem(jRoot, "Scene");

    cJSON* control = cJSON_GetObjectItem(jScene, "control");
    if (control)
        m_controlIsOn = control->valueint;

    cJSON* transition = cJSON_GetObjectItem(jScene, "transition");
    if (transition)
        m_transitionIsOn = transition->valueint;
    
    // Star Scale
    double starX = 1.0f;
    double starY = 1.0f;
    cJSON* starScaleX = cJSON_GetObjectItem(jScene, "starScaleX");
    if (starScaleX)
        starX = starScaleX->valuedouble;

    cJSON* starScaleY = cJSON_GetObjectItem(jScene, "starScaleY");
    if (starScaleY)
        starY = starScaleY->valuedouble;

    m_starTransitionObject->SetScale(vec2((float)starX, (float)starY));


    // PLAYER LOAD
    cJSON* jPlayer = cJSON_GetObjectItem(jRoot, "Player");

    // Get Position
    cJSON* jPlayerX = cJSON_GetObjectItem(jPlayer, "posx");
    cJSON* jPlayerY = cJSON_GetObjectItem(jPlayer, "posy");
    double playerX = START_ZONE.x;
    double playerY = START_ZONE.y;

    if (jPlayerX)
        playerX = jPlayerX->valuedouble;
    if (jPlayerY)
        playerY = jPlayerY->valuedouble;

    // Set Position
    Player* pPlayer = m_sceneManager->GetPlayer();
    pPlayer->SetPosition(vec2((float)playerX, (float)playerY));
    
    if (m_transitionIsOn)
        m_starTransitionObject->SetPosition(pPlayer->GetPosition());


    // ENEMY POSITIONS
    cJSON* jEnemyArray = cJSON_GetObjectItem(jRoot, "EnemyArray");

    m_enemyCount = (unsigned int)cJSON_GetArraySize(jEnemyArray);

    // Set positions
    for (unsigned int i = 0; i < m_enemyCount; ++i)
    {
        cJSON* jEnemy = cJSON_GetArrayItem(jEnemyArray, (int)i);

        vec2 enemyPos;
        cJSON* enemyX = cJSON_GetObjectItem(jEnemy, "posx");
        cJSON* enemyY = cJSON_GetObjectItem(jEnemy, "posy");

        if (enemyX)
            enemyPos.x = (float)enemyX->valuedouble;
        if (enemyY)
            enemyPos.y = (float)enemyY->valuedouble;

        m_enemyPool[i]->SetPosition(enemyPos);
    }

    ReloadActiveGameObjects();


    // Cleanup
    cJSON_Delete(jRoot);
    delete fileStr;

    return 0;
}

vec2 Game1Scene::GetValidRandomPosition()
{
	// Kind of wasteful
	vec2 worldSize = m_sceneManager->GetGame()->GetWorldSize() - 2*ENEMY_SCALE;
	vec2 worldOrigin = m_sceneManager->GetGame()->GetWorldOrigin();

	vec2 position(0.0f, 0.0f);
	do
	{
		position.x = (rand() % (int)(worldSize.x)) - (worldSize.x / 2) + worldOrigin.x;
		position.y = (rand() % (int)(worldSize.y)) - (worldSize.y / 2) + worldOrigin.y;
	} while (position.x <= START_ZONE.x + 4 * ENEMY_SCALE.x && position.y >= START_ZONE.y - 4 * ENEMY_SCALE.y ||
			 position.x >= END_ZONE.x - 4 * ENEMY_SCALE.x && position.y <= END_ZONE.y + 4 * ENEMY_SCALE.y);

	return position;
}

void Game1Scene::StartTransition()
{
    // turn on necessary booleans
    m_controlIsOn = false;
    m_transitionIsOn = true;

    // Reset star size
    m_starTransitionObject->SetScale(vec2(1.0f, 1.0f)); 
    m_starTransitionObject->SetPosition(m_sceneManager->GetPlayer()->GetPosition());
}

void Game1Scene::EndTransition()
{
    m_controlIsOn = true;
    m_transitionIsOn = false;

    // Reload Level
    ReloadLevel(1);
}

//void MinimapShaderSetup(ShaderProgram* pShader, double timeSinceGameStarted)
//{
//	// Use Shader
//	glUseProgram(pShader->GetProgram());
//
//	GLint uLoc = glGetUniformLocation(pShader->GetProgram(), "u_TimeVal");
//	if (uLoc != -1)
//	{
//		glUniform1f(uLoc, (float)timeSinceGameStarted);
//	}
//
//	// Check for errors.
//	CheckForGLErrors();
//}