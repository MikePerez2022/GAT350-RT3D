#pragma once

#include "Texture.h"
#include "Model.h"
#include "Material.h"
#include "Shader.h"
#include <vector>

namespace nc
{
	class Editor
	{
	public:
		enum eResourceTypes
		{
			TEXTURE,
			MODEL,
			MATERIAL,
			SHADER
		};

	public:
		
		void Update();
		void ProcessGui(class Scene* scene);

	private:
		bool m_active = true;
		class Object* m_selected = nullptr;

		eResourceTypes resourceType = TEXTURE;
	};
}