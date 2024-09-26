#include "ResourceRegistry.h"

namespace Coffee {

    std::unordered_map<std::string, Ref<Resource>> ResourceRegistry::m_Resources;

} // namespace Coffee