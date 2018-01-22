#include "GamePCH.h"

#include <iostream>

TilemapScene::TilemapScene(SceneManager *sceneManager, MyResourceManager *resourceManager, CollisionSystem* collisionSystem) :
	m_sceneManager(sceneManager),
	m_resourceManager(resourceManager),
	m_collisionSystem(collisionSystem),
	m_pCurrTilemap(nullptr),
	m_pPathfinder(nullptr),
	m_mouseIsOnValidLocation(false),
	m_enemySpawnTimer(ENEMY_SPAWN_TIME),
	m_mouseTimer(0.0f)
{
}

TilemapScene::~TilemapScene()
{
	// Delete particle system
	delete m_particleSystem;
	delete m_particleRenderer;

	delete m_scoreDisp;

	delete m_pPathfinder;

	delete m_pStaticCam;
	delete m_pScoreCam;

	// Delete All Nodes (enemies, bullets, towers)
	delete m_pRoot;

	// Delete tilemaps
	for (Tilemap* tilemap : m_pTilemaps)
	{
		delete tilemap;
	}
	m_pTilemaps.clear();
}

void TilemapScene::OnSurfaceChanged(int width, int height)
{
    // Set OpenGL to draw to the entire window.
    glViewport( 0, 0, width, height );
}

void TilemapScene::UpdateCameraViewports(const GameCore* game)
{
	Vector2Int aspectRatio = m_sceneManager->GetGame()->GetAspectRatio();
	Vector2 worldSize = m_pCurrTilemap->GetWorldSize();

	m_pStaticCam->SetPosition(worldSize / 2.0f);
	Vector2 camProj = worldSize / 1.5f;


	// Get current window size
	Point currentWindowSizePoint = game->GetWindowSize();
	vec2 currentWindowSize((float)currentWindowSizePoint.x, (float)currentWindowSizePoint.y);

	// Ratio of new window size to original window size
	vec2 windowSizeScaling = currentWindowSize / vec2((float)aspectRatio.x, (float)aspectRatio.y);

	float minComponent = min(windowSizeScaling.x, windowSizeScaling.y);

	camProj *= windowSizeScaling / minComponent;


	// Score camera
	vec2 scoreProj = SCORE_CAM_PROJ;
	scoreProj *= windowSizeScaling / minComponent;


	// Update camera projection
	m_pStaticCam->SetProjection(camProj);
	m_pScoreCam->SetProjection(scoreProj);
}

