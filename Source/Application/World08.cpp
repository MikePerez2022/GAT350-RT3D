#include "World08.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>


namespace nc
{
    bool World08::Initialize()
    {
        //Load scene
        m_scene = std::make_unique<Scene>();
        m_scene->Load("Scenes/scene_editor.json");//load scene editor
        m_scene->Load("Scenes/scene_shadow_cel.json");
        m_scene->Initialize();

        m_editor = std::make_unique<Editor>();


        res_t<Texture> texture = std::make_unique<Texture>();
        texture->CreateDepthTexture(1024, 1024);
        ADD_RESOURCE("depth_texture", texture);

        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateDepthBuffer(texture);
        ADD_RESOURCE("depth_buffer", framebuffer);

        auto material = GET_RESOURCE(Material, "materials/sprite.mtrl");
        if (material)
        {
            material->albedoTexture = texture;
        }

        auto materials = GET_RESOURCES(Material);
        for (auto material : materials)
        {
            material->depthTexture = texture;
        }
        

        return true;
    }

    void World08::Shutdown()
    {
    }

    void World08::Update(float dt)
    {
        m_time += dt;

        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);

        m_editor->Update();
        
        m_editor->ProcessGui(m_scene.get());

        //ImGui to modify the cel variables in world class
        ImGui::Begin("Cel");
        ImGui::SliderInt("Levels", &m_celLevels, 1, 8);
        ImGui::SliderFloat("Specular Cutoff", &m_celSpecularCutoff, 0, 1);
        ImGui::SliderFloat("Outline", &m_celOutline, 0, 1);
        ImGui::End();

        auto program = GET_RESOURCE(Program, "shaders/lit_phong_cel.prog");
        if (program)
        {
            //Connect the variables to the cel shader uniforms
            program->Use();
            program->SetUniform("celLevels", m_celLevels);
            program->SetUniform("celSpecularCutoff", m_celSpecularCutoff);
            program->SetUniform("celOutline", m_celOutline);
        }


        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World08::Draw(Renderer& renderer)
    {
        //*** PASS 1 ***
        auto framebuffer = GET_RESOURCE(Framebuffer, "depth_buffer");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.ClearDepth();
        auto program = GET_RESOURCE(Program, "shaders/shadow_depth.prog");
        program->Use();

        auto lights = m_scene->GetComponents<LightComponent>();
        for (auto light : lights)
        {
            if (light->castShadow)
            {
                glm::mat4 shadowMatrix = light->GetShadowMatrix();
                program->SetUniform("shadowVP", shadowMatrix);
            }
        }

        auto models = m_scene->GetComponents<ModelComponent>();
        for (auto model : models)
        {
            if (!model->castShadow) continue;

            //glCullFace(GL_FRONT);
            program->SetUniform("model", model->m_owner->transform.GetMatrix());
            model->model->Draw();
        }

        m_scene->Draw(renderer);

        framebuffer->Unbind();
        //*** PASS 2 ***

        renderer.ResetViewport();
        renderer.BeginFrame();
        m_scene->Draw(renderer);


        ENGINE.GetSystem<Gui>()->Draw();
        renderer.EndFrame();
    }
}
