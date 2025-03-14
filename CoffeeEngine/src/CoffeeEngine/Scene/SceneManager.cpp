#include "SceneManager.h"

namespace Coffee {

    Ref<Scene> SceneManager::m_ActiveScene = nullptr;
    std::filesystem::path SceneManager::s_WorkingDirectory = std::filesystem::current_path();


}