void TilemapScene::LoadContent()
{
	// Turn on depth buffer testing.
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Turn on alpha blending.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Only Texture to be active
	glActiveTexture(GL_TEXTURE0 + 0);


	// Create root of scene graph
	m_pRoot = new SceneNode(vec2(0.0f, 0.0f), vec2(1.0f, 1.0f));

	// set up the resource manager with the required resources
	LoadResources();

	// Read the JSON for bomberman spritesheet width and height
	vec2 bmanSize = GetSpriteSize("Data/JSON/BomberZelda.json");

	ShaderProgram* pShaderParticle = m_resourceManager->GetShader(ShaderName::Particle);
	ShaderProgram* pShaderHealth = m_resourceManager->GetShader(ShaderName::Healthbar);
	ShaderProgram* pShaderScore = m_resourceManager->GetShader(ShaderName::Score);


	// Create tilemaps
	m_pSmallTilemap = new Tilemap(m_resourceManager->GetMesh(MeshType::TexSquare), vec2(0.0f, 0.0f), vec2(1.0f, 1.0f));
	m_pSmallTilemap->InitializeTilemapSmall();
	m_pMediumTilemap = new Tilemap(m_resourceManager->GetMesh(MeshType::TexSquare), vec2(0.0f, 0.0f), vec2(1.0f, 1.0f));
	m_pMediumTilemap->InitializeTilemapMedium();
	m_pLargeTilemap = new Tilemap(m_resourceManager->GetMesh(MeshType::TexSquare), vec2(0.0f, 0.0f), vec2(1.0f, 1.0f));
	m_pLargeTilemap->InitializeTilemapLarge();

	m_pTilemaps.push_back(m_pSmallTilemap);
	m_pTilemaps.push_back(m_pMediumTilemap);
	m_pTilemaps.push_back(m_pLargeTilemap);

	// Small Tilemap is default
	m_pCurrTilemap = m_pSmallTilemap;
	m_currTilemapIndex = 0;

	GLuint spritesheetId = m_resourceManager->GetTexture(TexType::BombermanSpriteSheet);

	// Find each file in the spritesheet json
	SpriteFormat sprite;

	GetSpriteInfo(sprite, "Data/JSON/BomberZelda.json", "Floor.png");
	for (Tilemap* tilemap : m_pTilemaps)
		tilemap->SetTileMesh(TileType::Grass, spritesheetId, sprite);

	GetSpriteInfo(sprite, "Data/JSON/BomberZelda.json", "TileWater5.png");
	for (Tilemap* tilemap : m_pTilemaps)
		tilemap->SetTileMesh(TileType::Water, spritesheetId, sprite);

	GetSpriteInfo(sprite, "Data/JSON/BomberZelda.json", "TileBridge.png");
	for (Tilemap* tilemap : m_pTilemaps)
		tilemap->SetTileMesh(TileType::Wall, spritesheetId, sprite);

	GetSpriteInfo(sprite, "Data/JSON/BomberZelda.json", "TileEmpty.png");
	for (Tilemap* tilemap : m_pTilemaps)
		tilemap->SetTileMesh(TileType::Start, spritesheetId, sprite);

	for (Tilemap* tilemap : m_pTilemaps)
		tilemap->SetTileMesh(TileType::End, spritesheetId, sprite);

	GetSpriteInfo(m_towerSpriteInfo, "Data/JSON/BomberZelda.json", "Wall.png");

	for (Tilemap* tilemap : m_pTilemaps)
		tilemap->SetShader(m_resourceManager->GetShader(ShaderName::Texture));


	// Instantiate Particle stuff
	GetSpriteInfo(sprite, "Data/JSON/BomberZelda.json", "Fire1.png");

	m_particleRenderer = new ParticleRenderer(pShaderParticle, spritesheetId, sprite);
	m_particleRenderer->SetSpritesheetSize(bmanSize);

	m_particleSystem = new ParticleSystem(m_particleRenderer);

	// Score
	m_scoreDisp = new ScoreDisplay(pShaderScore, m_resourceManager->GetTexture(TexType::ScoreStrip));
	m_scoreDisp->UpdateScore(INITIAL_SCORE);


	// Make a camera?
	Vector2 worldSize = m_pCurrTilemap->GetWorldSize();
	m_pStaticCam = new StaticCamera(worldSize / 2.0f, worldSize / 1.5f);
	m_pStaticCam->SetupCamera(m_resourceManager->GetShader(ShaderName::Texture), GL_TRIANGLE_FAN, CAMERA_FLAG_RESIZE);

	m_pScoreCam = new StaticCamera(vec2(0.0f, 0.0f), vec2(200.0f, 200.0f));

	// Set up pathfinder
	Vector2Int maxMapSize = m_pLargeTilemap->GetMapSize();
	Vector2Int currMapSize = m_pCurrTilemap->GetMapSize();
	m_pPathfinder = new Pathfinder(m_pCurrTilemap, currMapSize.x, currMapSize.y);
	m_pPathfinder->PreAllocateGridGraph(maxMapSize.x, maxMapSize.y);
	m_pPathfinder->Reset();

	m_pPathfinder->SeedGraph();
	m_pPathfinder->DrawGraph();

	// Setup enemy pool
	InitializeEnemies();

	// Bullets
	InitializeBullets();

	// Towers
	InitializeTowers();
	
	// Mouse overlay setup
	m_mouseMesh = m_resourceManager->GetMesh(MeshType::Square);
	m_mouseShader = m_resourceManager->GetShader(ShaderName::Default);
	m_mouseMeshPos = m_pStaticCam->GetPosition();
	m_mouseTilePos = m_pCurrTilemap->GetTileAtWorldPos(m_mouseMeshPos);


	// Select Active Tilemap
	ChangeActiveTilemap(m_pSmallTilemap);

	// Draw map
	OutputMessage("\n\n\n MAP SHOWING INDICES AND NON-WALKABLE TILES\n");
	m_pCurrTilemap->DrawWalkabilityMap();

	ivec2 startpoint(1, 5);
	ivec2 endpoint(8, 1);
	bool found = m_pPathfinder->FindPath(startpoint.x, startpoint.y, endpoint.x, endpoint.y);

	OutputMessage("Found = %d\n", found);

	int path[255];
	int len = m_pPathfinder->GetPath(path, 255, endpoint.x, endpoint.y);

	OutputMessage("Length = %d\n", len);
	for (int i = 0; i<len; i++)
	{
		OutputMessage("%d -> ", path[i]);
	}
	OutputMessage("\n");
	OutputMessage("done\n");
	

    // Check for errors.
    CheckForGLErrors();
}


