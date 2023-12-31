#pragma once
#include "Texture.h"
#include <vector>

namespace nc
{
	class Cubemap : public Texture
	{
	public:
		bool Create(std::string filename, ...) override;
		bool Load(const std::vector<std::string>& filenames);

	private:
		//std::vector<std::string> GenerateCubeMapNames(const std::string& basename, const std::string& extension);
	};
}