#include <unordered_map>
#include <string>
#include "Texture.h"
#include <memory>

//this makes that the texture isn't loaded multiple times
namespace Coffee {
    class TextureCache {
    private:
        std::unordered_map<std::string, Ref<Texture2D>> m_TextureCache;

    public:
        static TextureCache& GetInstance() {
            static TextureCache instance;
            return instance;
        }

        Ref<Texture2D> LoadTexture(const std::filesystem::path& path, bool srgb) {
            std::string key = path.string() + (srgb ? "_srgb" : "_linear");

            // Verificar si la textura ya está en el caché
            if (m_TextureCache.find(key) != m_TextureCache.end()) {
                return m_TextureCache[key];
            }

            // Cargar la textura desde el disco
            COFFEE_INFO("Loading texture {} from disk.", path.string());
            Ref<Texture2D> texture = CreateRef<Texture2D>(path, srgb);

            // Almacenar la textura en el caché
            if (texture) {
                m_TextureCache[key] = texture;
            }

            return texture;
        }

        void ClearCache() {
            m_TextureCache.clear();
        }
    };
}