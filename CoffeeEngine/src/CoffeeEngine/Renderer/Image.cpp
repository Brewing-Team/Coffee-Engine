// #include "CoffeeEngine/Renderer/Image.h"
// #include "CoffeeEngine/Core/Base.h"
//
// #include <stb_image.h>
//
// namespace Coffee {
//
//     Image::Image(std::string& path)
//     {
//         m_FilePath = path;
//
//         int nrComponents;
//         m_Data = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &nrComponents, 0);
//
//         switch (nrComponents)
//         {
//             case 1:
//                 m_Channels = ImageChannels::R;
//             break;
//             case 3:
//                 m_Channels = ImageChannels::RGB;
//             break;
//             case 4:
//                 m_Channels = ImageChannels::RGBA;
//             break;<
//         }
//     }
//
//     static Ref<Image> Load(std::string& path)
//     {
//         return CreateRef<Image>(path);
//     }
//
// }
