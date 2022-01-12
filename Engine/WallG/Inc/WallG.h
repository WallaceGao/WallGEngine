#pragma once

#include "Common.h"

#include "App.h"
#include "AppState.h"

// Component headers
#include "AnimatorComponent.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include "ParticleComponent.h"

// Service headers
#include "CameraService.h"
#include "RenderService.h"
#include "SkyBoxService.h"

#include "Component.h"
#include "GameObject.h"
#include "GameObjectHandle.h"
#include "GameObjectIO.h"
#include "GameWorld.h"
#include "Service.h"

namespace WallG
{
	App& MainApp();
 }