void TilemapScene::InitializeEnemies()
{
	// Stuff needed from the resource manager
	ShaderProgram*	pShaderHealth		= m_resourceManager->GetShader(ShaderName::Healthbar);
	ShaderProgram*	pShaderTexture		= m_resourceManager->GetShader(ShaderName::Texture);
	GLuint			spritesheetTexID	= m_resourceManager->GetTexture(TexType::BombermanSpriteSheet);
	Mesh*			texMesh				= m_resourceManager->GetMesh(MeshType::TexSquare);
	Mesh*			squareMesh			= m_resourceManager->GetMesh(MeshType::Square);

	// Get all the sprites for enemy
	std::vector<std::string> animStrings;
	animStrings.push_back("OctorocDown1.png");
	animStrings.push_back("OctorocDown2.png");
	animStrings.push_back("OctorocUp1.png");
	animStrings.push_back("OctorocUp2.png");
	animStrings.push_back("OctorocLeft1.png");
	animStrings.push_back("OctorocLeft2.png");
	animStrings.push_back("OctorocRight1.png");
	animStrings.push_back("OctorocRight2.png");

	// Get the json stuff from file
	std::vector<SpriteFormat> spriteInfo;
	GetMultipleSpriteInfo(spriteInfo, "Data/JSON/BomberZelda.json", animStrings);

	// There must be two sprites per direction
	assert(spriteInfo.size() == 8);


	for (unsigned int i = 0; i < ENEMY_POOL_SIZE; ++i)
	{
		Enemy* enemy = new Enemy(m_pStaticCam->GetPosition() + vec2(2.0f * TILE_SIZE, 0.0f), vec2(TILE_SIZE / 2.0f, TILE_SIZE / 2.0f),
			texMesh, spritesheetTexID, this);
		enemy->SetShader(pShaderTexture);
		// enemy->SetSpeed(0.0f);
		enemy->Deactivate();

		// Setup enemy sprite stuff
		enemy->InsertSpriteFormat(EnemyDir::Down, spriteInfo[0]);
		enemy->InsertSpriteFormat(EnemyDir::Down, spriteInfo[1]);

		enemy->InsertSpriteFormat(EnemyDir::Up, spriteInfo[2]);
		enemy->InsertSpriteFormat(EnemyDir::Up, spriteInfo[3]);

		enemy->InsertSpriteFormat(EnemyDir::Left, spriteInfo[4]);
		enemy->InsertSpriteFormat(EnemyDir::Left, spriteInfo[5]);

		enemy->InsertSpriteFormat(EnemyDir::Right, spriteInfo[6]);
		enemy->InsertSpriteFormat(EnemyDir::Right, spriteInfo[7]);

		m_collisionSystem->AddCircleColliderToWorld(enemy);
		m_pEnemies.push_back(enemy);

		Healthbar *hb = new Healthbar(enemy, pShaderHealth, vec2(0.0f, 80.0f), vec2(1.0f, 1.0f));
		hb->SetMesh(squareMesh);

		// Attach to tilemap
		m_pRoot->AttachChild(enemy);
	}

	//m_pEnemy = m_pEnemies[0];
	//m_pEnemy->Activate();
}

void TilemapScene::InitializeBullets()
{
	ShaderProgram* defaultShader = m_resourceManager->GetShader(ShaderName::Default);
	for (int i = 0; i < BULLET_POOL_SIZE; ++i)
	{
		Bullet* bull = new Bullet(vec2(0.0f, 0.0f), vec2(10.0f, 10.0f), m_resourceManager->GetMesh(MeshType::Circle));
		bull->SetShader(defaultShader);
		bull->SetColor({ 0, 0, 255, 255 });
		bull->Deactivate();
		m_pBullets.push_back(bull);

		m_collisionSystem->AddCircleColliderToWorld(bull);

		m_pRoot->AttachChild(bull);
	}
}

void TilemapScene::InitializeTowers()
{
	for (int i = 0; i < TOWER_POOL_SIZE; ++i)
	{
		Tower* pTower = new Tower(this, vec2(0.0f, 0.0f), vec2(TILE_SIZE / 2.0f, TILE_SIZE / 2.0f),
			m_resourceManager->GetMesh(MeshType::TexSquare), m_resourceManager->GetTexture(TexType::BombermanSpriteSheet), m_towerSpriteInfo);
		pTower->SetShader(m_resourceManager->GetShader(ShaderName::Texture));
		pTower->Deactivate();

		m_pTowers.push_back(pTower);
		m_pRoot->AttachChild(pTower);
	}
}


void TilemapScene::Update(float deltatime)
{
	// Enemy spawn timer
	m_enemySpawnTimer -= deltatime;
	if (m_enemySpawnTimer < 0.0f)
	{
		SpawnEnemy();
		m_enemySpawnTimer += ENEMY_SPAWN_TIME;
	}

	m_mouseTimer += deltatime;

    m_pRoot->Update(deltatime);

	m_particleSystem->Update(deltatime);

	// DO COLLISION HERE FROM SYSTEM
	m_collisionSystem->CheckCollisions(m_sceneManager->GetGame());

	UpdateMouseOverlay(deltatime);
}

void TilemapScene::Draw()
{
    // Setup the values we will clear to, then actually clear the color and depth buffers.
    glClearColor( 0.0f, 0.0f, 0.4f, 0.0f ); // (Red, Green, Blue, Alpha) - Dark blue
    glClearDepth( 1 ); // 1 is maximum depth
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// draw current tilemap
	m_pCurrTilemap->Draw(m_pStaticCam->GetPosition(), m_pStaticCam->GetProjection());
	
    // Draw Scenegraph
    m_pRoot->Draw(m_pStaticCam->GetPosition(), m_pStaticCam->GetProjection());


    m_particleRenderer->Draw(m_pStaticCam->GetPosition(), m_pStaticCam->GetProjection());
    m_scoreDisp->Draw(vec2(-200.0f, -200.0f), m_pScoreCam->GetProjection(), vec2(25.0f, 25.0f));

	// Mouse Overlay
	if (m_mouseIsOnValidLocation)
		m_mouseMesh->SetColor(VALID_SPAWN_TILE_COL);
	else
		m_mouseMesh->SetColor(INVALID_SPAWN_TILE_COL);
	m_mouseMesh->Draw(vec2(TILE_SIZE / 2.0f, TILE_SIZE / 2.0f), m_mouseMeshPos, m_pStaticCam->GetPosition(), m_pStaticCam->GetProjection(), m_mouseShader);


    // Check for errors.
    CheckForGLErrors();
}

