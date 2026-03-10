#include "ResourceRegistry.h"
#include "CoffeeEngine/Core/UUID.h"

namespace Coffee {

    std::unordered_map<UUID, Ref<Resource>> ResourceRegistry::m_Resources;
    std::unordered_map<std::string, UUID> ResourceRegistry::m_NameToUUID;

} // namespace Coffee