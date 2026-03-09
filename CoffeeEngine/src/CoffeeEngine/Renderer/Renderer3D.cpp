#include "Renderer3D.h"
#include "CoffeeEngine/Renderer/Material.h"
#include "CoffeeEngine/Scene/PrimitiveMesh.h"
#include "CoffeeEngine/Renderer/Framebuffer.h"
#include "CoffeeEngine/Renderer/Mesh.h"
#include "CoffeeEngine/Renderer/Model.h"
#include "CoffeeEngine/Renderer/RendererAPI.h"
#include "CoffeeEngine/Renderer/Shader.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include "CoffeeEngine/Renderer/UniformBuffer.h"
#include "CoffeeEngine/Renderer/RenderTarget.h"
#include "CoffeeEngine/Renderer/VertexArray.h"
#include "CoffeeEngine/Animation/AnimationSystem.h"

#include "CoffeeEngine/Embedded/ToneMappingShader.inl"
#include "CoffeeEngine/Embedded/FinalPassShader.inl"
#include "CoffeeEngine/Embedded/MissingShader.inl"
#include "CoffeeEngine/Embedded/SimpleDepthShader.inl"
#include "CoffeeEngine/Embedded/BRDFLUTShader.inl"

#include <stdint.h>
#include <glm/fwd.hpp>
#include <glm/matrix.hpp>
#include <tracy/Tracy.hpp>

namespace Coffee {

