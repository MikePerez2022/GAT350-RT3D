#pragma once
#include "Framework/World.h"
#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"
#include <vector>

namespace nc
{
	class World05 : public World
	{
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_angle = 0;
		float m_time = 0;
		float m_speed = 0.5f;
		float m_refraction = 1.0f;

		glm::vec3 ambientLight{ 0.05f, 0.05f, 0.05f };
	};
}