bool TilemapScene::OnEvent(Event *pEvent)
{
	switch (pEvent->GetEventType())
	{
	case EventType::Input:
	{
		InputEvent * pInput = (InputEvent*)pEvent;
		if (pInput->GetDeviceType() == DeviceType::Keyboard)
		{
			if (pInput->GetKeyState() == KeyState::Pressed)
			{
				if (pInput->GetKeyID() == VK_SPACE)
				{
					SpawnEnemy();
				}
				else if (pInput->GetKeyID() == VK_F5)
				{
					SaveGame();
					return true;
				}
				else if (pInput->GetKeyID() == VK_F9)
				{
					LoadGame();
					return true;
				}
			}
		}
		else if (pInput->GetDeviceType() == DeviceType::Mouse)
		{
			if (pInput->GetKeyState() == KeyState::Pressed)
			{
				if (pInput->GetKeyID() == MK_LBUTTON)
				{
					if (m_mouseIsOnValidLocation)
					{
						vec2 tilePos = m_mouseMeshPos;
						SpawnTowerAtLocation(tilePos);
						return true;
					}
				}
			}
		}
	}
		break;
	case EventType::Collision:
	{
		CollisionEvent * pCol = (CollisionEvent*)pEvent;

		// Make sure both objects are still active
		if (!(pCol->GetObject1()->IsActive() && pCol->GetObject2()->IsActive()))
			return false;

		if (pCol->GetObject1()->GetObjectID() == GameObjectType::Bullet ||
			pCol->GetObject2()->GetObjectID() == GameObjectType::Bullet)
		{
			if (pCol->GetObject1()->GetObjectID() == GameObjectType::Enemy ||
				pCol->GetObject2()->GetObjectID() == GameObjectType::Enemy)
			{
				// Reduce enemy health
				// If enemy health is 0, deactivate and spawn a burst of particles
				Enemy* enemy = nullptr;
				Bullet* bullet = nullptr;

				if (pCol->GetObject1()->GetObjectID() == GameObjectType::Enemy)
				{
					enemy = (Enemy*)pCol->GetObject1();
					bullet = (Bullet*)pCol->GetObject2();
				}
				else
				{
					enemy = (Enemy*)pCol->GetObject2();
					bullet = (Bullet*)pCol->GetObject1();
				}

				// Deactivate bullet
				bullet->Deactivate();

				// Damage Calculations
				int eHealth = enemy->GetHealth() - BULLET_DAMAGE;
				enemy->SetHealth(eHealth);

				if (eHealth <= 0)
				{
					if (enemy->IsActive())
						m_particleSystem->CreateBurstAtLocation(enemy->GetWorldPosition(), 1.0f, 10);

					enemy->Deactivate(); 
					
					ModifyScore(25);
				}

				return true;
			}
		}
	}
		break;
	case EventType::Game:
	{
		GameEvent* pGame = (GameEvent*)pEvent;
		m_currTilemapIndex = pGame->GetTilemapNum();

		switch (m_currTilemapIndex)
		{
		case 0:
			ChangeActiveTilemap(m_pSmallTilemap);
			break;
		case 1:
			ChangeActiveTilemap(m_pMediumTilemap);
			break;
		case 2:
			ChangeActiveTilemap(m_pLargeTilemap);
			break;
		}
	}
		break;
	default:
		break;
	}

	return false;
}

