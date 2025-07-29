/**
 * @defgroup scene Scene
 * @{
 */

#pragma once

// Warning: This file should only be used when you want to include all components at once.
// If you want to include only specific components, use their individual headers instead.
// If you don't do it this way, you will end up increasing the compilation time unnecessarily.

// Include all individual component headers
#include "Components/TagComponent.h"
#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"
#include "Components/LightComponent.h"
#include "Components/AnimatorComponent.h"
#include "Components/MeshComponent.h"
#include "Components/MaterialComponent.h"
#include "Components/WorldEnvironmentComponent.h"
#include "Components/AudioSourceComponent.h"
#include "Components/AudioListenerComponent.h"
#include "Components/AudioZoneComponent.h"
#include "Components/ScriptComponent.h"
#include "Components/RigidbodyComponent.h"
#include "Components/NavMeshComponent.h"
#include "Components/NavigationAgentComponent.h"
#include "Components/ActiveComponent.h"
#include "Components/StaticComponent.h"
#include "Components/UIComponent.h"
#include "Components/UIImageComponent.h"
#include "Components/UITextComponent.h"
#include "Components/UIToggleComponent.h"
#include "Components/UIButtonComponent.h"
#include "Components/UISliderComponent.h"

/** @} */
