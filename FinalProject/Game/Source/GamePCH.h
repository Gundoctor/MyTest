#ifndef __GamePCH_H__
#define __GamePCH_H__

#include "../../Libraries/Framework/Source/FrameworkPCH.h"

// Mesh
#include "Resource/MyResourceManager.h"

// Game
#include "Game/GameSample.h"

// Event
#include "Event/GameEvent.h"

// GameObject
#include "GameObject/Controllers/PlayerController.h"
#include "GameObject/Controllers/CameraController.h"
#include "GameObject/Actors/SimpleObject.h"
#include "GameObject/Actors/Healthbar.h"
#include "GameObject/Actors/Player.h"
#include "GameObject/Actors/Enemy.h"
#include "GameObject/Actors/Bullet.h"
#include "GameObject/Actors/Tower.h"
#include "GameObject/Cameras/ControllableCamera.h"
#include "GameObject/Cameras/StaticCamera.h"


// Scene
#include "Scene/TilemapScene.h"

// Tilemap
#include "Tilemap/Tilemap.h"

// Pathfinder
#include "Pathfinder/Pathfinder.h"

// SceneManager relies on GameSample for ShaderName enum
#include "Scene/SceneManager.h"

#endif //__GamePCH_H__