void TilemapScene::LoadResources()
{
	// Create our shaders.
	//ShaderProgram* pShaderColor = new ShaderProgram("Data/Shaders/color.vert", "Data/Shaders/color.frag");
	ShaderProgram* pShaderTexture = new ShaderProgram("Data/Shaders/tex.vert", "Data/Shaders/tex.frag");
	ShaderProgram* pShaderParticle = new ShaderProgram("Data/Shaders/particles.vert", "Data/Shaders/particles.frag");
	ShaderProgram* pShaderScore = new ShaderProgram("Data/Shaders/score.vert", "Data/Shaders/score.frag");
	ShaderProgram* pShaderHealth = new ShaderProgram("Data/Shaders/healthbar.vert", "Data/Shaders/healthbar.frag");

	m_resourceManager->AddShader(ShaderName::Texture, pShaderTexture);
	m_resourceManager->AddShader(ShaderName::Particle, pShaderParticle);
	m_resourceManager->AddShader(ShaderName::Score, pShaderScore);
	m_resourceManager->AddShader(ShaderName::Healthbar, pShaderHealth);

	// Add textures to the resource manager
	m_resourceManager->GenerateTexture(TexType::BombermanSpriteSheet, "Data/Textures/BomberZelda.png");
	m_resourceManager->GenerateTexture(TexType::ScoreStrip, "Data/Textures/NumberSpritesheet.png");


	// Read the JSON for bomberman spritesheet width and height
	vec2 bmanSize = GetSpriteSize("Data/JSON/BomberZelda.json");
	// Generate mesh
	Mesh* texMesh = new Mesh();
	texMesh->GenerateTextureSquare(bmanSize);
	m_resourceManager->AddMesh(MeshType::TexSquare, texMesh);

	// Make a square
	Mesh *squareMesh = new Mesh();
	squareMesh->GenerateSquare();
	m_resourceManager->AddMesh(MeshType::Square, squareMesh);

}


void TilemapScene::ModifyScore(int amount)
{
	int newScore = m_scoreDisp->GetScore();

	newScore += amount;

	m_scoreDisp->UpdateScore(newScore);
}

bool TilemapScene::GetSpriteInfo(SpriteFormat& sprite, std::string filename, std::string jsonName)
{
	// Load JSON file
	// Data/JSON/Bomberman.json
	long jsonSLen = 0;
	char* jsonString = LoadCompleteFile(filename.c_str(), &jsonSLen);

	// Parse JSON file
	cJSON* jRoot = cJSON_Parse(jsonString);

	// Grab Files array
	cJSON* jFilesArray = cJSON_GetObjectItem(jRoot, "Files");


	int jFilesArraySize = cJSON_GetArraySize(jFilesArray);

	for (int i = 0; i < jFilesArraySize; ++i)
	{
		cJSON* jArrayItem = cJSON_GetArrayItem(jFilesArray, i);

		// Get filename of array item
		cJSON* jFilename = cJSON_GetObjectItem(jArrayItem, "filename");
		if (jFilename)
		{
			if (jFilename->valuestring != jsonName)
				continue;
		}

		// fill sprite with info
		FindJSONSpriteInfo(sprite, jArrayItem);

		// Break out of the loop
		break;
	}

	// Cleanup
	cJSON_Delete(jRoot);
	delete jsonString;

	return true;
}

bool TilemapScene::GetMultipleSpriteInfo(std::vector<SpriteFormat>& spriteVec, std::string filename, std::vector<std::string> stringsToFind)
{
	// Load JSON file
	// Data/JSON/Bomberman.json
	long jsonSLen = 0;
	char* jsonString = LoadCompleteFile(filename.c_str(), &jsonSLen);

	// Parse JSON file
	cJSON* jRoot = cJSON_Parse(jsonString);

	// Grab Files array
	cJSON* jFilesArray = cJSON_GetObjectItem(jRoot, "Files");


	int itemsLeft = stringsToFind.size();
	int jFilesArraySize = cJSON_GetArraySize(jFilesArray);

	for (std::string itemName : stringsToFind)
	{
		for (int i = 0; i < jFilesArraySize; ++i)
		{
			cJSON* jArrayItem = cJSON_GetArrayItem(jFilesArray, i);

			// Get filename of array item
			cJSON* jFilename = cJSON_GetObjectItem(jArrayItem, "filename");
			if (jFilename)
			{
				if (jFilename->valuestring != itemName)
				{
					continue;
				}

				// fill sprite with info
				SpriteFormat sprite;
				FindJSONSpriteInfo(sprite, jArrayItem);
				spriteVec.push_back(sprite);

				--itemsLeft;
			}

			// Break out of the loop if nothing else to find
			if (itemsLeft == 0)
				break;
		}
	}

	// Cleanup
	cJSON_Delete(jRoot);
	delete jsonString;

	return true;
}

void TilemapScene::FindJSONSpriteInfo(SpriteFormat& sprite, cJSON* jObject)
{
	// get width/height of found object
	cJSON* jWidth = cJSON_GetObjectItem(jObject, "origw");
	if (jWidth)
		sprite.uvSize.Set((float)(jWidth->valueint), sprite.uvSize.y);

	cJSON* jHeight = cJSON_GetObjectItem(jObject, "origh");
	if (jHeight)
		sprite.uvSize.Set(sprite.uvSize.x, (float)(jHeight->valueint));

	// get uv offset
	cJSON* jPosU = cJSON_GetObjectItem(jObject, "posx");
	if (jPosU)
		sprite.uvOffset.Set((float)(jPosU->valueint), sprite.uvOffset.y);

	cJSON* jPosV = cJSON_GetObjectItem(jObject, "posy");
	if (jPosV)
		sprite.uvOffset.Set(sprite.uvOffset.x, (float)(jPosV->valueint));
}

