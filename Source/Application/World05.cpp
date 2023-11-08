#include "World05.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>


namespace nc
{
    bool World05::Initialize()
    {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("Scenes/scene.json");
        m_scene->Initialize();

        //tree generation
        for (int i = 0; i < 20; i++)
        {
            auto actor = CREATE_CLASS_BASE(Actor, "tree");
            actor->transform.position = glm::vec3{ randomf(-10,10), 0, randomf(-10,10) };
            actor->transform.scale = glm::vec3(randomf(0.5f, 3.0f), randomf(0.5f, 3.0f), randomf(0.5f, 3.0f));
            actor->Initialize();
            m_scene->Add(std::move(actor));
        }


        return true;
    }

    void World05::Shutdown()
    {
    }

    void World05::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
        m_scene->ProcessGui();

        auto actor = m_scene->GetActorByName<Actor>("actor1");
        
        m_time += dt;

        
        auto material = actor->GetComponent<ModelComponent>()->material;
        material->ProcessGui();
        material->Bind();

        material = GET_RESOURCE(Material, "materials/refraction.mtrl");
        if (material)
        {
            ImGui::Begin("Refraction");

            //m_refraction = 1.0f + std::fabs();

            ImGui::DragFloat("IOR", &m_refraction, 0.01f, 1.00f, 3);
            auto program = material->GetProgram();
            program->Use();
            program->SetUniform("ior", m_refraction);


            ImGui::End();
        }

        //light        
        material->GetProgram()->SetUniform("ambientLight", ambientLight);        

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World05::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_scene->Draw(renderer); 
        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();
    }
}