    void Renderer3D::Init(RendererAPI* api)
    {
        ZoneScoped;

        m_API = api;
        
        m_RendererData.DefaultSkybox = Cubemap::Load("assets/textures/StandardCubeMap.hdr");
        m_CubeMesh = PrimitiveMesh::CreateCube({-1.0f, -1.0f, -1.0f});
        m_SkyboxShader = CreateRef<Shader>("assets/shaders/SkyboxShader.glsl");

        depthShader = CreateRef<Shader>("DepthShader", std::string(simpleDepthShaderSource));

        brdfShader = CreateRef<Shader>("BRDFLUTShader", std::string(BRDFLUTSource));

        // Shadow map
        TextureProperties shadowMapProperties;
        shadowMapProperties.srgb = false;
        shadowMapProperties.GenerateMipmaps = false;
        shadowMapProperties.Format = ImageFormat::DEPTH24STENCIL8;
        shadowMapProperties.Width = 4096;
        shadowMapProperties.Height = 4096;
        shadowMapProperties.Wrapping = TextureWrap::ClampToEdge;
        shadowMapProperties.BorderColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        m_RendererData.ShadowMapFramebuffer = Framebuffer::Create(4096, 4096);
        for (int i = 0; i < 4; i++)
        {
            m_RendererData.DirectionalShadowMapTextures[i] = Texture2D::Create(shadowMapProperties);
        }

        m_RendererData.SceneRenderDataUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::RenderData), 1);

        Ref<Shader> missingShader = CreateRef<Shader>("MissingShader", std::string(missingShaderSource));
        m_RendererData.DefaultMaterial = ShaderMaterial::Create("Missing Material", missingShader);

        // TODO: This is a hack to get the missing mesh add it to the PrimitiveMesh class
        Ref<Model> m = Model::Load("assets/models/MissingMesh.glb");
        m_RendererData.MissingMesh = m->GetMeshes()[0];

        m_ScreenQuad = PrimitiveMesh::CreateQuad();

        //m_FogShader = CreateRef<Shader>("FogShader", std::string(fogShaderSource));
        m_FogShader = CreateRef<Shader>("assets/shaders/FogShader.glsl");
        m_ToneMappingShader = CreateRef<Shader>("ToneMappingShader", std::string(toneMappingShaderSource));
        m_FXAAShader = CreateRef<Shader>("assets/shaders/FXAAShader.glsl"); // Shader source is too large
        m_FinalPassShader = CreateRef<Shader>("FinalPassShader", std::string(finalPassShaderSource));

        m_BloomShader = CreateRef<Shader>("assets/shaders/BloomShader.glsl");

        TextureProperties bloomTextureProperties;
        bloomTextureProperties.srgb = false;
        bloomTextureProperties.GenerateMipmaps = true;
        bloomTextureProperties.Format = ImageFormat::RGB16F; // Use a floating point format for bloom
        bloomTextureProperties.Width = 1280; // Initial size, will be resized later
        bloomTextureProperties.Height = 720; // Initial size, will be resized later
        bloomTextureProperties.Wrapping = TextureWrap::ClampToEdge;
        bloomTextureProperties.MinFilter = TextureFilter::LinearMipmapLinear;
        bloomTextureProperties.MagFilter = TextureFilter::Linear;

        m_BloomDownsampleTexture = Texture2D::Create(bloomTextureProperties);
        m_BloomUpsampleTexture = Texture2D::Create(bloomTextureProperties);
        
        m_BloomFramebuffer = Framebuffer::Create(1280, 720);
        GenerateBRDFLUT();
    }

    void Renderer3D::Shutdown()
    {
    }

    void Renderer3D::Submit(const LightComponent& light)
    {
        const int maxLights = Renderer3DData::MAX_LIGHTS - (light.type == LightComponent::Type::DirectionalLight ? 0 : 4);
        if (m_RendererData.RenderData.lightCount < maxLights)
        {
            m_RendererData.RenderData.lights[m_RendererData.RenderData.lightCount] = light;
            m_RendererData.RenderData.lightCount++;
        }
    }

    void Renderer3D::Submit(const RenderCommand& command)
    {
        if (!command.material)
        {
            m_RendererData.opaqueRenderQueue.push_back(command);
            return;
        }

        const auto& settings = command.material->GetRenderSettings();

        if (settings.transparencyMode == MaterialRenderSettings::TransparencyMode::Disabled)
        {
            m_RendererData.opaqueRenderQueue.push_back(command);
        }
        else
        {
            m_RendererData.transparentRenderQueue.push_back(command);
        }
    }

    // Temporal, this should be removed because this is rendering immediately.
    void Renderer3D::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform, uint32_t entityID)
    {
        shader->Bind();
        shader->setMat4("model", transform);
        shader->setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(transform))));

        //REMOVE: This is for the first release of the engine it should be handled differently
        shader->setBool("showNormals", m_RenderSettings.showNormals);

        // Convert entityID to vec3
        uint32_t r = (entityID & 0x000000FF) >> 0;
        uint32_t g = (entityID & 0x0000FF00) >> 8;
        uint32_t b = (entityID & 0x00FF0000) >> 16;
        glm::vec3 entityIDVec3 = glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f);

        shader->setVec3("entityID", entityIDVec3);

        m_API->DrawIndexed(vertexArray);

        m_Stats.DrawCalls++;
    }

    void Renderer3D::DepthPrePass(const Ref<RenderTarget>& target)
    {
        ZoneScoped;

        const Ref<Framebuffer>& forwardBuffer = target->GetFramebuffer("Forward");
        forwardBuffer->Bind();


    }

    void Renderer3D::ShadowPass(const Ref<RenderTarget>& target)
    {
        ZoneScoped;

        int directionalLightCount = 0;
    
        for (int i = 0; i < m_RendererData.RenderData.lightCount; ++i)
        {
            const auto& light = m_RendererData.RenderData.lights[i];
    
            // Check if the light is directional
            if (light.type == LightComponent::Type::DirectionalLight and light.Shadow)
            {
                auto& shadowMap = m_RendererData.DirectionalShadowMapTextures[directionalLightCount];
                m_RendererData.ShadowMapFramebuffer->AttachDepthTexture(shadowMap);
    
                m_RendererData.ShadowMapFramebuffer->Bind();
    
                m_API->SetViewport(0, 0, 4096, 4096);
                m_API->Clear();

                // Calculate light position based on camera and scene bounds
                glm::vec3 cameraPos = target->GetCameraTransform()[3];
                float shadowDistance = light.ShadowMaxDistance;
                
                // Position the light to cover the camera's view frustum
                glm::vec3 lightPos = cameraPos - light.Direction * (shadowDistance * 0.5f);

                // Adjust the orthographic projection bounds based on ShadowMaxDistance
                float orthoBounds = shadowDistance * 0.5f; // Slightly larger to avoid edge artifacts
                float nearPlane = 0.1f;
                float farPlane = shadowDistance;

                glm::mat4 lightProjection = glm::ortho(
                    -orthoBounds, orthoBounds, // left, right
                    -orthoBounds, orthoBounds, // bottom, top
                    nearPlane, farPlane        // near, far
                );

                glm::mat4 lightView = glm::lookAt(
                    lightPos,
                    lightPos + light.Direction,
                    glm::vec3(0.0f, 1.0f, 0.0f)
                );

                glm::mat4 lightSpaceMatrix = lightProjection * lightView;

                // Store the light space matrix for use in forward pass
                m_RendererData.RenderData.LightSpaceMatrices[directionalLightCount] = lightSpaceMatrix;

                depthShader->Bind();
                depthShader->setMat4("projView", lightSpaceMatrix);

                m_API->SetCullFace(CullFace::Front);
    
                for (const auto& command : m_RendererData.opaqueRenderQueue)
                {
                    if (command.animator)
                        AnimationSystem::SetBoneTransformations(depthShader, command.animator);
                    else
                        depthShader->setBool("animated", false);

                    // Set the model matrix
                    depthShader->setMat4("model", command.transform);

                    Mesh* mesh = command.mesh.get();
                    
                    if(mesh == nullptr)
                    {
                        mesh = m_RendererData.MissingMesh.get();
                    }
                    
                    m_API->DrawIndexed(mesh->GetVertexArray());
                }

                m_API->SetCullFace(CullFace::Back);
    
                m_RendererData.ShadowMapFramebuffer->UnBind();
    
                directionalLightCount++;
    
                // Stop after processing the first 4 directional lights
                if (directionalLightCount >= Renderer3DData::MAX_DIRECTIONAL_SHADOWS)
                    break;
            }
        }

        // Update the uniform buffer with the light data
        m_RendererData.SceneRenderDataUniformBuffer->SetData(&m_RendererData.RenderData, sizeof(Renderer3DData::RenderData));
    }

    void Renderer3D::ForwardPass(const Ref<RenderTarget>& target)
    {
        ZoneScoped;

        const Ref<Framebuffer>& forwardBuffer = target->GetFramebuffer("Forward");

        forwardBuffer->Bind();
        forwardBuffer->SetDrawBuffers({0, 1}); //TODO: This should only be done in the editor

        m_API->SetClearColor({0.03f,0.03f,0.03f,1.0});
        m_API->Clear();
        
        forwardBuffer->GetColorAttachment(1)->Clear({-1.0f,0.0f,0.0f,0.0f}); //TODO: This should only be done in the editor

        if (!m_RendererData.EnvironmentMap)
        {
            m_RendererData.EnvironmentMap = m_RendererData.DefaultSkybox;
        }

        // Bind the irradiance map
        m_RendererData.EnvironmentMap->BindIrradianceMap(6);
        m_RendererData.EnvironmentMap->BindPrefilteredMap(7);
        
        // Bind the BRDF LUT
        m_RendererData.BRDFLUT->Bind(8);

        // Set shadow map textures
        for (int i = 0; i < Renderer3DData::MAX_DIRECTIONAL_SHADOWS; ++i)
        {
            m_RendererData.DirectionalShadowMapTextures[i]->Bind(9 + i);
        }

        // Sort the render queue based on material and mesh
        std::sort(m_RendererData.opaqueRenderQueue.begin(), m_RendererData.opaqueRenderQueue.end(), [](const RenderCommand& a, const RenderCommand& b) {
            return std::tie(a.material, a.mesh) < std::tie(b.material, b.mesh);
        });

        for(const auto& command : m_RendererData.opaqueRenderQueue)
        {
            Material* material = command.material.get();

            if(material == nullptr or material->GetShader() == nullptr)
            {
                material = m_RendererData.DefaultMaterial.get();
            }
            
            material->Use();

            const Ref<Shader>& shader = material->GetShader();

            shader->Bind();

            // Set the irradiance map, is 6 because the first 6 slots are used by the material
            shader->setInt("irradianceMap", 6);
            shader->setInt("prefilterMap", 7);
            shader->setInt("brdfLUT", 8);

            // Set shadow map textures
            for (int i = 0; i < Renderer3DData::MAX_DIRECTIONAL_SHADOWS; ++i)
            {
                shader->setInt("shadowMaps[" + std::to_string(i) + "]", 9 + i);
            }

            if (command.animator)
                AnimationSystem::SetBoneTransformations(shader, command.animator);
            else
                shader->setBool("animated", false);

            shader->setMat4("model", command.transform);
            shader->setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(command.transform))));

            //REMOVE: This is for the first release of the engine it should be handled differently
            shader->setBool("showNormals", m_RenderSettings.showNormals);

            // Convert entityID to vec3
            uint32_t r = (command.entityID & 0x000000FF) >> 0;
            uint32_t g = (command.entityID & 0x0000FF00) >> 8;
            uint32_t b = (command.entityID & 0x00FF0000) >> 16;
            glm::vec3 entityIDVec3 = glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f);

            shader->setVec3("entityID", entityIDVec3);

            Mesh* mesh = command.mesh.get();
            
            if(mesh == nullptr)
            {
                mesh = m_RendererData.MissingMesh.get();
            }

            // Apply material settings
            const MaterialRenderSettings& settings = material->GetRenderSettings();
            switch (settings.cullMode)
            {
                case MaterialRenderSettings::CullMode::Front:
                    m_API->SetCullFace(CullFace::Front);
                    break;
                case MaterialRenderSettings::CullMode::Back:
                    m_API->SetCullFace(CullFace::Back);
                    break;
                case MaterialRenderSettings::CullMode::None:
                    m_API->SetFaceCulling(false);
                    break;
            }

            if (settings.depthTest)
            {
                m_API->SetDepthMask(true);
            }
            else
            {
                m_API->SetDepthMask(false);
            }

            if (settings.wireframe)
            {
                m_API->SetPolygonMode(PolygonMode::Line);
            }
            else
            {
                m_API->SetPolygonMode(PolygonMode::Fill);
            }

            m_API->DrawIndexed(mesh->GetVertexArray());

            
            m_Stats.DrawCalls++;

            m_Stats.VertexCount += mesh->GetVertices().size();
            m_Stats.IndexCount += mesh->GetIndices().size();
        }

        forwardBuffer->UnBind();

        // Reset render settings to default
        m_API->SetCullFace(CullFace::Back);
        m_API->SetFaceCulling(true);
        m_API->SetDepthMask(true);
        m_API->SetPolygonMode(PolygonMode::Fill);
    }

    void Renderer3D::SkyboxPass(const Ref<RenderTarget>& target)
    {
        ZoneScoped;

        // TODO: Think if this should be done here another time
        const Ref<Framebuffer>& forwardBuffer = target->GetFramebuffer("Forward");

        forwardBuffer->Bind();
        forwardBuffer->SetDrawBuffers({0, 1});

        m_API->SetDepthMask(false);
        m_RendererData.EnvironmentMap->Bind(0);
        m_SkyboxShader->Bind();
        m_SkyboxShader->setInt("skybox", 0);
        m_SkyboxShader->setFloat("exposure", m_RenderSettings.EnvironmentExposure);
        m_API->DrawIndexed(m_CubeMesh->GetVertexArray());
        m_API->SetDepthMask(true);

        forwardBuffer->UnBind();
    }

    void Renderer3D::TransparentPass(const Ref<RenderTarget>& target)
    {
        ZoneScoped;

        const Ref<Framebuffer>& forwardBuffer = target->GetFramebuffer("Forward");
        forwardBuffer->Bind();
        forwardBuffer->SetDrawBuffers({0, 1}); //TODO: This should only be done in the editor

        // Bind the irradiance map
        m_RendererData.EnvironmentMap->BindIrradianceMap(6);
        m_RendererData.EnvironmentMap->BindPrefilteredMap(7);

        // Bind the BRDF LUT
        m_RendererData.BRDFLUT->Bind(8);

        // Render transparent objects (back to front)
        glm::vec3 cameraPos = target->GetCameraTransform()[3];
        std::sort(m_RendererData.transparentRenderQueue.begin(), m_RendererData.transparentRenderQueue.end(), [&cameraPos](const RenderCommand& a, const RenderCommand& b) {
            float distA = glm::length(cameraPos - glm::vec3(a.transform[3]));
            float distB = glm::length(cameraPos - glm::vec3(b.transform[3]));
            return distA > distB;
        });

        m_API->SetDepthMask(false);

        for (const auto& command : m_RendererData.transparentRenderQueue)
        {
            Material* material = command.material.get();

            if(material == nullptr)
            {
                material = m_RendererData.DefaultMaterial.get();
            }

            material->Use();

            const Ref<Shader>& shader = material->GetShader();

            shader->Bind();

            // Set the irradiance map, is 6 because the first 6 slots are used by the material
            shader->setInt("irradianceMap", 6);
            shader->setInt("prefilterMap", 7);
            shader->setInt("brdfLUT", 8);

            if (command.animator)
                AnimationSystem::SetBoneTransformations(shader, command.animator);
            else
                shader->setBool("animated", false);

            shader->setMat4("model", command.transform);
            shader->setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(command.transform))));

            //REMOVE: This is for the first release of the engine it should be handled differently
            shader->setBool("showNormals", m_RenderSettings.showNormals);

            // Convert entityID to vec3
            uint32_t r = (command.entityID & 0x000000FF) >> 0;
            uint32_t g = (command.entityID & 0x0000FF00) >> 8;
            uint32_t b = (command.entityID & 0x00FF0000) >> 16;
            glm::vec3 entityIDVec3 = glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f);

            shader->setVec3("entityID", entityIDVec3);

            Mesh* mesh = command.mesh.get();
            
            if(mesh == nullptr)
            {
                mesh = m_RendererData.MissingMesh.get();
            }

                // Apply material settings
                const MaterialRenderSettings& settings = material->GetRenderSettings();
                switch (settings.cullMode)
                {
                    case MaterialRenderSettings::CullMode::Front:
                        m_API->SetCullFace(CullFace::Front);
                        break;
                    case MaterialRenderSettings::CullMode::Back:
                        m_API->SetCullFace(CullFace::Back);
                        break;
                    case MaterialRenderSettings::CullMode::None:
                        m_API->SetFaceCulling(false);
                        break;
                }
    
                if (settings.wireframe)
                {
                    m_API->SetPolygonMode(PolygonMode::Line);
                }
                else
                {
                    m_API->SetPolygonMode(PolygonMode::Fill);
                }

            m_API->DrawIndexed(mesh->GetVertexArray());
        }

        m_API->SetDepthMask(true);

        forwardBuffer->UnBind();

        // Reset render settings to default
        m_API->SetCullFace(CullFace::Back);
        m_API->SetFaceCulling(true);
        m_API->SetDepthMask(true);
        m_API->SetPolygonMode(PolygonMode::Fill);
    }

    void Renderer3D::PostProcessingPass(const Ref<RenderTarget>& target)
    {
        ZoneScoped;

        //Render All the fancy effects :D
        const Ref<Framebuffer>& forwardBuffer = target->GetFramebuffer("Forward");
        
        Ref<Framebuffer> lastBuffer = target->GetFramebuffer("PostProcessingA");
        Ref<Framebuffer> postBuffer = target->GetFramebuffer("PostProcessingB");

        // Copy the forward buffer to the last buffer (think if is necessary)
        lastBuffer->Bind();
        m_FinalPassShader->Bind();
        m_FinalPassShader->setInt("screenTexture", 0);
        forwardBuffer->GetColorAttachment(0)->Bind(0);

        m_API->DrawIndexed(m_ScreenQuad->GetVertexArray());
        m_FinalPassShader->Unbind();
        lastBuffer->UnBind();

        std::swap(lastBuffer, postBuffer);

        // Depth Fog (Is possible that some uniforms are not needed)
        if (m_RenderSettings.DepthFog)
        {
            lastBuffer->Bind();
            m_FogShader->Bind();
            m_FogShader->setBool("DepthFog", m_RenderSettings.DepthFog);
            m_FogShader->setVec3("FogColor", m_RenderSettings.FogColor);
            m_FogShader->setFloat("FogDensity", m_RenderSettings.FogDensity);
            m_FogShader->setFloat("FogHeight", m_RenderSettings.FogHeight);
            m_FogShader->setFloat("FogHeightDensity", m_RenderSettings.FogHeightDensity);
            m_FogShader->setMat4("invProjection", glm::inverse(target->GetCamera().GetProjection()));
            m_FogShader->setMat4("invView", target->GetCameraTransform());
            m_FogShader->setInt("colorTexture", 0);
            m_FogShader->setInt("depthTexture", 1);
            postBuffer->GetColorAttachment(0)->Bind(0);
            forwardBuffer->GetDepthTexture()->Bind(1);

            m_API->DrawIndexed(m_ScreenQuad->GetVertexArray());
            m_FogShader->Unbind();
            lastBuffer->UnBind();

            std::swap(lastBuffer, postBuffer);
        }

        // Bloom
        if (m_RenderSettings.Bloom)
        {
            // Bind the framebuffer for downsampling with a color texture
            // Iterate over all the downsampling passes, binding each mip as output and the previous mip as input
            // In other framebuffer (output), we set as input the texture of the downsampling pass and

            glm::vec2 currentTargetSize = target->GetSize()/*  * 0.75f */;

            // TODO: Do it only when the resolution changes not every frame
            //m_BloomFramebuffer->Resize(currentTargetSize.x, currentTargetSize.y);
            static glm::vec2 lastTargetSize = {0, 0};
            if (currentTargetSize != lastTargetSize)
            {
                lastTargetSize = currentTargetSize;
                m_BloomDownsampleTexture->Resize(currentTargetSize.x, currentTargetSize.y);
                m_BloomUpsampleTexture->Resize(currentTargetSize.x, currentTargetSize.y);
            }

            m_BloomShader->Bind();
            m_BloomShader->setInt("sourceTexture", 0);
            postBuffer->GetColorAttachment(0)->Bind(0); // Bind the post-processing texture as the source
            m_BloomShader->setInt("downsamplingTexture", 1);
            m_BloomDownsampleTexture->Bind(1); // Bind the downsample texture to texture unit 1
            m_BloomShader->setInt("upsamplingTexture", 2);
            m_BloomUpsampleTexture->Bind(2); // Bind the upsample texture to texture unit 2

            // Copy the scene texture to the bloom downsample texture
            m_BloomShader->setInt("mode", 0); // 0 for copy
            m_BloomShader->setInt("mipmapLevel", 0); // Use mip level 0 for the initial copy
            m_BloomShader->setFloat("bloomIntensity", m_RenderSettings.BloomIntensity);

            m_BloomFramebuffer->AttachColorTexture(0, m_BloomDownsampleTexture, 0);
            m_BloomFramebuffer->Bind();
            m_BloomFramebuffer->SetDrawBuffers({0});
            m_API->SetViewport(0, 0, currentTargetSize.x, currentTargetSize.y);
            m_API->Clear();
            
            m_API->DrawIndexed(m_ScreenQuad->GetVertexArray());

            // Downsampling Passes
            int maxMipLevel = m_RenderSettings.BloomMaxMipLevels; // Number of downsampling passes
            for (int mip = 1; mip < maxMipLevel; mip++)
            {
                // Resize the bloom downsample texture for the current mip level
                uint32_t mipWidth = static_cast<uint32_t>(currentTargetSize.x) >> mip;
                uint32_t mipHeight = static_cast<uint32_t>(currentTargetSize.y) >> mip;

                // Attach the current mip level to the framebuffer
                m_BloomFramebuffer->AttachColorTexture(0, m_BloomDownsampleTexture, mip);
                m_BloomFramebuffer->Bind();

                m_API->SetViewport(0, 0, mipWidth, mipHeight);
                m_API->Clear();

                // Set the shader for downsampling
                m_BloomShader->setInt("mode", 1); // 1 for downsampling
                m_BloomShader->setInt("mipmapLevel", mip);

                m_API->DrawIndexed(m_ScreenQuad->GetVertexArray());

                m_BloomFramebuffer->UnBind();
            }

            // Copy the last downsampled texture to the upsample texture
            m_BloomFramebuffer->AttachColorTexture(0, m_BloomUpsampleTexture, maxMipLevel - 1);
            m_BloomFramebuffer->Bind();
            uint32_t mipWidth = static_cast<uint32_t>(currentTargetSize.x) >> (maxMipLevel - 1);
            uint32_t mipHeight = static_cast<uint32_t>(currentTargetSize.y) >> (maxMipLevel - 1);
            m_API->SetViewport(0, 0, mipWidth, mipHeight);
            m_API->Clear();
            m_BloomShader->setInt("mode", 0); // 0 for copy
            m_BloomShader->setInt("mipmapLevel", maxMipLevel - 1); // Use the last downsampled mip level
            m_BloomDownsampleTexture->Bind(0); // Bind the last downsampled texture to texture unit 0

            m_API->DrawIndexed(m_ScreenQuad->GetVertexArray());

            m_BloomShader->setFloat("filterRadius", m_RenderSettings.BloomRadius); // Set a filter radius for the bloom effect

            //m_API->SetBlendFunc(BlendFunc::One, BlendFunc::One);

            // Upsampling Passes
            for (int mip = maxMipLevel - 2; mip >= 0; --mip)
            {
                // Resize the bloom upsample texture for the current mip level
                uint32_t mipWidth = static_cast<uint32_t>(currentTargetSize.x) >> mip;
                uint32_t mipHeight = static_cast<uint32_t>(currentTargetSize.y) >> mip;

                // Attach the current mip level to the framebuffer
                m_BloomFramebuffer->AttachColorTexture(0, m_BloomUpsampleTexture, mip);
                m_BloomFramebuffer->Bind();

                m_API->SetViewport(0, 0, mipWidth, mipHeight);
                m_API->Clear();

                // Set the shader for upsampling
                m_BloomShader->setInt("mode", 2); // 2 for upsampling
                m_BloomShader->setInt("mipmapLevel", mip);

                m_API->DrawIndexed(m_ScreenQuad->GetVertexArray());

                m_BloomFramebuffer->UnBind();
            }

            //m_API->SetBlendFunc(BlendFunc::SrcAlpha, BlendFunc::OneMinusSrcAlpha);

            // Final Composition Pass
            lastBuffer->Bind();
            m_BloomShader->setInt("mode", 3); // 3 for final composition

            m_API->SetViewport(0, 0, static_cast<uint32_t>(target->GetSize().x), static_cast<uint32_t>(target->GetSize().y));
            m_API->Clear();

            m_API->DrawIndexed(m_ScreenQuad->GetVertexArray());

            m_BloomShader->Unbind();
            m_BloomFramebuffer->UnBind();
            lastBuffer->UnBind();
            std::swap(lastBuffer, postBuffer);
        }

        //ToneMapping
        lastBuffer->Bind();
        m_ToneMappingShader->Bind();
        m_ToneMappingShader->setInt("screenTexture", 0);
        m_ToneMappingShader->setFloat("exposure", m_RenderSettings.Exposure);
        postBuffer->GetColorAttachment(0)->Bind(0);

        m_API->DrawIndexed(m_ScreenQuad->GetVertexArray());

        m_ToneMappingShader->Unbind();
        lastBuffer->UnBind();

        std::swap(lastBuffer, postBuffer);

        // TODO better logic for dynamically enabling and disabling individual post-processing effects
        // Fast aproXimate AntiAliasing
        if (m_RenderSettings.FXAA)
        {

            lastBuffer->Bind();
            m_FXAAShader->Bind();
            m_FXAAShader->setInt("screenTexture", 0);
            m_FXAAShader->setVec2("screenSize", {forwardBuffer->GetWidth(), forwardBuffer->GetHeight()});
            postBuffer->GetColorAttachment(0)->Bind(0);

            m_API->DrawIndexed(m_ScreenQuad->GetVertexArray());

            m_FXAAShader->Unbind();
            lastBuffer->UnBind();

            std::swap(lastBuffer, postBuffer);
        }

        // Final pass to copy the post-processing texture to the main render texture

        //This has to be set because the m_ScreenQuad overwrites the depth buffer
        m_API->SetDepthMask(false);

        // Copy PostProcessing Texture to the Main Render Texture
        forwardBuffer->Bind();
        forwardBuffer->SetDrawBuffers({0});

        m_FinalPassShader->Bind();
        m_FinalPassShader->setInt("screenTexture", 0);
        postBuffer->GetColorAttachment(0)->Bind(0);
        //m_BloomDownsampleTexture->Bind(0);
        //m_BloomUpsampleTexture->Bind(0); // Use the downsampled texture for final pass

        m_API->DrawIndexed(m_ScreenQuad->GetVertexArray());

        m_FinalPassShader->Unbind();

        m_API->SetDepthMask(true);

        forwardBuffer->UnBind();
    }

    void Renderer3D::ResetCalls()
    {
        m_RendererData.RenderData.lightCount = 0;
        m_RendererData.opaqueRenderQueue.clear();
        m_RendererData.transparentRenderQueue.clear();

        m_RendererData.EnvironmentMap = nullptr;
    }

    void Renderer3D::GenerateBRDFLUT()
    {
        TextureProperties properties;
        properties.Format = ImageFormat::RGBA16F;
        properties.Width = 512;
        properties.Height = 512;
        properties.GenerateMipmaps = false;
        properties.Wrapping = TextureWrap::ClampToEdge;
        properties.MinFilter = TextureFilter::Linear;
        properties.MagFilter = TextureFilter::Linear;

        m_RendererData.BRDFLUT = Texture2D::Create(properties);
        
        Framebuffer framebuffer = Framebuffer(properties.Width, properties.Height);
        framebuffer.AttachColorTexture(0, m_RendererData.BRDFLUT);
        framebuffer.Bind();
        framebuffer.SetDrawBuffers({0});

        m_API->SetViewport(0, 0, properties.Width, properties.Height);

        brdfShader->Bind();

        m_API->SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
        m_API->Clear();

        m_ScreenQuad->GetVertexArray()->Bind();
        m_API->DrawIndexed(m_ScreenQuad->GetVertexArray());

        framebuffer.UnBind();
    }
}