vec2 TilemapScene::GetSpriteSize(std::string filename)
{
	vec2 size(0.0f, 0.0f);

	// Load JSONFile
	// Data/JSON/Bomberman.json
	long jsonSLen = 0;
	char* jsonString = LoadCompleteFile(filename.c_str(), &jsonSLen);

	// Parse JSON file
	cJSON* jRoot = cJSON_Parse(jsonString);

	cJSON* jWidth = cJSON_GetObjectItem(jRoot, "width");
	if (jWidth)
		size.Set((float)(jWidth->valueint), size.y);

	cJSON* jHeight = cJSON_GetObjectItem(jRoot, "height");
	if (jHeight)
		size.Set(size.x, (float)(jHeight->valueint));

	// Cleanup
	cJSON_Delete(jRoot);
	delete jsonString;

	return size;
}

vec2 TilemapScene::CalculateMouseInWorldSpace(float x, float y)
{
	// Get mouse position in Window Space
	GameSample* game = m_sceneManager->GetGame();
	// Vector2Int mouseCoord = game->GetMouseCoordinates();
	vec2 mouseCoord(x, y);

	// Convert to Screen Space
	Point windowSizePoint = game->GetWindowSize();
	mouseCoord.y = (float)windowSizePoint.y - mouseCoord.y;

	// Convert to Clip Space
	vec2 clipPos;
	clipPos.x = (float)mouseCoord.x - (windowSizePoint.x / 2.0f);
	clipPos.y = (float)mouseCoord.y - (windowSizePoint.y / 2.0f);
	
	clipPos.x /= (float)windowSizePoint.x;
	clipPos.y /= (float)windowSizePoint.y;


	// Convert to Camera Space
	vec2 camPos = clipPos * m_pStaticCam->GetProjection() * 2.0f;

	// Convert to World Space
	vec2 worldPos = camPos + m_pStaticCam->GetPosition();

	return worldPos;
}

Enemy* TilemapScene::FindClosestEnemy(Tower* pTower)
{
	// Find enemies in range of the tower
	std::vector<Enemy*> enemiesInRange;
	float towerRange = pTower->GetSize();

	for (Enemy* e : m_pEnemies)
	{
		if (e->IsActive())
		{
			vec2 displacement = pTower->GetWorldPosition() - e->GetWorldPosition();

			if (displacement.LengthSquared() <= towerRange * towerRange)
			{
				enemiesInRange.push_back(e);
			}
		}
	}

	// Find the enemy that is closest to the end point
	Enemy* farthestEnemy = nullptr;
	float bestDistance = FLT_MAX;

	for (Enemy* e : enemiesInRange)
	{
		vec2 displacement = e->GetWorldPosition() - m_EndZone;
		if (displacement.LengthSquared() <= bestDistance * bestDistance)
		{
			bestDistance = displacement.Length();
			farthestEnemy = e;
		}
	}

	return farthestEnemy;
}

Enemy* TilemapScene::GetInactiveEnemy()
{
	for (Enemy* enemy : m_pEnemies)
	{
		if (!enemy->IsActive())
		{
			return enemy;
		}
	}

	return nullptr;
}

Bullet* TilemapScene::GetInactiveBullet()
{
	for (Bullet* bullet : m_pBullets)
	{
		if (!bullet->IsActive())
		{
			return bullet;
		}
	}

	return nullptr;
}

Tower* TilemapScene::GetInactiveTower()
{
	for (Tower* tower : m_pTowers)
	{
		if (!tower->IsActive())
		{
			return tower;
		}
	}

	return nullptr;
}

void TilemapScene::SpawnEnemy()
{
	Enemy* enemy = GetInactiveEnemy();

	if (enemy != nullptr)
	{
		vec2 spawnLocation = m_pCurrTilemap->GetWorldPosAtTile(m_StartZone);

		enemy->SetPosition(spawnLocation);
		enemy->Activate();
	}
}

// Spawn a tower at a given world position
void TilemapScene::SpawnTowerAtLocation(vec2 pos)
{
	// Check if you have enough points first
	int score = m_scoreDisp->GetScore();
	if (score >= TOWER_COST)
	{
		score -= TOWER_COST;
		m_scoreDisp->UpdateScore(score);
	}
	else
	{
		return;
	}

	vec2 spawnLocation = pos;

	// find an inactive tower
	Tower* pTower = GetInactiveTower();

	// Make sure it's valid
	if (pTower != nullptr)
	{
		pTower->SetPosition(pos);
		pTower->Activate();

		vec2 tilePos = m_pCurrTilemap->GetTileAtWorldPos(pos);
		m_pCurrTilemap->UpdateWalkability((int)m_pCurrTilemap->GetInverseY(tilePos.y), (int)tilePos.x, false);

		m_mouseIsOnValidLocation = false;
	}
}

