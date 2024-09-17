#include "ResourceRegistry.h"

namespace Tea {

    std::unordered_map<std::string, Ref<Resource>> ResourceRegistry::m_Resources;

} // namespace Tea