#pragma once
#include "Framework/World.h"
#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"
#include <vector>

namespace nc
{
	class World04 : public World
	{
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_angle = 0;
		float m_time;
		float m_speed = 5;

		glm::vec3 position{ 0, 8, 0 };
		glm::vec3 color{ 1, 1, 1 };
		glm::vec3 ambientLight{ 0.2f, 0.2f, 0.2f };

		Transform m_transform;
		res_t<Model> m_model;

	};
}