void TilemapScene::UpdateMouseOverlay(float deltatime)
{
	Vector2Int inputCoord = m_sceneManager->GetGame()->GetMouseCoordinates();
	vec2 mousePos = CalculateMouseInWorldSpace((float)inputCoord.x, (float)inputCoord.y);
	vec2 tilePos = m_pCurrTilemap->GetTileAtWorldPos(mousePos);

	// check if we're still on the same mouse location as before
	if (tilePos == m_mouseTilePos)
	{
		if (m_mouseTimer < MOUSE_TIME)
		{
			return;
		}
	}
	else
	{
		m_mouseTimer = 0.0f;
	}

	m_mouseTilePos = tilePos;

	bool isValid = m_pCurrTilemap->IsValidPos((int)tilePos.y, (int)tilePos.x);
	if (isValid)
	{
		// Only update if it's somewhere on the tilemap
		m_mouseMeshPos.Set(tilePos.x * TILE_SIZE + TILE_SIZE / 2.0f, tilePos.y * TILE_SIZE + TILE_SIZE / 2.0f);

		// FIGURE OUT IF WE HAVE ENOUGH POINTS FIRST
		int score = m_scoreDisp->GetScore();
		if (score < TOWER_COST)
		{
			m_mouseIsOnValidLocation = false;
		}

		// FIGURE OUT IF IT'S A VALID SQUARE OR NOT
		// Don't let the player place a tower on the start or end locations
		else if (tilePos == m_StartZone || tilePos == m_EndZone)
		{
			m_mouseIsOnValidLocation = false;
		}
		
		// Tilepos is not the start or end location, check if that tile is walkable
		else
		{
			// Check if the tile is walkable and adjust color accordingly
			TileInfo* tile = m_pCurrTilemap->GetTileInfo((int)m_pCurrTilemap->GetInverseY(tilePos.y), (int)tilePos.x);
			if (tile != nullptr)
			{
				if (tile->m_Walkable)
				{
					m_mouseIsOnValidLocation = true;
				}
				else
				{
					m_mouseIsOnValidLocation = false;
				}
			}


			// Tile is walkable, check if it'll cause path blockage
			if (m_mouseTimer > MOUSE_TIME)
			{
				if (m_mouseIsOnValidLocation)
				{
					// Try blocking off the space where we will place a tower
					unsigned char tileType = m_pCurrTilemap->GetTileType((int)m_pCurrTilemap->GetInverseY(tilePos.y), (int)tilePos.x);
					m_pCurrTilemap->UpdateWalkability((int)m_pCurrTilemap->GetInverseY(tilePos.y), (int)tilePos.x, false);

					bool found = m_pPathfinder->FindPath((int)m_StartZone.x, (int)m_StartZone.y, (int)m_EndZone.x, (int)m_EndZone.y);

					// If a path was found then it's a valid location
					m_mouseIsOnValidLocation = found;

					m_pCurrTilemap->SetTileType((int)m_pCurrTilemap->GetInverseY(tilePos.y), (int)tilePos.x, tileType);
				}
			}
			else
			{
				m_mouseIsOnValidLocation = false;
			}
		}
	}
}

void TilemapScene::ChangeActiveTilemap(Tilemap* tilemap)
{
	m_pCurrTilemap = tilemap;
	m_StartZone = m_pCurrTilemap->GetStartPos();
	m_EndZone = m_pCurrTilemap->GetEndPos();

	// Re-Seed the pathfinder graph with new tilemap info
	Vector2Int mapSize = m_pCurrTilemap->GetMapSize();
	m_pPathfinder->SetAndPrimeTilemap(m_pCurrTilemap, mapSize.x, mapSize.y);

	// Deactivate all enemies
	DeactivateAllEnemies();

	// Deactivate all bullets
	DeactivateAllBullets();

	// Deactivate all towers
	DeactivateAllTowers();

	// Reload current tilemap
	m_pCurrTilemap->ReloadTilemap();


	// Score
	int score = m_scoreDisp->GetScore();
	if (score < INITIAL_SCORE)
	{
		m_scoreDisp->UpdateScore(INITIAL_SCORE);
	}
}

void TilemapScene::DeactivateAllEnemies()
{
	for (Enemy* enemy : m_pEnemies)
	{
		enemy->Deactivate();
	}
}

void TilemapScene::DeactivateAllBullets()
{
	for (Bullet* bullet : m_pBullets)
	{
		bullet->Deactivate();
	}
}

void TilemapScene::DeactivateAllTowers()
{
	for (Tower* tower : m_pTowers)
	{
		tower->Deactivate();
	}
}

vec2 TilemapScene::GetEndZone()
{
	return m_EndZone;
}

Tilemap* TilemapScene::GetCurrentTilemap()
{
	return m_pCurrTilemap;
}
Pathfinder* TilemapScene::GetPathfinder()
{
	return m_pPathfinder;
}

