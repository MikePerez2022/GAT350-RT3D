#pragma once
#include "Framework/World.h"
#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"
#include <vector>

namespace nc
{
	struct Light_t
	{
		enum etype
		{
			Point,
			Directional,
			Spot
		};

		etype type;
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 color;
		float intensity;
		float range;
		float innerAngle;
		float outerAngle;
	};

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

		//changed to dynamic
		std::vector<Light_t> m_lights;
		//Light_t m_lights[3];

		int m_selected = 0;
		glm::vec3 ambientLight{ 0.05f, 0.05f, 0.05f };

		Transform m_transform;
		res_t<Model> m_model;

	};
}