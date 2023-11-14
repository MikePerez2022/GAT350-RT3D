#include "World07.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>


namespace nc
{
    bool World07::Initialize()
    {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("Scenes/scene_shadow.json");
        m_scene->Initialize();

        res_t<Texture> texture = std::make_unique<Texture>();
        texture->CreateTexture(1024, 1024);
        ADD_RESOURCE("fb_texture", texture);

        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateFramebuffer(texture);
        ADD_RESOURCE("fb", framebuffer);

        auto material = GET_RESOURCE(Material, "materials/postprocess.mtrl");
        if (material)
        {
            material->albedoTexture = texture;
        }
        

        return true;
    }

    void World07::Shutdown()
    {
    }

    void World07::Update(float dt)
    {
        m_time += dt;

        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
        m_scene->ProcessGui();

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World07::Draw(Renderer& renderer)
    {
        //*** PASS 1 ***
        //m_scene->GetActorByName("postprocess")->active = false;
        //auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
        //renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        //framebuffer->Bind();        
        //renderer.BeginFrame({0, 0, 0});
        //m_scene->Draw(renderer);
        //framebuffer->Unbind();
        //*** PASS 2 ***
        //m_scene->GetActorByName("postprocess")->active = true;

        renderer.ResetViewport();
        renderer.BeginFrame();
        m_scene->Draw(renderer);


        ENGINE.GetSystem<Gui>()->Draw();
        renderer.EndFrame();
    }
}
