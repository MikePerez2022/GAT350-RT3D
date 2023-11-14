#include "World06.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>


namespace nc
{
    bool World06::Initialize()
    {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("Scenes/scene_framebuffer.json");
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

    void World06::Shutdown()
    {
    }

    void World06::Update(float dt)
    {
        m_time += dt;

        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
        m_scene->ProcessGui();

        //set postprocess
        ImGui::Begin("Postprocess");
        ImGui::SliderFloat("Blend", &m_blend, 0, 1);
        bool effect = m_params & INVERT_MASK;
        if (ImGui::Checkbox("Invert", &effect))
        {
            (effect) ? m_params |= INVERT_MASK : m_params &= INVERT_MASK;
        }

        effect = m_params & GREYSCALE_MASK;
        if (ImGui::Checkbox("Greyscale", &effect))
        {
            (effect) ? m_params |= GREYSCALE_MASK : m_params &= GREYSCALE_MASK;
        }

        effect = m_params & COLORTINT_MASK;
        if (ImGui::Checkbox("Color Tint", &effect))
        {
            (effect) ? m_params |= COLORTINT_MASK : m_params &= COLORTINT_MASK;
        }
        ImGui::ColorEdit3("Tint", glm::value_ptr(m_tint));

        effect = m_params & GRAIN_MASK;
        if (ImGui::Checkbox("Grain", &effect))
        {
            (effect) ? m_params |= GRAIN_MASK : m_params &= GRAIN_MASK;
        }

        effect = m_params & SCANLINE_MASK;
        if (ImGui::Checkbox("Scan lines", &effect))
        {
            (effect) ? m_params |= SCANLINE_MASK : m_params &= SCANLINE_MASK;
        }

        effect = m_params & EDGE_DETECTION;
        if (ImGui::Checkbox("Edge Detection", &effect))
        {
            (effect) ? m_params |= EDGE_DETECTION : m_params &= EDGE_DETECTION;
        }

        ImGui::End();

        //postprocess shader
        auto program = GET_RESOURCE(Program, "shaders/postprocess.prog");
        if (program)
        {
            program->Use();
            program->SetUniform("blend", m_blend);
            program->SetUniform("tint", m_tint);
            program->SetUniform("params", m_params);
        }

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World06::Draw(Renderer& renderer)
    {
        //*** PASS 1 ***
        m_scene->GetActorByName("postprocess")->active = false;

        auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();
        
        renderer.BeginFrame({0, 0, 1});
        m_scene->Draw(renderer);

        framebuffer->Unbind();

        //*** PASS 2 ***
        m_scene->GetActorByName("postprocess")->active = true;

        renderer.ResetViewport();
        renderer.BeginFrame();
        //m_scene->Draw(renderer);
        m_scene->GetActorByName("postprocess")->Draw(renderer);


        ENGINE.GetSystem<Gui>()->Draw();
        renderer.EndFrame();
    }
}
