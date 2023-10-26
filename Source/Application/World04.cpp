#include "World04.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>


namespace nc
{
    bool World04::Initialize()
    {
        //auto material = GET_RESOURCE(Material, "Materials/multi.mtrl");
        //m_model = std::make_shared<Model>();
        //m_model->SetMaterial(material);
        //m_model->Load("Models/plane.obj");
        //m_transform.position.y = -1;
        //m_model->Load("Models/buddha.obj", glm::vec3{ 0 }, glm::vec3{ -90, 0, 0 });
        //m_model->Load("Models/dragon.obj", glm::vec3{ 0 }, glm::vec3{ 0, 0, 0 }, glm::vec3{0.1f, 0.1f, 0.1f});

        auto material = GET_RESOURCE(Material, "materials/squirrel.mtrl");
        m_model = std::make_shared<Model>();
        m_model->SetMaterial(material);
        m_model->Load("models/squirrel.glb", glm::vec3{ 0, -0.7f, 0 }, glm::vec3{ 0 }, glm::vec3{ 0.4f });

        for (int i = 0; i < 3; i++)
        {

            Light_t light;
            light.type = Light_t::etype::Point;
            light.position = glm::vec3{ randomf(-5, 5), randomf(1, 8), randomf(-5, 5)};
            light.direction = glm::vec3{ 0, -1, 0 };
            light.color = glm::rgbColor(glm::vec3{ randomf() * 360 , 1, 1 });
            light.intensity = 1;
            light.range = 12;
            light.innerAngle = 10.0f;
            light.outerAngle = 30.0f;


            m_lights.push_back(light);
        }
        

        return true;
    }

    void World04::Shutdown()
    {
    }

    void World04::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_transform.ProcessGui();

        ImGui::Begin("Scene");
        ImGui::ColorEdit3("Ambient Color", glm::value_ptr(ambientLight));
        ImGui::Separator();

        if (ImGui::Button("Add Light"))
        {
            //m_lights.push_back(Light_t{ Light_t::etype::Point, glm::vec3{0, 0, 0}, glm::vec3{0, -1, 0}, glm::vec3{1, 1, 1}, 1.0f, 10.0f, 10.0f, 30.0f});
        }

        if (ImGui::Button("Remove Light") && !m_lights.empty())
        {
            //m_lights.pop_back();
        }

        for (int i = 0; i < 3; i++)
        {
            std::string name = "light" + std::to_string(i);
            if (ImGui::Selectable(name.c_str(), m_selected == i)) m_selected = i;
        }


        ImGui::End();


        ImGui::Begin("Light");

        const char* lights[] = { "Light1", "Light2", "Light3"};
        ImGui::Combo("Lights", (int*)(&m_selected), lights, 3);

        const char* types[] = { "Point", "Directional", "Spot" };
        ImGui::Combo("Type", (int*)(&m_lights[m_selected].type), types, 3);

        if (m_lights[m_selected].type != Light_t::etype::Point) ImGui::DragFloat3("Direction", glm::value_ptr(m_lights[m_selected].direction), 0.1f);
        if (m_lights[m_selected].type != Light_t::etype::Directional) ImGui::DragFloat3("Position", glm::value_ptr(m_lights[m_selected].position), 0.1f);

        if (m_lights[m_selected].type == Light_t::etype::Spot)
        {
            ImGui::DragFloat("Inner Angle", &m_lights[m_selected].innerAngle, 1, 0, m_lights[m_selected].outerAngle);
            ImGui::DragFloat("Outer Angle", &m_lights[m_selected].outerAngle, 1, m_lights[m_selected].innerAngle, 90);
        }

        ImGui::ColorEdit3("Color", glm::value_ptr(m_lights[m_selected].color), (ImGuiColorEditFlags)0.1f);
        ImGui::DragFloat("Intensity", &m_lights[m_selected].intensity, 0.1f, 0, 10);

        if(m_lights[m_selected].type != Light_t::etype::Directional) ImGui::DragFloat("Range", &m_lights[m_selected].range, 0.1f, 0.1f, 50);

        //ImGui::ColorEdit3("Ambient color", glm::value_ptr(ambientLight), (ImGuiColorEditFlags)0.1f);
        ImGui::End();


        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * -dt : 0;
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed * dt : 0;

        m_transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * dt : 0;
        m_transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed * -dt : 0;

        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_O) ? m_speed * -dt : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_I) ? m_speed * dt : 0;

        m_time += dt;


        auto material = m_model->GetMaterial();
        material->ProcessGui();
        material->Bind();        

        //Model matrix
        material->GetProgram()->SetUniform("model", m_transform.GetMatrix());

        //View matrix
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 0, 3 }, glm::vec3{ 0,0,0 }, glm::vec3{ 0,1,0 });
        material->GetProgram()->SetUniform("view", view);

        //Projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), ENGINE.GetSystem<Renderer>()->GetWidth() / (float)ENGINE.GetSystem<Renderer>()->GetHeight(), 0.01f, 100.0f);
        material->GetProgram()->SetUniform("projection", projection);



        //light
        for (int i = 0; i < 3; i++)
        {
            std::string name = "lights[" + std::to_string(i) + "]";

            material->GetProgram()->SetUniform(name + ".position", m_lights[i].position);
            material->GetProgram()->SetUniform(name + ".color", m_lights[i].color);
            material->GetProgram()->SetUniform(name + ".intensity", m_lights[i].intensity);
            material->GetProgram()->SetUniform(name + ".range", m_lights[i].range);
            material->GetProgram()->SetUniform(name + ".type", m_lights[i].type);
            material->GetProgram()->SetUniform(name + ".direction", glm::normalize(m_lights[i].direction));
            material->GetProgram()->SetUniform(name + ".innerAngle", glm::radians(m_lights[i].innerAngle));
            material->GetProgram()->SetUniform(name + ".outerAngle", glm::radians(m_lights[i].outerAngle));
        }
        
        material->GetProgram()->SetUniform("ambientLight", ambientLight);

        


        

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World04::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_model->Draw(); 
        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();
    }
}
