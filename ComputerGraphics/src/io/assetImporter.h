#pragma once

#include <string>
#include <vector>

#include "../model_data/material.h"
#include "../model_data/vertex.h"

namespace assetimporter {

	void loadModel(const std::string& dir,
				   const std::string& file,
				   std::vector<Vertex>& vertices,
				   std::vector<uint32_t>& indices,
				   std::vector<std::string>& textures,
				   Material& material);



};