int TilemapScene::SaveGame()
{
	cJSON* jRoot = cJSON_CreateObject();

	cJSON* jTilemapScene = cJSON_CreateObject();
	cJSON_AddItemToObject(jRoot, "scene", jTilemapScene);

	cJSON_AddNumberToObject(jTilemapScene, "tilemap", m_currTilemapIndex);
	cJSON_AddNumberToObject(jTilemapScene, "score", m_scoreDisp->GetScore());

	// TOWER SAVING
	cJSON* jTowerArray = cJSON_CreateArray();
	cJSON_AddItemToObject(jTilemapScene, "TowerArray", jTowerArray);

	for (unsigned int i = 0; i < m_pTowers.size(); ++i)
	{
		Tower* currTower = m_pTowers[i];
		if (currTower->IsActive())
		{
			cJSON* jTower = cJSON_CreateObject();
			cJSON_AddItemToArray(jTowerArray, jTower);

			// Position
			vec2 tPos = currTower->GetPosition();
			cJSON_AddNumberToObject(jTower, "posx", tPos.x);
			cJSON_AddNumberToObject(jTower, "posy", tPos.y);

			// don't worry about shots or bullets
		}
	}

	// ENEMY SAVING
	cJSON* jEnemyArray = cJSON_CreateArray();
	cJSON_AddItemToObject(jTilemapScene, "EnemyArray", jEnemyArray);

	for (unsigned int i = 0; i < ENEMY_POOL_SIZE; ++i)
	{
		Enemy* currEnemy = m_pEnemies[i];
		if (currEnemy->IsActive())
		{
			cJSON* jEnemy = cJSON_CreateObject();
			cJSON_AddItemToArray(jEnemyArray, jEnemy);

			vec2 ePos = currEnemy->GetPosition();
			cJSON_AddNumberToObject(jEnemy, "posx", ePos.x);
			cJSON_AddNumberToObject(jEnemy, "posy", ePos.y);

			cJSON_AddNumberToObject(jEnemy, "health", currEnemy->GetHealth());
		}
	}


	char* jsonstr = cJSON_Print(jRoot);

	// quick write to a location
	FILE* filehandle;
	errno_t error = fopen_s(&filehandle, SAVE_FILE_LOC, "wb");

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

int TilemapScene::LoadGame()
{
	long fileSize = 0;
	char* fileStr = LoadCompleteFile(SAVE_FILE_LOC, &fileSize);

	if (fileSize == 0)
		return -1;

	cJSON* jRoot = cJSON_Parse(fileStr);

	// GAMEPLAY SCENE LOAD
	cJSON* jScene = cJSON_GetObjectItem(jRoot, "scene");

	cJSON* jTilemap = cJSON_GetObjectItem(jScene, "tilemap");
	if (jTilemap)
		m_currTilemapIndex = jTilemap->valueint;

	ChangeActiveTilemap(m_pTilemaps[m_currTilemapIndex]);


	cJSON* jScore = cJSON_GetObjectItem(jScene, "score");
	if (jScore)
		m_scoreDisp->UpdateScore(jScore->valueint);

	// TOWER POSITIONS
	cJSON* jTowerArray = cJSON_GetObjectItem(jScene, "TowerArray");
	int activeTowerCount = cJSON_GetArraySize(jTowerArray);

	for (int i = 0; i < activeTowerCount; ++i)
	{
		Tower* currTower = m_pTowers[i];

		cJSON* jTower = cJSON_GetArrayItem(jTowerArray, i);

		// Position
		vec2 towerPos;
		cJSON* jPosX = cJSON_GetObjectItem(jTower, "posx");
		cJSON* jPosY = cJSON_GetObjectItem(jTower, "posy");

		if (jPosX)
			towerPos.x = (float)jPosX->valuedouble;
		if (jPosY)
			towerPos.y = (float)jPosY->valuedouble;

		// Set all the values
		currTower->Activate();
		currTower->SetPosition(towerPos);

		// Calc tile loc of tower
		vec2 tilePos = m_pCurrTilemap->GetTileAtWorldPos(towerPos);
		m_pCurrTilemap->UpdateWalkability((int)m_pCurrTilemap->GetInverseY(tilePos.y), (int)tilePos.x, false);
	}

	// ENEMY POSITIONS
	cJSON* jEnemyArray = cJSON_GetObjectItem(jScene, "EnemyArray");

	int enemyCount = (unsigned int)cJSON_GetArraySize(jEnemyArray);

	// Set positions
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* currEnemy = m_pEnemies[i];
		cJSON* jEnemy = cJSON_GetArrayItem(jEnemyArray, (int)i);

		// Position
		vec2 enemyPos;
		cJSON* enemyX = cJSON_GetObjectItem(jEnemy, "posx");
		cJSON* enemyY = cJSON_GetObjectItem(jEnemy, "posy");

		if (enemyX)
			enemyPos.x = (float)enemyX->valuedouble;
		if (enemyY)
			enemyPos.y = (float)enemyY->valuedouble;

		// Health
		int health = 0;
		cJSON* jHealth = cJSON_GetObjectItem(jEnemy, "health");
		
		if (jHealth)
			health = jHealth->valueint;


		// Set Values
		currEnemy->SetPosition(enemyPos);
		currEnemy->Activate();
		currEnemy->SetHealth(health);
	}

	// Cleanup
	cJSON_Delete(jRoot);
	delete fileStr;

	return 0;
}