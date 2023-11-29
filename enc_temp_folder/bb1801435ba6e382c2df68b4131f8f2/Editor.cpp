#include "Editor.h"
#include "Framework/Scene.h"
#include "Framework/Components/CameraComponent.h"

#include "Texture.h"
#include "Model.h"
#include "Material.h"
#include "Shader.h"
//#include <vector>

namespace nc
{
	void Editor::Update()
	{
		if (ImGui::IsKeyPressed(ImGuiKey_GraveAccent)) m_active = !m_active;
	}

	void Editor::ProcessGui(Scene* scene)
	{
		if (!m_active) return;

		ImGui::Begin("Resources");

		const char* types[] = { "Texture", "Model", "Material", "Shader" };
		ImGui::Combo("Resource Filter", (int*)(&resourceType), types, 4);

		auto res = GET_RESOURCES(Texture); 
		

				
		auto resources = res;
		for (auto& resource : resources)
		{
			auto hh = resource.get();

			if (ImGui::Selectable(resource->name.c_str(), resource.get() == m_selected))
			{
				m_selected = resource.get();
			}
		}
		ImGui::End();

		//Show scene
		ImGui::Begin("Scene");
		scene->ProcessGui();
		ImGui::Separator();
		//Show Acors

		ImGui::BeginChild("Actors");
		//
		if (ImGui::BeginPopupContextWindow())
		{
			auto cameras = scene->GetComponents<CameraComponent>();
			auto camera = (!cameras.empty()) ? cameras[0] : nullptr;

			if (ImGui::MenuItem("Create Empty")) 
			{
				auto actor = CREATE_CLASS(Actor);
				actor->name = "Actor";
				SUtils.CreateUnique(actor->name);
				actor->Initialize();
				m_selected = actor.get();
				scene->Add(std::move(actor));
			}
			if (ImGui::MenuItem("Sphere"))
			{
				auto actor = CREATE_CLASS_BASE(Actor, "Sphere");
				SUtils.CreateUnique(actor->name);
				actor->Initialize();
				m_selected = actor.get();
				if (camera)
				{
					actor->transform.position = camera->m_owner->transform.position + camera->m_owner->transform.Forward() * 3.0f;
				}
				scene->Add(std::move(actor));
			}
			if (ImGui::MenuItem("Cube"))
			{
				auto actor = CREATE_CLASS_BASE(Actor, "Cube");
				SUtils.CreateUnique(actor->name);
				actor->Initialize();
				m_selected = actor.get();
				if (camera)
				{
					actor->transform.position = camera->m_owner->transform.position + camera->m_owner->transform.Forward() * 3.0f;
				}
				scene->Add(std::move(actor));
			}
			if (ImGui::MenuItem("Camera"))
			{
				auto actor = CREATE_CLASS_BASE(Actor, "Camera");
				SUtils.CreateUnique(actor->name);
				actor->Initialize();
				m_selected = actor.get(); 
				if (camera)
				{
					actor->transform.position = camera->m_owner->transform.position + camera->m_owner->transform.Forward() * 3.0f;
				}
				scene->Add(std::move(actor));
			}
			if (ImGui::MenuItem("Light"))
			{
				auto actor = CREATE_CLASS_BASE(Actor, "Light");
				SUtils.CreateUnique(actor->name);
				actor->Initialize();
				m_selected = actor.get();
				if (camera)
				{
					actor->transform.position = camera->m_owner->transform.position + camera->m_owner->transform.Forward() * 3.0f;
				}
				scene->Add(std::move(actor));
			}
			ImGui::EndPopup();
		}

		for (auto& actor : scene->m_actors)
		{
			if (ImGui::Selectable(actor->name.c_str(), actor.get() == m_selected))
			{
				m_selected = actor.get();
			}
		}
		ImGui::EndChild();
		
		ImGui::End();

		//Show inspector		
		ImGui::Begin("Inspector");
		if (m_selected)
		{
			m_selected->ProcessGui();

			//Delete selected actor
			if (ImGui::IsKeyPressed(ImGuiKey_Delete))
			{
				auto actor = dynamic_cast<Actor*>(m_selected);
				if (actor)
				{
					scene->Remove(actor);
					m_selected = nullptr;
				}
			}
		}


		
		ImGui::End();
	}
	
}


