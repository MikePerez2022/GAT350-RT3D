#pragma once
#include "Framework/World.h"
#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"
#include <vector>

//These don't work. check them later. for now substitution hardcoding is your best friend
#define POINT			 0;
#define DIRECTIONAL		 1;
#define SPOT			 2;

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
		float cuttoff;
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

		Light_t m_light;
		glm::vec3 ambientLight{ 0.05f, 0.05f, 0.05f };

		Transform m_transform;
		res_t<Model> m_model;

	};
}